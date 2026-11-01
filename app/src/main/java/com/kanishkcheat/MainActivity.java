package com.kanishkcheat;

import android.Manifest;
import android.app.ActionBar;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.Settings;
import android.widget.FrameLayout;
import android.widget.Toast;

public class MainActivity extends Activity {

    private static final int OVERLAY_PERMISSION_REQUEST_CODE = 100;
    private static final int STORAGE_PERMISSION_REQUEST_CODE = 101;

    private boolean permissionsGranted = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        ActionBar bar = getActionBar();
        if (bar != null) bar.hide();

        FrameLayout layout = new FrameLayout(this);
        layout.setBackgroundColor(Color.parseColor("#000000"));
        setContentView(layout);

        // SECURITY: every app entry invalidates any previous session and tears
        // down leftover overlays, so the menu can never appear without a fresh
        // verified login in this process.
        AuthGate.reset();
        Menu.destroyInstance();

        // Native anti-debug / anti-tamper watchdog.
        NativeGuard.init(this);

        checkPermissions();
    }

    private void checkPermissions() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && !Settings.canDrawOverlays(this)) {
            permissionsGranted = false;
            Intent intent = new Intent(
                    Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
                    Uri.parse("package:" + getPackageName()));
            startActivityForResult(intent, OVERLAY_PERMISSION_REQUEST_CODE);
            Toast.makeText(this, "Please grant Overlay Permission", Toast.LENGTH_LONG).show();
        } else {
            checkStoragePermission();
        }
    }

    private void checkStoragePermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M &&
                checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
            permissionsGranted = false;
            requestPermissions(new String[]{
                    Manifest.permission.WRITE_EXTERNAL_STORAGE
            }, STORAGE_PERMISSION_REQUEST_CODE);
        } else {
            if (!permissionsGranted) {
                permissionsGranted = true;
                startLogin();
            }
        }
    }

    private void startLogin() {
        new Login(MainActivity.this);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == OVERLAY_PERMISSION_REQUEST_CODE) {
            checkPermissions();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == STORAGE_PERMISSION_REQUEST_CODE) {
            checkPermissions();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Menu.destroyInstance();
        // Hard-kill the process so the overlay window is guaranteed gone
        android.os.Process.killProcess(android.os.Process.myPid());
    }

    // Called when user swipes the app away from recents
    @Override
    public void onTrimMemory(int level) {
        super.onTrimMemory(level);
        if (level >= TRIM_MEMORY_UI_HIDDEN) {
            Menu.destroyInstance();
        }
    }
}
