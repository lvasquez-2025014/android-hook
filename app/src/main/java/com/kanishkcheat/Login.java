package com.kanishkcheat;

import android.animation.ValueAnimator;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.res.ColorStateList;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Matrix;
import android.graphics.Shader;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.RippleDrawable;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.text.InputType;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.Window;
import android.view.WindowManager;
import android.view.animation.DecelerateInterpolator;
import android.view.animation.OvershootInterpolator;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.DataOutputStream;
import java.io.File;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Login {

    // Theme: Reference FF_COVERV7A style - dark background, deep red
    private static final int PrimaryColor = 0xFFE53935;
    private static final int AccentColor = 0xFFB71C1C;
    private static final int CardBgColor = 0xE6101010;
    private static final int CardBorderColor = 0xFF4A2020;

    static {
        try {
            System.loadLibrary("amithnew");
        } catch (Throwable t) {
            t.printStackTrace();
        }
    }

    /**
     * Native credential retrieval from client.cpp.
     * key 0 = BASE_URL, 1 = APP_NAME, 2 = APP_KEY, 3 = APP_SECRET, 4 = VERSION
     */
    private static final String PREFS_NAME = "mvp_prefs";
    private static final String USERNAME_KEY = "username_key";
    private static final String PASSWORD_KEY = "password_key";

    private Context context;
    private Handler mainHandler;
    private SharedPreferences prefs;

    private EditText usernameInput;
    private EditText passwordInput;
    private Button loginBtn;
    private GradientDrawable btnBg;
    private ValueAnimator colorAnimator;

    private int selectedInjectType = 0; // default NON-ROOT

    private final String selectedTargetPackage = "com.dts.freefireth";

    public Login(Context ctx) {
        context = ctx;
        mainHandler = new Handler(Looper.getMainLooper());
        prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE);
        initUI();
    }

    private int dp(int value) {
        return (int) (value * context.getResources().getDisplayMetrics().density);
    }

    private void initUI() {
        // Root container: Full-screen deep dark obsidian background
        FrameLayout root = new FrameLayout(context);
        root.setBackgroundColor(0xFF050505);
        root.setLayoutParams(new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));

        DisplayMetrics metrics = context.getResources().getDisplayMetrics();
        int screenW = metrics.widthPixels;
        int panelWidth = Math.min((int)(screenW * 0.82), dp(320));

        ScrollView scrollView = new ScrollView(context);
        scrollView.setLayoutParams(new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT
        ));
        scrollView.setFillViewport(true);
        scrollView.setVerticalScrollBarEnabled(false);

        LinearLayout contentContainer = new LinearLayout(context);
        contentContainer.setOrientation(LinearLayout.VERTICAL);
        contentContainer.setGravity(Gravity.CENTER);
        contentContainer.setPadding(dp(8), dp(40), dp(8), dp(40));

        // ========== p1 = Brand / Product Header Panel ==========
        LinearLayout p1 = new LinearLayout(context);
        p1.setOrientation(LinearLayout.VERTICAL);
        p1.setGravity(Gravity.CENTER_HORIZONTAL);
        p1.setPadding(dp(14), dp(16), dp(14), dp(16));
        LinearLayout.LayoutParams p1Params = new LinearLayout.LayoutParams(panelWidth, ViewGroup.LayoutParams.WRAP_CONTENT);
        p1Params.setMargins(0, 0, 0, dp(14));
        p1.setLayoutParams(p1Params);

        GradientDrawable p1Bg = new GradientDrawable();
        p1Bg.setColor(CardBgColor);
        p1Bg.setCornerRadius(dp(8));
        p1Bg.setStroke(dp(1), CardBorderColor);
        p1.setBackground(p1Bg);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            p1.setElevation(dp(3));
        }

        // Logo Image
        try {
            ImageView logo = new ImageView(context);
            logo.setImageResource(R.drawable.seu_logo);
            logo.setScaleType(ImageView.ScaleType.FIT_CENTER);
            LinearLayout.LayoutParams logoParams = new LinearLayout.LayoutParams(dp(56), dp(56));
            logoParams.setMargins(0, 0, 0, dp(6));
            logo.setLayoutParams(logoParams);
            p1.addView(logo);
        } catch (Exception ignored) {}

        // Product Name with Shimmering Gradient Shader
        final TextView productName = new TextView(context);
        productName.setText("JN X!T");
        productName.setTextColor(PrimaryColor);
        productName.setTextSize(15);
        productName.setTypeface(Typeface.DEFAULT_BOLD);
        productName.setGravity(Gravity.CENTER_HORIZONTAL);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            productName.setLetterSpacing(0.06f);
        }
        productName.setShadowLayer(dp(2), 0, 0, 0x66E53935);
        LinearLayout.LayoutParams productParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        productParams.setMargins(0, 0, 0, 0);
        productName.setLayoutParams(productParams);
        p1.addView(productName);

        productName.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                if (productName.getWidth() > 0) {
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
                        productName.getViewTreeObserver().removeOnGlobalLayoutListener(this);
                    } else {
                        productName.getViewTreeObserver().removeGlobalOnLayoutListener(this);
                    }
                    int[] colors = { PrimaryColor, 0xFFFFFFFF, PrimaryColor };
                    float[] positions = { 0f, 0.5f, 1f };
                    final LinearGradient shader = new LinearGradient(
                            0, 0, productName.getWidth(), 0,
                            colors, positions,
                            Shader.TileMode.CLAMP
                    );
                    productName.getPaint().setShader(shader);
                    final Matrix matrix = new Matrix();
                    ValueAnimator animator = ValueAnimator.ofFloat(0f, 1f);
                    animator.setDuration(2500);
                    animator.setRepeatCount(ValueAnimator.INFINITE);
                    animator.addUpdateListener(animation -> {
                        float v = (float) animation.getAnimatedValue();
                        float translate = (productName.getWidth() * 3) * v - productName.getWidth();
                        matrix.setTranslate(translate, 0);
                        shader.setLocalMatrix(matrix);
                        productName.invalidate();
                    });
                    animator.start();
                }
            }
        });

        // Copyright / Subtitle
        TextView copyright = new TextView(context);
        copyright.setText("JN X!T");
        copyright.setTextColor(0xFF666666);
        copyright.setTextSize(9);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            copyright.setLetterSpacing(0.03f);
        }
        copyright.setGravity(Gravity.CENTER_HORIZONTAL);
        LinearLayout.LayoutParams copyrightParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        copyrightParams.setMargins(0, dp(4), 0, 0);
        copyright.setLayoutParams(copyrightParams);
        p1.addView(copyright);

        p1.setAlpha(0f);
        p1.setTranslationY(dp(70));
        contentContainer.addView(p1);

        // ========== p2 = Login Panel ==========
        LinearLayout p2 = new LinearLayout(context);
        p2.setOrientation(LinearLayout.VERTICAL);
        p2.setGravity(Gravity.CENTER_HORIZONTAL);
        p2.setPadding(dp(20), dp(16), dp(20), dp(18));
        LinearLayout.LayoutParams p2Params = new LinearLayout.LayoutParams(panelWidth, ViewGroup.LayoutParams.WRAP_CONTENT);
        p2.setLayoutParams(p2Params);

        GradientDrawable p2Bg = new GradientDrawable();
        p2Bg.setColor(CardBgColor);
        p2Bg.setCornerRadius(dp(8));
        p2Bg.setStroke(dp(1), CardBorderColor);
        p2.setBackground(p2Bg);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            p2.setElevation(dp(3));
        }

        // Shimmering LOGIN Title
        final TextView title = new TextView(context);
        title.setText("LOGIN");
        title.setTextColor(PrimaryColor);
        title.setTextSize(18);
        title.setTypeface(Typeface.DEFAULT_BOLD);
        title.setGravity(Gravity.CENTER);
        title.setPadding(0, 0, 0, dp(10));
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            title.setLetterSpacing(0.08f);
        }
        title.setShadowLayer(dp(3), 0, 0, 0x66E53935);
        title.setAlpha(0f);
        title.setTranslationY(dp(20));
        p2.addView(title);

        title.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                if (title.getWidth() > 0) {
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
                        title.getViewTreeObserver().removeOnGlobalLayoutListener(this);
                    } else {
                        title.getViewTreeObserver().removeGlobalOnLayoutListener(this);
                    }
                    int[] colors = { PrimaryColor, 0xFFFFFFFF, PrimaryColor };
                    float[] positions = { 0f, 0.5f, 1f };
                    final LinearGradient shader = new LinearGradient(
                            0, 0, title.getWidth(), 0,
                            colors, positions,
                            Shader.TileMode.CLAMP
                    );
                    title.getPaint().setShader(shader);
                    final Matrix matrix = new Matrix();
                    ValueAnimator animator = ValueAnimator.ofFloat(0f, 1f);
                    animator.setDuration(2500);
                    animator.setRepeatCount(ValueAnimator.INFINITE);
                    animator.addUpdateListener(animation -> {
                        float v = (float) animation.getAnimatedValue();
                        float translate = (title.getWidth() * 3) * v - title.getWidth();
                        matrix.setTranslate(translate, 0);
                        shader.setLocalMatrix(matrix);
                        title.invalidate();
                    });
                    animator.start();
                }
            }
        });

        // USERNAME INPUT
        usernameInput = new EditText(context);
        usernameInput.setHint("Enter Username");
        usernameInput.setSingleLine(true);
        styleInput(usernameInput, true);
        usernameInput.setAlpha(0f);
        usernameInput.setTranslationY(dp(20));
        p2.addView(usernameInput);

        // PASSWORD INPUT
        passwordInput = new EditText(context);
        passwordInput.setHint("Enter Password");
        passwordInput.setSingleLine(true);
        passwordInput.setInputType(InputType.TYPE_CLASS_TEXT | InputType.TYPE_TEXT_VARIATION_PASSWORD);
        styleInput(passwordInput, false);
        passwordInput.setAlpha(0f);
        passwordInput.setTranslationY(dp(20));
        p2.addView(passwordInput);


        // CONTINUE BUTTON
        btnBg = new GradientDrawable();
        btnBg.setCornerRadius(dp(8));
        btnBg.setOrientation(GradientDrawable.Orientation.LEFT_RIGHT);
        btnBg.setColors(new int[]{ PrimaryColor, AccentColor });
        btnBg.setStroke(0, 0);
        RippleDrawable ripple = new RippleDrawable(ColorStateList.valueOf(0x44FFFFFF), btnBg, null);

        loginBtn = new Button(context);
        loginBtn.setText("CONTINUE");
        loginBtn.setTextColor(0xFFFFFFFF);
        loginBtn.setTextSize(13);
        loginBtn.setTypeface(Typeface.DEFAULT_BOLD);
        loginBtn.setBackground(ripple);
        loginBtn.setMinHeight(0);
        loginBtn.setMinimumHeight(0);
        loginBtn.setPadding(0, dp(12), 0, dp(12));
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            loginBtn.setLetterSpacing(0.03f);
        }
        loginBtn.setAllCaps(false);
        LinearLayout.LayoutParams btnParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        btnParams.setMargins(0, dp(14), 0, 0);
        loginBtn.setLayoutParams(btnParams);
        loginBtn.setOnClickListener(v -> handleLogin());
        loginBtn.setAlpha(0f);
        loginBtn.setTranslationY(dp(20));
        p2.addView(loginBtn);


        p2.setAlpha(0f);
        p2.setTranslationY(dp(80));
        contentContainer.addView(p2);

        scrollView.addView(contentContainer);
        root.addView(scrollView);

        // Autofill credentials only if user previously logged in successfully
        String savedUser = prefs.getString(USERNAME_KEY, "");
        String savedPass = prefs.getString(PASSWORD_KEY, "");
        if ("admin".equalsIgnoreCase(savedUser) && "admin".equalsIgnoreCase(savedPass)) {
            // Discard old default bypass if present in prefs
            prefs.edit().remove(USERNAME_KEY).remove(PASSWORD_KEY).apply();
            savedUser = "";
            savedPass = "";
        }
        if (savedUser != null && !savedUser.isEmpty()) {
            usernameInput.setText(savedUser);
        }
        if (savedPass != null && !savedPass.isEmpty()) {
            passwordInput.setText(savedPass);
        }

        // Set View to Activity or WindowManager
        if (context instanceof Activity) {
            ((Activity) context).setContentView(root);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                Window window = ((Activity) context).getWindow();
                window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
                window.setStatusBarColor(0xFF050505);
            }
        } else {
            WindowManager wm = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
            WindowManager.LayoutParams params = new WindowManager.LayoutParams(
                    WindowManager.LayoutParams.MATCH_PARENT,
                    WindowManager.LayoutParams.MATCH_PARENT,
                    Build.VERSION.SDK_INT >= Build.VERSION_CODES.O ? WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY : WindowManager.LayoutParams.TYPE_PHONE,
                    WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
                    android.graphics.PixelFormat.TRANSLUCENT
            );
            wm.addView(root, params);
        }

        // Entrance animation: p1 + p2 slide up, then staggered fields
        mainHandler.postDelayed(() ->
                playEntranceAnimation(p1, p2, title, usernameInput, passwordInput, loginBtn),
                80);
    }

    private void styleInput(EditText input, boolean isFirst) {
        input.setTextColor(0xFFFFFFFF);
        input.setHintTextColor(0xFF888888);
        input.setTextSize(13);
        input.setPadding(dp(14), dp(12), dp(14), dp(12));

        GradientDrawable bg = new GradientDrawable();
        bg.setCornerRadius(dp(8));
        bg.setColor(0xFF1A1A1A);
        bg.setStroke(dp(1), CardBorderColor);

        input.setBackground(bg);

        input.setOnFocusChangeListener((v, hasFocus) -> {
            if (hasFocus) {
                bg.setStroke(dp(2), PrimaryColor);
            } else {
                bg.setStroke(dp(1), CardBorderColor);
            }
        });

        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        params.setMargins(0, isFirst ? dp(0) : dp(10), 0, 0);
        input.setLayoutParams(params);
    }

    private void playEntranceAnimation(View p1, View p2, View title, View uInput, View pInput, View continueBtn) {
        int cardDuration = 280;
        int cardStagger = 120;
        if (p1 != null) {
            p1.animate()
                    .translationY(0f)
                    .alpha(1f)
                    .setDuration(cardDuration)
                    .setInterpolator(new DecelerateInterpolator(1.6f))
                    .start();
        }
        p2.animate()
                .translationY(0f)
                .alpha(1f)
                .setDuration(cardDuration)
                .setStartDelay(cardStagger)
                .setInterpolator(new OvershootInterpolator(0.75f))
                .withEndAction(() -> animateLoginContents(title, uInput, pInput, continueBtn))
                .start();
    }

    private void animateLoginContents(View title, View uInput, View pInput, View continueBtn) {
        int itemDuration = 220;
        int stagger = 40;
        DecelerateInterpolator decel = new DecelerateInterpolator(1.2f);
        if (title != null) title.animate().translationY(0f).alpha(1f).setDuration(itemDuration).setStartDelay(0).setInterpolator(decel).start();
        if (uInput != null) uInput.animate().translationY(0f).alpha(1f).setDuration(itemDuration).setStartDelay(stagger).setInterpolator(decel).start();
        if (pInput != null) pInput.animate().translationY(0f).alpha(1f).setDuration(itemDuration).setStartDelay(stagger * 2).setInterpolator(decel).start();
        if (continueBtn != null) continueBtn.animate().translationY(0f).alpha(1f).setDuration(itemDuration).setStartDelay(stagger * 3).setInterpolator(new OvershootInterpolator(0.8f)).start();
    }


    private void handleLogin() {
        final String user = usernameInput.getText().toString().trim();
        final String pass = passwordInput.getText().toString().trim();

        if (user.isEmpty()) {
            Toast.makeText(context, "Please enter username or license key", Toast.LENGTH_SHORT).show();
            return;
        }

        startLoadingAnimation();

        ExecutorService executor = Executors.newSingleThreadExecutor();
        executor.execute(() -> {
            try {
                long startTime = System.currentTimeMillis();

                CryptonAuth.init();
                String sessionToken = CryptonAuth.authenticate(context, user, pass);
                NativeGuard.setSessionToken(sessionToken);

                long elapsedTime = System.currentTimeMillis() - startTime;
                if (elapsedTime < 1500) {
                    Thread.sleep(1500 - elapsedTime);
                }

                mainHandler.post(() -> {
                    stopLoadingAnimation(true);
                    handleLoginSuccess(user, pass);
                });
            } catch (Exception e) {
                try { Thread.sleep(1200); } catch (InterruptedException ignored) {}
                mainHandler.post(() -> {
                    stopLoadingAnimation(false);
                    String errorMsg = e.getMessage() != null ? e.getMessage() : "Invalid credentials";
                    Toast.makeText(context, "Login Failed: " + errorMsg, Toast.LENGTH_LONG).show();
                });
            }
        });
    }

    private void startLoadingAnimation() {
        loginBtn.setEnabled(false);
        loginBtn.setText("VERIFYING...");

        colorAnimator = ValueAnimator.ofArgb(PrimaryColor, AccentColor);
        colorAnimator.setDuration(500);
        colorAnimator.setRepeatCount(ValueAnimator.INFINITE);
        colorAnimator.setRepeatMode(ValueAnimator.REVERSE);
        colorAnimator.addUpdateListener(animator -> {
            int color = (int) animator.getAnimatedValue();
            btnBg.setColor(color);
        });
        colorAnimator.start();
    }

    private void stopLoadingAnimation(boolean success) {
        if (colorAnimator != null) colorAnimator.cancel();
        loginBtn.setEnabled(true);
        btnBg.setColor(PrimaryColor);
        loginBtn.setText(success ? "SUCCESS" : "CONTINUE");

        if (!success) {
            new Handler(Looper.getMainLooper()).postDelayed(() -> {
                btnBg.setColor(PrimaryColor);
                loginBtn.setText("CONTINUE");
            }, 1500);
        }
    }

    private void handleLoginSuccess(final String username, final String password) {
        AuthGate.markAuthenticated(username);
        prefs.edit().putString(USERNAME_KEY, username).putString(PASSWORD_KEY, password).apply();

        if (selectedInjectType == 1) {
            toggleSu(false);
        }

        new Handler(Looper.getMainLooper()).postDelayed(() -> {
            try {
                new Menu(context, selectedInjectType, selectedTargetPackage);
                launchGame();
            } catch (Throwable t) {
                android.util.Log.e("Login", "Error starting Menu: " + t.getMessage(), t);
                Toast.makeText(context, "Menu Error: " + t.getMessage(), Toast.LENGTH_LONG).show();
            }
        }, 600);
    }

    private void toggleSu(boolean enable) {
        String from = enable ? "/system/xbin/su" : "/system/xbin/su1";
        String to = enable ? "/system/xbin/su1" : "/system/xbin/su";
        try {
            Process process = Runtime.getRuntime().exec(enable ? "su" : "su1");
            process.getOutputStream().write(("mount -o remount,rw /system\n").getBytes());
            process.getOutputStream().write(("mv " + from + " " + to + "\n").getBytes());
            process.getOutputStream().write("exit\n".getBytes());
            process.getOutputStream().flush();
            process.waitFor();
        } catch (Exception ignored) {}
    }

    private void launchGame() {
        try {
            PackageManager pm = context.getPackageManager();
            Intent intent = pm.getLaunchIntentForPackage(selectedTargetPackage);
            if (intent == null) {
                intent = pm.getLaunchIntentForPackage("com.dts.freefireth");
            }
            if (intent != null) {
                intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                context.startActivity(intent);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
