package com.kanishkcheat;

import android.content.Context;

/**
 * Native anti-tamper guard.
 *
 * Started from MainActivity.onCreate(). Once running, the native watchdog
 * periodically checks for debuggers (TracerPid), Frida/Gadget/Xposed/Substrate
 * mappings and Frida default ports, and hard-kills the process on detection.
 *
 * Packer-compatible: if a tracer is already attached (Virbox and similar
 * protectors keep their own tracer for the process lifetime), the watchdog
 * stands down quietly instead of killing the app.
 */
public final class NativeGuard {
    private NativeGuard() {}

    static {
        // Never let a failed native load crash the Activity before its UI is up
        // (packers like Virbox can restrict dlopen at startup).
        try {
            System.loadLibrary("amithnew");
        } catch (Throwable ignored) {
        }
    }

    /**
     * Arms the native watchdog (idempotent: only the first call starts it).
     * Never throws - if the native side is unavailable the app keeps working.
     */
    public static synchronized void init(Context context) {
        try {
            nativeInit();
        } catch (Throwable ignored) {
        }
    }

    private static native boolean nativeInit();

    // --- Anti-skip login: session token lives in native memory ---
    // The token is generated on the Java side after successful Crypton Auth login and mirrored natively.
    // mirrored into the native watchdog. Opening the menu re-verifies it against
    // the native copy, so forcing the Java flag alone can never open the panel.
    public static native void nativeSetSessionToken(String token);
    public static native boolean nativeVerifySessionToken(String token);
    public static native void nativeMarkCorrupt(Context context);


    private static volatile String localToken = null;

    /** Store the token both natively and locally (idempotent). */
    public static synchronized void setSessionToken(String token) {
        localToken = token;
        try { nativeSetSessionToken(token); } catch (Throwable ignored) {}
    }

    /** Verify the token against the native-cached copy. */
    public static synchronized boolean verifySessionToken(String token) {
        if (token == null) return false;
        if (!token.equals(localToken)) return false;
        try { return nativeVerifySessionToken(token); } catch (Throwable ignored) { return false; }
    }

    /** Persist a corruption flag natively; used when a crack is detected. */
    public static void markCorrupt(Context context) {
        try { nativeMarkCorrupt(context); } catch (Throwable ignored) {}
    }
}
