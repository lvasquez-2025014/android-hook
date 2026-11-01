#include <map>
#include <mutex>
#include <string>
#include <codecvt>
#include <locale>
#include <jni.h>
#include <unistd.h>
#include "Il2cppp.h"
#include "xdl/include/xdl.h"

typedef unsigned short UTF16;
typedef wchar_t UTF32;
typedef char UTF8;

namespace {
    const void *(*il2cpp_assembly_get_image)(const void *assembly) = nullptr;
    void *(*il2cpp_domain_get)() = nullptr;
    void **(*il2cpp_domain_get_assemblies)(const void *domain, size_t *size) = nullptr;
    const char *(*il2cpp_image_get_name)(void *image) = nullptr;
    void *(*il2cpp_class_from_name)(const void *image, const char *namespaze, const char *name) = nullptr;
    void *(*il2cpp_class_get_field_from_name)(void *klass, const char *name) = nullptr;
    void *(*il2cpp_class_get_method_from_name)(void *klass, const char *name, int argsCount) = nullptr;
    size_t (*il2cpp_field_get_offset)(void *field) = nullptr;
    void (*il2cpp_field_static_get_value)(void *field, void *value) = nullptr;
    void (*il2cpp_field_static_set_value)(void *field, void *value) = nullptr;
    void *(*il2cpp_array_new)(void *elementTypeInfo, size_t length) = nullptr;
    uint16_t *(*il2cpp_string_chars)(void *str) = nullptr;
    Il2CppString *(*il2cpp_string_new)(const char *str) = nullptr;
    Il2CppString *(*il2cpp_string_new_utf16)(const wchar_t *str, int32_t length) = nullptr;
    char *(*il2cpp_type_get_name)(void *type) = nullptr;
    void* (*il2cpp_method_get_param)(void *method, uint32_t index) = nullptr;
    void* (*il2cpp_class_get_methods)(void *klass, void* *iter) = nullptr;
    const char* (*il2cpp_method_get_name)(void *method) = nullptr;
    void *(*il2cpp_object_new)(void *klass) = nullptr;
}

static std::mutex g_il2cppMutex;
static std::map<std::string, void *> g_image_cache;
static std::map<std::string, void *> g_class_cache;
static std::map<std::string, void *> g_method_cache;
static std::map<std::string, size_t> g_field_cache;
static std::map<std::string, size_t> g_static_field_cache;

int is_surrogate(UTF16 uc) {
    return (uc - 0xd800u) < 2048u;
}

int is_high_surrogate(UTF16 uc) {
    return (uc & 0xfffffc00) == 0xd800;
}

int is_low_surrogate(UTF16 uc) {
    return (uc & 0xfffffc00) == 0xdc00;
}

UTF32 surrogate_to_utf32(UTF16 high, UTF16 low) {
    return (high << 10) + low - 0x35fdc00;
}

const char* utf16_to_utf8(const UTF16* source, size_t len) {
    std::u16string s(source, source + len);
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.to_bytes(s).c_str();
}

uintptr_t _StaticClass = 0x5c;

const wchar_t* utf16_to_utf32(const UTF16* source, size_t len) {
    auto output = new UTF32[len + 1];

    for (int i = 0; i < len; i++) {
        const UTF16 uc = source[i];
        if (!is_surrogate(uc)) {
            output[i] = uc;
        }
        else {
            if (is_high_surrogate(uc) && is_low_surrogate(source[i]))
                output[i] = surrogate_to_utf32(uc, source[i]);
            else
                output[i] = L'?';
        }
    }

    output[len] = L'\0';
    return output;
}

const char* Il2CppString::CString() {
    return utf16_to_utf8(&this->start_char, this->length);
}

const wchar_t* Il2CppString::WCString() {
    return utf16_to_utf32(&this->start_char, this->length);
}

Il2CppString *Il2CppString::Create(const char *s) {
    if (!il2cpp_string_new) return nullptr;
    return il2cpp_string_new(s);
}

Il2CppString *Il2CppString::Create(const wchar_t *s, int len) {
    if (!il2cpp_string_new_utf16) return nullptr;
    return il2cpp_string_new_utf16(s, len);
}

void *Il2CppGetImageByName(const char *image) {
    if (!image || !il2cpp_domain_get || !il2cpp_domain_get_assemblies) return nullptr;
    std::string key = image;
    {
        std::lock_guard<std::mutex> lock(g_il2cppMutex);
        auto it = g_image_cache.find(key);
        if (it != g_image_cache.end()) return it->second;
    }
    size_t size = 0;
    void **assemblies = il2cpp_domain_get_assemblies(il2cpp_domain_get(), &size);
    if (!assemblies) return nullptr;
    for (size_t i = 0; i < size; ++i) {
        void *img = (void *)il2cpp_assembly_get_image(assemblies[i]);
        if (!img) continue;
        const char *img_name = il2cpp_image_get_name(img);
        if (img_name && strcmp(img_name, image) == 0) {
            std::lock_guard<std::mutex> lock(g_il2cppMutex);
            g_image_cache[key] = img;
            return img;
        }
    }
    return nullptr;
}

void *Il2CppGetClassType(const char *image, const char *namespaze, const char *clazz) {
    if (!image || !clazz) return nullptr;
    std::string s = std::string(image) + "::" + (namespaze ? namespaze : "") + "::" + clazz;
    {
        std::lock_guard<std::mutex> lock(g_il2cppMutex);
        auto it = g_class_cache.find(s);
        if (it != g_class_cache.end()) return it->second;
    }
    void *img = Il2CppGetImageByName(image);
    if (!img) return nullptr;
    void *klass = il2cpp_class_from_name(img, namespaze, clazz);
    if (!klass) return nullptr;
    std::lock_guard<std::mutex> lock(g_il2cppMutex);
    g_class_cache[s] = klass;
    return klass;
}

void *Il2CppCreateClassInstance(const char *image, const char *namespaze, const char *clazz) {
    if (!il2cpp_object_new) return nullptr;
    void *klass = Il2CppGetClassType(image, namespaze, clazz);
    if (!klass) return nullptr;
    return il2cpp_object_new(klass);
}

void* Il2CppCreateArray(const char *image, const char *namespaze, const char *clazz, size_t length) {
    if (!il2cpp_array_new) return nullptr;
    void *klass = Il2CppGetClassType(image, namespaze, clazz);
    if (!klass) return nullptr;
    return il2cpp_array_new(klass, length);
}

void Il2CppGetStaticFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void *output) {
    if (!il2cpp_field_static_get_value || !output) return;
    void *klass = Il2CppGetClassType(image, namespaze, clazz);
    if (!klass) return;
    void *field = il2cpp_class_get_field_from_name(klass, name);
    if (!field) return;
    il2cpp_field_static_get_value(field, output);
}

void Il2CppSetStaticFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void* value) {
    if (!il2cpp_field_static_set_value || !value) return;
    void *klass = Il2CppGetClassType(image, namespaze, clazz);
    if (!klass) return;
    void *field = il2cpp_class_get_field_from_name(klass, name);
    if (!field) return;
    il2cpp_field_static_set_value(field, value);
}

void *Il2CppGetMethodOffset(const char *image, const char *namespaze, const char *clazz, const char *name, int argsCount) {
    if (!image || !clazz || !name) return nullptr;
    std::string key = std::string(image) + "::" + (namespaze ? namespaze : "") + "::" + clazz + "::" + name + "#" + std::to_string(argsCount);
    {
        std::lock_guard<std::mutex> lock(g_il2cppMutex);
        auto it = g_method_cache.find(key);
        if (it != g_method_cache.end()) return it->second;
    }
    void *klass = Il2CppGetClassType(image, namespaze, clazz);
    if (!klass) return nullptr;
    void **method = (void**)il2cpp_class_get_method_from_name(klass, name, argsCount);
    void *res = method ? *method : nullptr;
    std::lock_guard<std::mutex> lock(g_il2cppMutex);
    g_method_cache[key] = res;
    return res;
}

void *Il2CppGetMethodOffset(const char *image, const char *namespaze, const char *clazz, const char *name, char** args, int argsCount) {
    if (!image || !clazz || !name) return nullptr;
    std::string key = std::string(image) + "::" + (namespaze ? namespaze : "") + "::" + clazz + "::" + name + "#typed#" + std::to_string(argsCount);
    {
        std::lock_guard<std::mutex> lock(g_il2cppMutex);
        auto it = g_method_cache.find(key);
        if (it != g_method_cache.end()) return it->second;
    }
    void *klass = Il2CppGetClassType(image, namespaze, clazz);
    if (!klass) return nullptr;
    void *iter = nullptr;
    void **method = (void**)il2cpp_class_get_methods(klass, &iter);

    while (method) {
        const char *fname = il2cpp_method_get_name(method);
        if (fname && strcmp(fname, name) == 0) {
            int score = 0;
            for (int i = 0; i < argsCount; i++) {
                void *arg = il2cpp_method_get_param(method, i);
                if (arg) {
                    const char *tname = il2cpp_type_get_name(arg);
                    if (tname && strcmp(tname, args[i]) == 0) {
                        score++;
                    } else {
                        break;
                    }
                }
            }
            if (score == argsCount) {
                void *res = *method;
                std::lock_guard<std::mutex> lock(g_il2cppMutex);
                g_method_cache[key] = res;
                return res;
            }
        }
        method = (void **)il2cpp_class_get_methods(klass, &iter);
    }
    return nullptr;
}

size_t Il2CppGetFieldOffset(const char *image, const char *namespaze, const char *clazz, const char *name) {
    if (!image || !clazz || !name) return (size_t)-1;
    std::string key = std::string(image) + "::" + (namespaze ? namespaze : "") + "::" + clazz + "::" + name;
    {
        std::lock_guard<std::mutex> lock(g_il2cppMutex);
        auto it = g_field_cache.find(key);
        if (it != g_field_cache.end()) return it->second;
    }
    void *klass = Il2CppGetClassType(image, namespaze, clazz);
    if (!klass) return (size_t)-1;
    void *field = il2cpp_class_get_field_from_name(klass, name);
    if (!field) return (size_t)-1;
    size_t result = il2cpp_field_get_offset(field);
    std::lock_guard<std::mutex> lock(g_il2cppMutex);
    g_field_cache[key] = result;
    return result;
}

size_t Il2CppGetStaticFieldOffset(const char *image, const char *namespaze, const char *clazz, const char *name){
    if (!image || !clazz || !name) return (size_t)-1;
    std::string key = std::string(image) + "::" + (namespaze ? namespaze : "") + "::" + clazz + "::" + name;
    {
        std::lock_guard<std::mutex> lock(g_il2cppMutex);
        auto it = g_static_field_cache.find(key);
        if (it != g_static_field_cache.end()) return it->second;
    }
    void *klass = Il2CppGetClassType(image, namespaze, clazz);
    if (!klass) return (size_t)-1;
    FieldInfo *field = (FieldInfo*)il2cpp_class_get_field_from_name(klass, name);
    if (!field || !field->parent) return (size_t)-1;
    size_t result = (unsigned long)((uint64_t)field->parent->static_fields + field->offset);
    std::lock_guard<std::mutex> lock(g_il2cppMutex);
    g_static_field_cache[key] = result;
    return result;
}

bool Il2CppIsAssembliesLoaded() {
    if (!il2cpp_domain_get || !il2cpp_domain_get_assemblies) return false;
    size_t size = 0;
    void **assemblies = il2cpp_domain_get_assemblies(il2cpp_domain_get(), &size);
    return size != 0 && assemblies != nullptr;
}

void Il2CppAttach(const char *name) {
    void *handle = xdl_open(name, 0);
    while (!handle) {
        handle = xdl_open(name, 0);
        sleep(1);
    }
    il2cpp_assembly_get_image = (const void *(*)(const void *)) xdl_sym(handle, "il2cpp_assembly_get_image", 0);
    il2cpp_domain_get = (void *(*)()) xdl_sym(handle, "il2cpp_domain_get", 0);
    il2cpp_domain_get_assemblies = (void **(*)(const void* , size_t*)) xdl_sym(handle, "il2cpp_domain_get_assemblies", 0);
    il2cpp_image_get_name = (const char *(*)(void *)) xdl_sym(handle, "il2cpp_image_get_name", 0);
    il2cpp_class_from_name = (void* (*)(const void*, const char*, const char *)) xdl_sym(handle, "il2cpp_class_from_name", 0);
    il2cpp_class_get_field_from_name = (void* (*)(void*, const char *)) xdl_sym(handle, "il2cpp_class_get_field_from_name", 0);
    il2cpp_class_get_method_from_name = (void* (*)(void *, const char*, int)) xdl_sym(handle, "il2cpp_class_get_method_from_name", 0);
    il2cpp_field_get_offset = (size_t (*)(void *)) xdl_sym(handle, "il2cpp_field_get_offset", 0);
    il2cpp_field_static_get_value = (void (*)(void*, void *)) xdl_sym(handle, "il2cpp_field_static_get_value", 0);
    il2cpp_field_static_set_value = (void (*)(void*, void *)) xdl_sym(handle, "il2cpp_field_static_set_value", 0);
    il2cpp_array_new = (void *(*)(void*, size_t)) xdl_sym(handle, "il2cpp_array_new", 0);
    il2cpp_string_chars = (uint16_t *(*)(void*)) xdl_sym(handle, "il2cpp_string_chars", 0);
    il2cpp_string_new = (Il2CppString *(*)(const char *)) xdl_sym(handle, "il2cpp_string_new", 0);
    il2cpp_string_new_utf16 = (Il2CppString *(*)(const wchar_t *, int32_t)) xdl_sym(handle, "il2cpp_string_new", 0);
    il2cpp_type_get_name = (char *(*)(void *)) xdl_sym(handle, "il2cpp_type_get_name", 0);
    il2cpp_method_get_param = (void *(*)(void *, uint32_t)) xdl_sym(handle, "il2cpp_method_get_param", 0);
    il2cpp_class_get_methods = (void *(*)(void *, void **)) xdl_sym(handle, "il2cpp_class_get_methods", 0);
    il2cpp_method_get_name = (const char *(*)(void *)) xdl_sym(handle, "il2cpp_method_get_name", 0);
    il2cpp_object_new = (void *(*)(void *)) xdl_sym(handle, "il2cpp_object_new", 0);
    xdl_close(handle);
}
