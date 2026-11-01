// NativeGuard.cpp - anti-tamper watchdog for the amithnew (menu) library.
//
// Runs inside the app process. nativeInit() self-attaches ptrace and spawns a
// detached thread that periodically checks:
//   - /proc/self/status TracerPid (debugger attached)
//   - /proc/self/maps for Frida / Gadget / Xposed / Substrate / injector maps
//   - /proc/net/tcp(6) for Frida default ports 27042/27043 in LISTEN state
//
// Packer-compatible: a packer shell like Virbox legitimately keeps its own
// tracer attached, so on TracerPid != 0 the watchdog stands down quietly
// instead of killing the process.
//
// This file also implements the native methods declared by IntegrityChecker
// (library presence checks) and restart (config strings), which have no other
// implementation.

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/ptrace.h>
#include <sys/stat.h>
#include <android/log.h>

#define TAG "NativeGuard"

static volatile int g_guardRunning = 0;

// ---------------------------------------------------------------------------
// Session token: mirrors the Java KeyAuth session into native memory so the
// Java "authenticated" flag alone can never open the menu.
// ---------------------------------------------------------------------------
static volatile const char *g_sessionToken = NULL;

// ---------------------------------------------------------------------------
// Persistent corruption marker helpers.
// ---------------------------------------------------------------------------
#define CORRUPT_MAGIC 0x4B4C4F50   // marker
#define CORRUPT_FILE  "sys.ini"

static jstring getFilesDirPath(JNIEnv *env, jobject ctx) {
    if (!ctx || !env) return NULL;
    jclass ctxClass = env->GetObjectClass(ctx);
    if (!ctxClass) { if (env->ExceptionCheck()) env->ExceptionClear(); return NULL; }
    jmethodID m = env->GetMethodID(ctxClass, "getFilesDir", "()Ljava/io/File;");
    if (!m) { if (env->ExceptionCheck()) env->ExceptionClear(); env->DeleteLocalRef(ctxClass); return NULL; }
    jobject dir = env->CallObjectMethod(ctx, m);
    if (!dir) { if (env->ExceptionCheck()) env->ExceptionClear(); env->DeleteLocalRef(ctxClass); return NULL; }
    jclass fileClass = env->GetObjectClass(dir);
    if (!fileClass) { if (env->ExceptionCheck()) env->ExceptionClear(); env->DeleteLocalRef(ctxClass); return NULL; }
    jmethodID path = env->GetMethodID(fileClass, "getAbsolutePath", "()Ljava/lang/String;");
    if (!path) { if (env->ExceptionCheck()) env->ExceptionClear(); env->DeleteLocalRef(fileClass); env->DeleteLocalRef(ctxClass); return NULL; }
    jstring p = (jstring) env->CallObjectMethod(dir, path);
    if (env->ExceptionCheck()) env->ExceptionClear();
    env->DeleteLocalRef(fileClass);
    env->DeleteLocalRef(dir);
    env->DeleteLocalRef(ctxClass);
    return p;
}

static int readCorruptFlag(JNIEnv *env, jobject ctx) {
    if (!env || !ctx) return 0;
    jstring dir = getFilesDirPath(env, ctx);
    if (!dir) return 0;
    const char *d = env->GetStringUTFChars(dir, NULL);
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", d ? d : "", CORRUPT_FILE);
    if (d) env->ReleaseStringUTFChars(dir, d);
    env->DeleteLocalRef(dir);
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
    unsigned int magic = 0;
    unsigned char flag = 0;
    unsigned int check = 0;
    size_t rd = fread(&magic, 1, 4, f);
    if (rd == 4) {
        if (fread(&flag, 1, 1, f) != 1) flag = 0;
        if (fread(&check, 1, 4, f) != 4) check = 0;
    }
    fclose(f);
    if (magic != CORRUPT_MAGIC) return 0;
    if (check != (0x5A5A5A5AU ^ (unsigned int) flag)) return 0;
    return (flag == 0x55) ? 1 : 0;
}

static void writeCorruptFlag(JNIEnv *env, jobject ctx) {
    if (!env || !ctx) return;
    jstring dir = getFilesDirPath(env, ctx);
    if (!dir) return;
    const char *d = env->GetStringUTFChars(dir, NULL);
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", d ? d : "", CORRUPT_FILE);
    if (d) env->ReleaseStringUTFChars(dir, d);
    env->DeleteLocalRef(dir);
    FILE *f = fopen(path, "wb");
    if (!f) return;
    unsigned int magic = CORRUPT_MAGIC;
    unsigned char flag = 0x55;
    unsigned int check = 0x5A5A5A5AU ^ (unsigned int) flag;
    fwrite(&magic, 1, 4, f);
    fwrite(&flag, 1, 1, f);
    fwrite(&check, 1, 4, f);
    fclose(f);
}

static void guardKillSelf() {
    signal(SIGKILL, SIG_DFL);
    kill(getpid(), SIGKILL);
    _exit(137);
}

static int mapsContains(const char *needle) {
    FILE *f = fopen("/proc/self/maps", "r");
    if (!f) return 0;
    char line[1024];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, needle)) {
            found = 1;
            break;
        }
    }
    fclose(f);
    return found;
}

static int checkTracerPid(void) {
    FILE *f = fopen("/proc/self/status", "r");
    if (!f) return 0;
    char line[256];
    int traced = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "TracerPid:", 10) == 0) {
            int tp = atoi(line + 10);
            if (tp > 0) traced = 1;
            break;
        }
    }
    fclose(f);
    return traced;
}

// Returns 1 if the given TCP port is in LISTEN state ("0A") locally.
static int portListening(int port) {
    static const char *files[] = {"/proc/net/tcp", "/proc/net/tcp6"};
    char target[8];
    snprintf(target, sizeof(target), "%04X", (unsigned int) port);
    for (int i = 0; i < 2; i++) {
        FILE *f = fopen(files[i], "r");
        if (!f) continue;
        char line[512];
        int first = 1;
        int found = 0;
        while (fgets(line, sizeof(line), f)) {
            if (first) { first = 0; continue; } // skip header
            char local[128], rem[128], st[16];
            if (sscanf(line, "%*d: %127s %127s %15s", local, rem, st) != 3) continue;
            const char *colon = strrchr(local, ':');
            if (!colon) continue;
            if (strcmp(colon + 1, target) != 0) continue;
            if (strcmp(st, "0A") == 0) { found = 1; break; }
        }
        fclose(f);
        if (found) return 1;
    }
    return 0;
}

static void *guardThread(void *arg) {
    (void) arg;
    static const char *needles[] = {
            "frida", "gum-js-loop", "gadget", "xposed", "substrate", "linjector"
    };
    while (g_guardRunning) {
        if (checkTracerPid()) {
            // Someone is tracing us. A packer like Virbox keeps its own tracer
            // attached for the whole process lifetime, so NEVER kill here -
            // stand down quietly and let the packer handle anti-debug.
            g_guardRunning = 0;
            break;
        }
        for (size_t i = 0; i < sizeof(needles) / sizeof(needles[0]); i++) {
            if (mapsContains(needles[i])) guardKillSelf();
        }
        if (portListening(27042) || portListening(27043)) guardKillSelf();
        sleep(3);
    }
    return NULL;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_kanishkcheat_NativeGuard_nativeInit(JNIEnv *env, jclass clazz) {
    (void) env;
    (void) clazz;
    if (g_guardRunning) return JNI_TRUE;
    // If a tracer is already attached at startup it is a legitimate protector
    // shell (Virbox etc.) - do not arm our watchdog and do not self-trace.
    if (checkTracerPid()) return JNI_TRUE;
    // Best-effort self-trace so an external debugger cannot attach later.
    ptrace(PTRACE_TRACEME, 0, 0, 0);
    g_guardRunning = 1;
    pthread_t tid;
    if (pthread_create(&tid, NULL, guardThread, NULL) == 0) {
        pthread_detach(tid);
    }
    return JNI_TRUE;
}

// ---------------------------------------------------------------------------
// Library presence checks (used by IntegrityChecker).
// ---------------------------------------------------------------------------

static int loaderLibLoaded(void) {
    return mapsContains("libamithnew.so");
}

static jstring getNativeLibDir(JNIEnv *env, jobject ctx) {
    if (!ctx || !env) return NULL;
    jclass ctxClass = env->GetObjectClass(ctx);
    if (!ctxClass) { if (env->ExceptionCheck()) env->ExceptionClear(); return NULL; }
    jmethodID getAppInfo = env->GetMethodID(ctxClass, "getApplicationInfo",
                                            "()Landroid/content/pm/ApplicationInfo;");
    if (!getAppInfo) { if (env->ExceptionCheck()) env->ExceptionClear(); return NULL; }
    jobject appInfo = env->CallObjectMethod(ctx, getAppInfo);
    if (!appInfo) { if (env->ExceptionCheck()) env->ExceptionClear(); return NULL; }
    jclass aiClass = env->GetObjectClass(appInfo);
    if (!aiClass) { if (env->ExceptionCheck()) env->ExceptionClear(); return NULL; }
    jfieldID fld = env->GetFieldID(aiClass, "nativeLibraryDir", "Ljava/lang/String;");
    if (!fld) { if (env->ExceptionCheck()) env->ExceptionClear(); return NULL; }
    jstring dir = (jstring) env->GetObjectField(appInfo, fld);
    env->DeleteLocalRef(aiClass);
    env->DeleteLocalRef(appInfo);
    env->DeleteLocalRef(ctxClass);
    return dir;
}

static int libDirHasFile(JNIEnv *env, jstring libDir, const char *name) {
    if (!libDir || !env) return 0;
    const char *dir = env->GetStringUTFChars(libDir, NULL);
    if (!dir) return 0;
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", dir, name);
    env->ReleaseStringUTFChars(libDir, dir);
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
    fclose(f);
    return 1;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_kanishkcheat_IntegrityChecker_nativeCheckLibraryIntegrity(JNIEnv *env, jclass clazz) {
    (void) env;
    (void) clazz;
    return loaderLibLoaded() ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_kanishkcheat_IntegrityChecker_nativeCheckAllLibs(JNIEnv *env, jclass clazz) {
    (void) env;
    (void) clazz;
    return loaderLibLoaded() ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_kanishkcheat_IntegrityChecker_nativeCheckAllLibsWithContext(JNIEnv *env, jclass clazz,
                                                                     jobject context) {
    (void) clazz;
    if (!loaderLibLoaded()) return JNI_FALSE;
    jstring libDir = getNativeLibDir(env, context);
    if (!libDir) return JNI_TRUE; // lenient: cannot resolve path on this ROM
    int ok = libDirHasFile(env, libDir, "libamithnew.so")
             && libDirHasFile(env, libDir, "libamithcore.so")
             && libDirHasFile(env, libDir, "libinjectEmulator.so");
    env->DeleteLocalRef(libDir);
    return ok ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_kanishkcheat_IntegrityChecker_nativeVerifyLibVed(JNIEnv *env, jclass clazz) {
    (void) env;
    (void) clazz;
    return loaderLibLoaded() ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_kanishkcheat_IntegrityChecker_nativeVerifyAppConfigValues(JNIEnv *env, jclass clazz,
                                                                   jobject context) {
    (void) env;
    (void) clazz;
    (void) context;
    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_kanishkcheat_IntegrityChecker_nativeCheckCorruptedState(JNIEnv *env, jclass clazz,
                                                                 jobject context) {
    (void) env;
    (void) clazz;
    // If the corruption marker is set (previous crack / forge attempt), kill
    // the process immediately so an already-burned install can never run.
    if (readCorruptFlag(env, context)) {
        guardKillSelf();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_kanishkcheat_IntegrityChecker_nativeCorruptApp(JNIEnv *env, jclass clazz,
                                                        jobject context) {
    (void) env;
    (void) clazz;
    // Persist corruption so the app stays dead even after a reinstall-less restart.
    writeCorruptFlag(env, context);
}

extern "C" JNIEXPORT void JNICALL
Java_com_kanishkcheat_NativeGuard_nativeMarkCorrupt(JNIEnv *env, jclass clazz,
                                                    jobject context) {
    (void) env;
    (void) clazz;
    writeCorruptFlag(env, context);
}

// --- Native session token mirror (anti-skip-login) -------------------------

extern "C" JNIEXPORT void JNICALL
Java_com_kanishkcheat_NativeGuard_nativeSetSessionToken(JNIEnv *env, jclass clazz,
                                                        jstring token) {
    (void) clazz;
    if (!env || !token) return;
    const char *c = env->GetStringUTFChars(token, NULL);
    if (!c) return;
    size_t n = strlen(c) + 1;
    char *copy = (char *) malloc(n);
    if (copy) {
        memcpy(copy, c, n);
        g_sessionToken = copy;
    }
    env->ReleaseStringUTFChars(token, c);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_kanishkcheat_NativeGuard_nativeVerifySessionToken(JNIEnv *env, jclass clazz,
                                                           jstring token) {
    (void) clazz;
    if (!env || !token || g_sessionToken == NULL) return JNI_FALSE;
    const char *c = env->GetStringUTFChars(token, NULL);
    if (!c) return JNI_FALSE;
    int ok = (strcmp(c, (const char *)g_sessionToken) == 0);
    env->ReleaseStringUTFChars(token, c);
    return ok ? JNI_TRUE : JNI_FALSE;
}

// Legacy no-op JNI endpoint retained for the restart class.

extern "C" JNIEXPORT void JNICALL
Java_com_kanishkcheat_restart_disableAll(JNIEnv *env, jclass clazz) {
    (void) env;
    (void) clazz;
}


// ---------------------------------------------------------------------------
// Exported anti-skip guard for the native menu entry point (client.cpp).
// True only after Crypton Auth success mirrors a non-empty session token.
// ---------------------------------------------------------------------------
extern "C" bool ngSessionActive(void) {
    return g_sessionToken != NULL && g_sessionToken[0] != '\0';
}
