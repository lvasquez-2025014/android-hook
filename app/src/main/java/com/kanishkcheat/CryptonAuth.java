package com.kanishkcheat;

import android.content.Context;
import android.provider.Settings;

import org.json.JSONObject;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.security.SecureRandom;
import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;

/** Android Java client for Crypton Auth 2.0. */
public final class CryptonAuth {
    private static final String BASE_URL = "https://gallant-salamander-590.convex.site";
    private static final String AUTH_USERNAME = "jonathansilvaah021k3034";
    private static final String SECRET = "CRY_KBFc6ZqNZQq8olkED3gslcjEASX5sCEn";
    private static final SecureRandom RANDOM = new SecureRandom();

    private CryptonAuth() {}

    public static void init() throws Exception {
        JSONObject response = post(endpoint("dTsqM3Usa3UzNDMu"), "{}");
        if (!response.optBoolean("success", false)) throw apiError(response);
    }

    public static String authenticate(Context context, String username, String password) throws Exception {
        String hwid = Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
        if (hwid == null || hwid.isEmpty()) hwid = "unknown";
        JSONObject body = new JSONObject();
        JSONObject response;
        if (password != null && !password.isEmpty()) {
            body.put("auth_username", username);
            body.put("auth_password", password);
            body.put("hwid", hwid);
            response = post(endpoint("dTsqM3Usa3U2NT0zNA=="), body.toString());
        } else {
            body.put("license_key", username);
            body.put("hwid", hwid);
            response = post(endpoint("dTsqM3Usa3U2Mzk/NCk/"), body.toString());
        }
        if (!response.optBoolean("success", false)) throw apiError(response);
        return java.util.UUID.randomUUID().toString().replace("-", "");
    }

    private static JSONObject post(String path, String body) throws Exception {
        long timestamp = System.currentTimeMillis() / 1000L;
        byte[] nonceBytes = new byte[16];
        RANDOM.nextBytes(nonceBytes);
        String nonce = hex(nonceBytes);
        String signature = hmacSha256(timestamp + ":" + nonce + ":" + body);
        HttpURLConnection connection = (HttpURLConnection) new URL(BASE_URL + path).openConnection();
        connection.setRequestMethod("POST");
        connection.setConnectTimeout(15000);
        connection.setReadTimeout(15000);
        connection.setDoOutput(true);
        connection.setRequestProperty("X-Auth-Username", AUTH_USERNAME);
        connection.setRequestProperty("X-Auth-Timestamp", Long.toString(timestamp));
        connection.setRequestProperty("X-Auth-Nonce", nonce);
        connection.setRequestProperty("X-Auth-Signature", signature);
        connection.setRequestProperty("Content-Type", "application/json");
        try {
            try (OutputStream out = connection.getOutputStream()) {
                out.write(body.getBytes(StandardCharsets.UTF_8));
            }
            int code = connection.getResponseCode();
            InputStream stream = code >= 200 && code < 400 ? connection.getInputStream() : connection.getErrorStream();
            if (stream == null) throw new Exception("Crypton Auth HTTP " + code);
            ByteArrayOutputStream bytes = new ByteArrayOutputStream();
            try (InputStream in = stream) {
                byte[] buffer = new byte[2048];
                int count;
                while ((count = in.read(buffer)) != -1) bytes.write(buffer, 0, count);
            }
            String response = new String(bytes.toByteArray(), StandardCharsets.UTF_8).trim();
            if (response.isEmpty()) throw new Exception("Empty response from Crypton Auth (HTTP " + code + ")");
            return new JSONObject(response);
        } finally {
            connection.disconnect();
        }
    }

    private static String hmacSha256(String message) throws Exception {
        Mac mac = Mac.getInstance("HmacSHA256");
        mac.init(new SecretKeySpec(SECRET.getBytes(StandardCharsets.UTF_8), "HmacSHA256"));
        return hex(mac.doFinal(message.getBytes(StandardCharsets.UTF_8)));
    }

    private static String hex(byte[] bytes) {
        final char[] digits = "0123456789abcdef".toCharArray();
        char[] result = new char[bytes.length * 2];
        for (int i = 0; i < bytes.length; i++) {
            int value = bytes[i] & 0xff;
            result[i * 2] = digits[value >>> 4];
            result[i * 2 + 1] = digits[value & 0x0f];
        }
        return new String(result);
    }

    private static String endpoint(String encoded) {
        byte[] decoded = android.util.Base64.decode(encoded, android.util.Base64.DEFAULT);
        for (int i = 0; i < decoded.length; i++) decoded[i] ^= 0x5a;
        return new String(decoded, StandardCharsets.UTF_8);
    }

    private static Exception apiError(JSONObject response) {
        String error = response.optString("error", response.optString("code", "AUTH_REJECTED"));
        String message = response.optString("message", "");
        return new Exception(message.isEmpty() ? error : error + ": " + message);
    }
}
