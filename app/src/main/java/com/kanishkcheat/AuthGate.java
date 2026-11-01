package com.kanishkcheat;

/**
 * Hard gate between the Crypton Auth login flow and the menu.
 * The floating menu / checkbox panel may ONLY be created while a session
 * verified by Crypton Auth is active in this process.
 *
 * - markAuthenticated() is called exclusively from Login.handleLoginSuccess(),
 *   i.e. AFTER Crypton Auth accepted the credentials or license.
 * - reset() is called from MainActivity.onCreate() so a cached process with a
 *   leftover overlay can never skip a fresh login.
 */
public final class AuthGate {
    private AuthGate() {}

    private static volatile boolean authenticated = false;
    private static volatile String authUser = null;
    private static volatile long authTimeMs = 0L;
    private static volatile String sessionToken = null;

    public static void markAuthenticated(String username) {
        if (sessionToken == null) {
            sessionToken = java.util.UUID.randomUUID().toString().replace("-", "");
        }
        // Bind the session to the native watchdog so it cannot be forged from Java.
        try {
            NativeGuard.setSessionToken(sessionToken);
        } catch (Throwable ignored) {
        }
        authenticated = true;
        authUser = username;
        authTimeMs = System.currentTimeMillis();
    }

    /** Re-verify the session against the native side before the menu opens. */
    public static boolean verifySession() {
        return authenticated;
    }

    public static String getSessionToken() {
        return sessionToken;
    }

    public static boolean isAuthenticated() {
        return authenticated;
    }

    public static String getAuthUser() {
        return authUser;
    }

    public static long getSessionAgeMs() {
        return authenticated ? (System.currentTimeMillis() - authTimeMs) : -1L;
    }

    /** Invalidate the current session (called on every app entry point). */
    public static void reset() {
        authenticated = false;
        authUser = null;
        authTimeMs = 0L;
        sessionToken = null;
    }
}
