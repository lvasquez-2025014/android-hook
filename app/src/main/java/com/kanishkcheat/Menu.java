package com.kanishkcheat;

import android.animation.ValueAnimator;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.ColorStateList;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Matrix;
import android.graphics.PixelFormat;
import android.graphics.Shader;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.RippleDrawable;
import android.os.Build;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;


import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Menu {

    // Native Functions
    public static native void Functions();
    public static native void ChangesID(int ID, int Value);
    public static native void Init(Context context);
    public static native void OnDrawLoad(DrawView drawView, Canvas canvas);

    public static String target = "com.dts.freefireth";
    private int injectType;

    // Theme - match FF_COVERV7A (HXC inject UI: dark + deep red)
    private int buttonClick = 0;
    public static int PrimaryColor = 0xFFC62828;
    public static int TabSelectedColor = 0xFFC62828;
    private static final int AccentBorderColor = 0xFFE53935;
    private static final int MenuBgColor = 0xE0121212;
    private static final int MenuBorderColor = 0xFF2A2A2A;
    private static final int CardBgColor = 0xE01A1A1A;
    private static final int CardBorderColor = 0xFF383838;
    private static final int InactiveTabBgColor = 0xFF1A1A1A;
    private static final int InactiveTabBorderColor = 0xFF404040;
    private static final int InactiveTabTextColor = 0xFFB0B0B0;
    private static final int KeybindBtnBgColor = 0xFF2A2A2A;
    private static final int KeybindBtnBorderColor = 0xFF505050;
    private static final int TextPrimaryColor = 0xFFFFFFFF;
    private static final int TextSecondaryColor = 0xFFB0B0B0;

    private static Context context;
    private static Utils utils;
    private static Menu menuInstance;

    // Window System
    private WindowManager windowManager;
    private WindowManager.LayoutParams windowManagerParams;
    private FrameLayout frameLayout;

    // DrawView
    DrawView drawView;
    WindowManager.LayoutParams windowManagerDrawViewParams;

    // UI Containers
    private static ScrollView scrollView_center;
    private static LinearLayout featuresScrollContainer;
    private static LinearLayout container_menu;
    private static ImageView icon_cheat;
    private static Button inject_close;

    // Keybind management
    private static Map<Integer, Integer> idToKeyCode = new HashMap<>();
    private static Map<Integer, TextView> idToKeybindTv = new HashMap<>();
    private static Map<Integer, CheckBox> idToBindSwitch = new HashMap<>();
    private static List<TextView> allKeybindButtons = new ArrayList<>();

    private static final String KEYBIND_PREFS = "keybinds";
    private static final String PREF_KEYBIND_ENABLED = "keybind_enabled";
    private static volatile boolean keybindDialogShowing = false;
    private static volatile Integer pendingKeybindId = null;

    private static final int ID_TURN_ON_KEYBIND = 6000;
    private static final int ID_CLEAR_KEYBINDS = 6001;
    private static volatile boolean keybindEnabled = false;

    // Floating controls
    private static final String FLOAT_PREFS = "TX999_Floats";
    private static Map<Integer, CheckBox> floatCheckboxByFeatureId = new HashMap<>();
    private static List<LinearLayout> floatingButtons = new ArrayList<>();
    private static boolean isLinkingHacks = false;

    // Keybind overlay

    private static SharedPreferences getKeybindPrefs() {
        return context.getSharedPreferences(KEYBIND_PREFS, Context.MODE_PRIVATE);
    }

    private static int getKeybindForId(int id) {
        return getKeybindPrefs().getInt("keybind_" + id, -1);
    }

    private static String keyCodeToLabel(int keyCode) {
        String s = KeyEvent.keyCodeToString(keyCode);
        if (s != null && s.startsWith("KEYCODE_")) s = s.substring(7);
        else if (s == null) s = String.valueOf(keyCode);
        if (s != null) while (s.startsWith("_")) s = s.substring(1);
        return s != null ? s : String.valueOf(keyCode);
    }

    private static void setKeybindForId(int id, int keyCode) {
        getKeybindPrefs().edit().putInt("keybind_" + id, keyCode).apply();
        idToKeyCode.put(id, keyCode);
        for (Map.Entry<Integer, Integer> e : new HashMap<>(idToKeyCode).entrySet()) {
            if (e.getValue() == keyCode && e.getKey() != id) {
                idToKeyCode.remove(e.getKey());
                TextView other = idToKeybindTv.get(e.getKey());
                if (other != null) other.setText("None");
                break;
            }
        }
        TextView tv = idToKeybindTv.get(id);
        if (tv != null) tv.setText(keyCodeToLabel(keyCode));
    }

    private static void clearAllKeybinds() {
        SharedPreferences.Editor editor = getKeybindPrefs().edit();
        for (String key : getKeybindPrefs().getAll().keySet()) {
            editor.remove(key);
        }
        editor.apply();
        idToKeyCode.clear();
        for (Map.Entry<Integer, TextView> e : idToKeybindTv.entrySet()) {
            TextView tv = e.getValue();
            if (tv != null) tv.setText("None");
        }
    }

    private static SharedPreferences getFloatPrefs() {
        return context.getSharedPreferences(FLOAT_PREFS, Context.MODE_PRIVATE);
    }

    private static boolean floatShown(int id) {
        return getFloatPrefs().getBoolean("float_shown_" + id, false);
    }

    private static boolean floatInnerOn(int id) {
        return getFloatPrefs().getBoolean("float_inner_" + id, false);
    }

    private static int floatX(int id) {
        return getFloatPrefs().getInt("float_x_" + id, -1);
    }

    private static int floatY(int id) {
        return getFloatPrefs().getInt("float_y_" + id, -1);
    }

    private static void saveFloatShown(int id, boolean shown) {
        getFloatPrefs().edit().putBoolean("float_shown_" + id, shown).apply();
    }

    private static void saveFloatInner(int id, boolean on) {
        getFloatPrefs().edit().putBoolean("float_inner_" + id, on).apply();
    }

    private static void saveFloatPos(int id, int x, int y) {
        getFloatPrefs().edit().putInt("float_x_" + id, x).putInt("float_y_" + id, y).apply();
    }

    private static void restoreSavedFloats() {
        for (Map.Entry<Integer, CheckBox> e : new HashMap<>(floatCheckboxByFeatureId).entrySet()) {
            int featureId = e.getKey();
            if (!floatShown(featureId)) continue;
            CheckBox outer = e.getValue();
            if (outer != null && !outer.isChecked()) {
                outer.setChecked(true);
            }
        }
    }

    public void DrawCanvas() {
        int LAYOUT_FLAG;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
        } else {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_PHONE;
        }

        drawView = new DrawView(context);
        windowManagerDrawViewParams = new WindowManager.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT,
                LAYOUT_FLAG,
                WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL |
                        WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE |
                        WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                        WindowManager.LayoutParams.FLAG_FULLSCREEN |
                        WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED,
                PixelFormat.TRANSPARENT);
        windowManagerDrawViewParams.gravity = Gravity.CENTER;
        windowManager.addView(drawView, windowManagerDrawViewParams);
    }

    public Menu(Context globContext, int glob_injectType) {
        this(globContext, glob_injectType, "com.dts.freefireth");
    }

    public Menu(Context globContext, int glob_injectType, String glob_target) {
        if (!AuthGate.isAuthenticated()) {
            android.util.Log.e("Menu", "Blocked: no valid authenticated session, login required");
            return;
        }
        context = globContext;
        utils = new Utils(context);
        injectType = glob_injectType;
        target = glob_target;
        menuInstance = this;
        System.loadLibrary("amithnew");
        onCreate();
    }

    public static void destroyInstance() {
        final Menu m = menuInstance;
        menuInstance = null;
        if (m == null) return;
        new android.os.Handler(android.os.Looper.getMainLooper()).post(() -> {
            for (LinearLayout btn : new ArrayList<>(floatingButtons)) {
                try { if (m.windowManager != null) m.windowManager.removeView(btn); } catch (Throwable ignored) {}
            }
            floatingButtons.clear();
            WindowManager wm = m.windowManager;
            if (wm != null) {
                try { if (m.drawView != null) wm.removeView(m.drawView); } catch (Throwable ignored) {}
                try { if (m.frameLayout != null) wm.removeView(m.frameLayout); } catch (Throwable ignored) {}
            }
        });
    }

    public void onCreate() {
        onCreateSystemWindow();
        onCreateTemplate();
    }

    // Criar Template - dimensions & styles matching FF_COVERV7A exactly
    public void onCreateTemplate() {
        int menuWidth = utils.FixDP(195);

        GradientDrawable gradientDrawable_container = new GradientDrawable();
        gradientDrawable_container.setColor(MenuBgColor);
        gradientDrawable_container.setCornerRadius(utils.FixDP(1));
        gradientDrawable_container.setStroke(utils.FixDP(1), MenuBorderColor);

        LinearLayout container = new LinearLayout(context);
        container.setOrientation(LinearLayout.VERTICAL);
        container.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
        ));

        container_menu = new LinearLayout(context);
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(menuWidth, ViewGroup.LayoutParams.WRAP_CONTENT);
        container_menu.setLayoutParams(params);
        container_menu.setOrientation(LinearLayout.VERTICAL);
        container_menu.setBackgroundColor(MenuBgColor);
        container_menu.setVisibility(View.GONE);
        container_menu.setBackground(gradientDrawable_container);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            container_menu.setElevation(utils.FixDP(4));
        }

        // Floating icon
        icon_cheat = new ImageView(context);
        icon_cheat.setLayoutParams(new LinearLayout.LayoutParams(
                utils.FixDP(40),
                utils.FixDP(40)
        ));
        icon_cheat.setImageResource(R.drawable.seu_logo);
        icon_cheat.setScaleType(ImageView.ScaleType.CENTER_CROP);
        GradientDrawable iconBackground = new GradientDrawable();
        iconBackground.setShape(GradientDrawable.OVAL);
        iconBackground.setColor(Color.TRANSPARENT);
        icon_cheat.setBackground(iconBackground);
        icon_cheat.setPadding(utils.FixDP(4), utils.FixDP(4), utils.FixDP(4), utils.FixDP(4));
        icon_cheat.setOnTouchListener(onTouchListener());
        icon_cheat.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                icon_cheat.setVisibility(View.GONE);
                container_menu.setVisibility(View.VISIBLE);
                if (menuInstance != null) {
                    if (keybindEnabled) {
                        menuInstance.enableHotkeys();
                        menuInstance.requestKeybindFocus();
                    } else {
                        menuInstance.disableHotkeys();
                    }
                }
            }
        });

        // Header - 32dp height with animated sweeping gradient title (tap to close)
        LinearLayout container_top = new LinearLayout(context);
        container_top.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                utils.FixDP(32)
        ));
        container_top.setPadding(utils.FixDP(8), utils.FixDP(6), utils.FixDP(6), utils.FixDP(6));
        container_top.setGravity(Gravity.CENTER_VERTICAL);
        container_top.setOrientation(LinearLayout.HORIZONTAL);

        GradientDrawable topBg = new GradientDrawable();
        topBg.setColor(0x0D000000);
        topBg.setCornerRadii(new float[]{utils.FixDP(1), utils.FixDP(1), utils.FixDP(1), utils.FixDP(1), 0, 0, 0, 0});
        container_top.setBackground(topBg);

        FrameLayout headerFrame = new FrameLayout(context);
        headerFrame.setLayoutParams(new LinearLayout.LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT, 1f));

        final TextView titleMain = new TextView(context);
        titleMain.setText("JN X!T");
        titleMain.setTextColor(PrimaryColor);
        titleMain.setTextSize(11);
        titleMain.setTypeface(Typeface.DEFAULT_BOLD);
        titleMain.setLayerType(View.LAYER_TYPE_SOFTWARE, null);
        titleMain.setShadowLayer((float) utils.FixDP(6), 0, 0, 0xDDC62828);
        FrameLayout.LayoutParams titleLp = new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT,
                Gravity.CENTER
        );
        titleMain.setLayoutParams(titleLp);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) titleMain.setLetterSpacing(0.04f);
        titleMain.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                if (titleMain.getWidth() > 0) {
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
                        titleMain.getViewTreeObserver().removeOnGlobalLayoutListener(this);
                    } else {
                        titleMain.getViewTreeObserver().removeGlobalOnLayoutListener(this);
                    }
                    int[] colors = { PrimaryColor, 0xFFFFFFFF, PrimaryColor };
                    float[] positions = { 0f, 0.5f, 1f };
                    final LinearGradient shader = new LinearGradient(
                            0, 0, titleMain.getWidth(), 0,
                            colors, positions,
                            Shader.TileMode.CLAMP
                    );
                    titleMain.getPaint().setShader(shader);
                    final Matrix matrix = new Matrix();
                    ValueAnimator animator = ValueAnimator.ofFloat(0, 1);
                    animator.setDuration(2500);
                    animator.setRepeatCount(ValueAnimator.INFINITE);
                    animator.addUpdateListener(animation -> {
                        float v = (float) animation.getAnimatedValue();
                        float translate = (titleMain.getWidth() * 3) * v - titleMain.getWidth();
                        matrix.setTranslate(translate, 0);
                        shader.setLocalMatrix(matrix);
                        titleMain.invalidate();
                    });
                    animator.start();
                }
            }
        });

        View.OnClickListener closeMenuClick = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                icon_cheat.setVisibility(View.VISIBLE);
                container_menu.setVisibility(View.GONE);
                if (menuInstance != null && !keybindEnabled) menuInstance.disableHotkeys();
            }
        };
        headerFrame.setOnClickListener(closeMenuClick);
        titleMain.setOnClickListener(closeMenuClick);

        headerFrame.addView(titleMain);
        container_top.addView(headerFrame);

        // Center section - fixed 200dp height with single scrollable feature container
        final LinearLayout container_center = new LinearLayout(context);
        LinearLayout.LayoutParams centerParams = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                utils.FixDP(200)
        );
        container_center.setLayoutParams(centerParams);
        container_center.setMinimumHeight(utils.FixDP(200));
        container_center.setGravity(Gravity.CENTER);
        container_center.setOrientation(LinearLayout.VERTICAL);

        scrollView_center = new ScrollView(context);
        scrollView_center.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT
        ));
        scrollView_center.setPadding(0, 0, 0, 0);
        scrollView_center.setVerticalScrollBarEnabled(false);

        featuresScrollContainer = new LinearLayout(context);
        featuresScrollContainer.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        featuresScrollContainer.setOrientation(LinearLayout.VERTICAL);

        scrollView_center.addView(featuresScrollContainer);

        // Centered Circle Loader
        final CircleLoadingView progressBar = new CircleLoadingView(context);
        FrameLayout.LayoutParams progressLp = new FrameLayout.LayoutParams(
                utils.FixDP(42),
                utils.FixDP(42)
        );
        progressLp.gravity = Gravity.CENTER;
        progressBar.setLayoutParams(progressLp);
        progressBar.setPrimaryColor(PrimaryColor);

        FrameLayout loadingContainer = new FrameLayout(context);
        loadingContainer.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT
        ));
        loadingContainer.addView(progressBar);

        // Bottom section - single full-width INJECT / CLOSE button
        LinearLayout container_bottom = new LinearLayout(context);
        LinearLayout.LayoutParams bottomLp = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
        );
        bottomLp.bottomMargin = 0;
        container_bottom.setLayoutParams(bottomLp);
        container_bottom.setMinimumHeight(utils.FixDP(44));
        container_bottom.setPadding(
                utils.FixDP(6),
                utils.FixDP(4),
                utils.FixDP(6),
                utils.FixDP(4)
        );
        container_bottom.setOrientation(LinearLayout.VERTICAL);
        container_bottom.setGravity(Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL);

        GradientDrawable gradientDrawable_inject_close = new GradientDrawable();
        gradientDrawable_inject_close.setColor(PrimaryColor);
        gradientDrawable_inject_close.setCornerRadius(utils.FixDP(4));
        gradientDrawable_inject_close.setStroke(utils.FixDP(1), AccentBorderColor);
        RippleDrawable rippleDrawable = new RippleDrawable(
                ColorStateList.valueOf(0x40FFFFFF),
                gradientDrawable_inject_close,
                null
        );

        inject_close = new Button(context);
        LinearLayout.LayoutParams injectCloseLp = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                utils.FixDP(28)
        );
        injectCloseLp.gravity = Gravity.CENTER_HORIZONTAL;
        inject_close.setLayoutParams(injectCloseLp);
        inject_close.setPadding(utils.FixDP(8), utils.FixDP(6), utils.FixDP(8), utils.FixDP(6));
        inject_close.setText("INJECT");
        inject_close.setTextSize(9);
        inject_close.setTypeface(Typeface.DEFAULT_BOLD);
        inject_close.setTextColor(0xFFFFFFFF);
        inject_close.setBackground(rippleDrawable);
        inject_close.setVisibility(View.VISIBLE);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            inject_close.setLetterSpacing(0.04f);
            inject_close.setElevation(utils.FixDP(4));
        }

        inject_close.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (buttonClick == 0) {
                    Toast.makeText(context, "Processing injection...", Toast.LENGTH_SHORT).show();

                    boolean success = false;
                    if (injectType == 0) {
                        success = InjectX32("libamithcore.so");
                    } else {
                        success = InjectX86("libamithcore.so");
                    }

                    if (success) {
                        Init(context);
                        Functions();

                        new android.os.Handler(android.os.Looper.getMainLooper()).postDelayed(new Runnable() {
                            @Override
                            public void run() {
                                progressBar.setVisibility(View.GONE);
                                inject_close.setText("CLOSE");
                                container_center.removeAllViews();
                                container_center.addView(scrollView_center);
                                buttonClick = 1;

                                restoreSavedFloats();

                                if (menuInstance != null && keybindEnabled) {
                                    menuInstance.enableHotkeys();
                                }
                                toggleSu(true);
                                Toast.makeText(context, "Injection successful!", Toast.LENGTH_SHORT).show();
                            }
                        }, 100);
                    } else {
                        Toast.makeText(context, "Injection failed! Please try again.", Toast.LENGTH_LONG).show();
                    }
                } else if (buttonClick == 1) {
                    icon_cheat.setVisibility(View.VISIBLE);
                    container_menu.setVisibility(View.GONE);
                    if (menuInstance != null && !keybindEnabled) menuInstance.disableHotkeys();
                }
            }
        });

        // Add views into container
        frameLayout.addView(container);
        container.addView(icon_cheat);
        container.addView(container_menu);

        container_menu.addView(container_top);
        container_menu.addView(container_center);
        container_center.addView(loadingContainer);

        container_menu.addView(container_bottom);
        container_bottom.addView(inject_close);
    }

    public void onCreateSystemWindow() {
        int LAYOUT_FLAG;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
        } else {
            LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_PHONE;
        }

        frameLayout = new FrameLayout(context) {
            @Override
            public boolean dispatchKeyEvent(KeyEvent event) {
                if (handleGlobalKeyEvent(event)) {
                    return true;
                }
                return super.dispatchKeyEvent(event);
            }
        };
        frameLayout.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));
        frameLayout.setOnTouchListener(onTouchListener());
        frameLayout.setAlpha(0.95f);

        windowManagerParams = new WindowManager.LayoutParams(
                WindowManager.LayoutParams.WRAP_CONTENT,
                WindowManager.LayoutParams.WRAP_CONTENT,
                LAYOUT_FLAG,
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL
                        | WindowManager.LayoutParams.FLAG_LAYOUT_IN_OVERSCAN
                        | WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN
                        | WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM
                        | WindowManager.LayoutParams.FLAG_SPLIT_TOUCH | WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED,
                PixelFormat.TRANSPARENT);
        windowManagerParams.gravity = Gravity.TOP | Gravity.LEFT;
        windowManagerParams.x = 50;
        windowManagerParams.y = 100;

        windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        DrawCanvas();
        windowManager.addView(frameLayout, windowManagerParams);
    }

    private void enableHotkeys() {
        if (windowManagerParams == null || frameLayout == null) return;
        windowManagerParams.flags &= ~WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        windowManager.updateViewLayout(frameLayout, windowManagerParams);
        frameLayout.setFocusable(true);
        frameLayout.setFocusableInTouchMode(true);
        frameLayout.requestFocus();
        frameLayout.post(new Runnable() {
            @Override
            public void run() {
                if (frameLayout != null) frameLayout.requestFocus();
            }
        });
    }

    private void disableHotkeys() {
        if (windowManagerParams == null || frameLayout == null) return;
        frameLayout.setFocusable(false);
        frameLayout.setFocusableInTouchMode(false);
        windowManagerParams.flags |= WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        windowManager.updateViewLayout(frameLayout, windowManagerParams);
    }

    public void requestKeybindFocus() {
        if (frameLayout == null) return;
        frameLayout.requestFocus();
        frameLayout.post(new Runnable() {
            @Override
            public void run() {
                if (frameLayout != null) frameLayout.requestFocus();
            }
        });
    }

    private boolean handleGlobalKeyEvent(KeyEvent event) {
        if (event.getAction() != KeyEvent.ACTION_DOWN) return false;
        int keyCode = event.getKeyCode();

        if (keybindDialogShowing && pendingKeybindId != null) {
            if (keyCode == KeyEvent.KEYCODE_BACK) {
                int cancelledId = pendingKeybindId;
                pendingKeybindId = null;
                keybindDialogShowing = false;
                TextView tv = idToKeybindTv.get(cancelledId);
                if (tv != null) {
                    int k = getKeybindForId(cancelledId);
                    tv.setText(k != -1 ? keyCodeToLabel(k) : "None");
                }
                Toast.makeText(context, "Key selection cancelled", Toast.LENGTH_SHORT).show();
                return true;
            }
            int targetId = pendingKeybindId;
            pendingKeybindId = null;
            keybindDialogShowing = false;
            setKeybindForId(targetId, keyCode);
            Toast.makeText(context, "Key set: " + keyCodeToLabel(keyCode), Toast.LENGTH_SHORT).show();
            return true;
        }

        if (keybindDialogShowing) return false;
        if (!keybindEnabled) return false;

        Map<Integer, Integer> keyCodeToId = new HashMap<>();
        for (Map.Entry<Integer, Integer> e : idToKeyCode.entrySet())
            keyCodeToId.put(e.getValue(), e.getKey());

        Integer id = keyCodeToId.get(keyCode);
        if (id == null) return false;

        CheckBox sw = idToBindSwitch.get(id);
        if (sw != null) {
            sw.setChecked(!sw.isChecked());
        }
        return true;
    }

    private View.OnTouchListener onTouchListener() {
        return new View.OnTouchListener() {
            private static final int TOUCH_MOVE_THRESHOLD = 8;
            private int x, y;
            private int initialX, initialY;
            private boolean isMoving = false;

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        x = (int) event.getRawX();
                        y = (int) event.getRawY();
                        initialX = x;
                        initialY = y;
                        isMoving = false;
                        frameLayout.setAlpha(0.8f);
                        return true;

                    case MotionEvent.ACTION_MOVE:
                        int nowX = (int) event.getRawX();
                        int nowY = (int) event.getRawY();

                        int totalMoveX = Math.abs(nowX - initialX);
                        int totalMoveY = Math.abs(nowY - initialY);

                        if (!isMoving && (totalMoveX > TOUCH_MOVE_THRESHOLD || totalMoveY > TOUCH_MOVE_THRESHOLD)) {
                            isMoving = true;
                        }

                        if (isMoving) {
                            int movedX = nowX - x;
                            int movedY = nowY - y;
                            x = nowX;
                            y = nowY;
                            windowManagerParams.x = windowManagerParams.x + movedX;
                            windowManagerParams.y = windowManagerParams.y + movedY;
                            windowManager.updateViewLayout(frameLayout, windowManagerParams);
                        }
                        return true;

                    case MotionEvent.ACTION_UP:
                        if (!isMoving) {
                            v.performClick();
                        }
                        frameLayout.setAlpha(0.95f);
                        return true;

                    default:
                        break;
                }
                return false;
            }
        };
    }

    // ==================== SINGLE CONTINUOUS FEATURE LIST METHODS ====================

    /**
     * In reference project (FF_COVERV7A), there are NO sidebar tab buttons.
     * All tabs and features are arranged directly in one continuous line.
     */
    public static void addTab(final String name) {
        if (name == null || name.isEmpty()) return;
        // Don't show "LOGO" as a separate category header since "FLOATING CONTROLS" is right under it
        if (name.equalsIgnoreCase("LOGO")) return;
        addCategory(name);
    }

    /**
     * Category banner badge - compact Red pill header matching reference project
     */
    public static void addCategory(String name) {
        if (featuresScrollContainer == null) return;

        GradientDrawable gradientDrawable = new GradientDrawable();
        gradientDrawable.setColor(PrimaryColor);
        gradientDrawable.setCornerRadius(utils.FixDP(5));
        gradientDrawable.setStroke(utils.FixDP(1), AccentBorderColor);

        LinearLayout linearLayout = new LinearLayout(context);
        linearLayout.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                utils.FixDP(18)
        ));
        linearLayout.setBackground(gradientDrawable);
        linearLayout.setGravity(Gravity.CENTER);
        linearLayout.setPadding(utils.FixDP(8), utils.FixDP(2), utils.FixDP(8), utils.FixDP(2));

        LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) linearLayout.getLayoutParams();
        params.setMargins(0, utils.FixDP(4), 0, utils.FixDP(2));
        linearLayout.setLayoutParams(params);

        TextView textView = new TextView(context);
        textView.setText(name);
        textView.setTextSize(8);
        textView.setTypeface(Typeface.DEFAULT_BOLD);
        textView.setTextColor(0xFFFFFFFF);
        textView.setGravity(Gravity.CENTER);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            textView.setLetterSpacing(0.02f);
        }

        linearLayout.addView(textView);
        featuresScrollContainer.addView(linearLayout);
    }

    /**
     * Switch feature card row - Card background with label (left) and CheckBox (right)
     */
    public static void addSwitch(String name, final int ID) {
        if (featuresScrollContainer == null) return;

        // Card background - HXC-style row
        GradientDrawable cardBg = new GradientDrawable();
        cardBg.setColor(CardBgColor);
        cardBg.setCornerRadius(utils.FixDP(4));
        cardBg.setStroke(utils.FixDP(1), CardBorderColor);

        LinearLayout card = new LinearLayout(context);
        card.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        LinearLayout.LayoutParams cardParams = (LinearLayout.LayoutParams) card.getLayoutParams();
        cardParams.setMargins(0, utils.FixDP(2), 0, 0);
        card.setLayoutParams(cardParams);
        card.setBackground(cardBg);
        card.setPadding(utils.FixDP(6), utils.FixDP(4), utils.FixDP(6), utils.FixDP(4));
        card.setOrientation(LinearLayout.HORIZONTAL);
        card.setGravity(Gravity.CENTER_VERTICAL);

        LinearLayout linearLayout = new LinearLayout(context);
        linearLayout.setLayoutParams(new LinearLayout.LayoutParams(
                0,
                ViewGroup.LayoutParams.WRAP_CONTENT,
                1
        ));
        linearLayout.setPadding(0, 0, 0, 0);
        linearLayout.setOrientation(LinearLayout.HORIZONTAL);
        linearLayout.setGravity(Gravity.CENTER_VERTICAL);

        final TextView textView = new TextView(context);
        textView.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        textView.setGravity(Gravity.CENTER_VERTICAL);
        textView.setText(name);
        textView.setTextColor(0xFFFFFFFF);
        textView.setTextSize(9);
        textView.setTypeface(Typeface.DEFAULT_BOLD);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            textView.setLetterSpacing(0.02f);
        }

        linearLayout.addView(textView);

        // Keybind button (compact HXC style)
        final TextView keybindTv = new TextView(context);
        LinearLayout.LayoutParams keybindLp = new LinearLayout.LayoutParams(
                utils.FixDP(48),
                utils.FixDP(18)
        );
        keybindLp.setMargins(utils.FixDP(3), 0, utils.FixDP(3), 0);
        keybindTv.setLayoutParams(keybindLp);
        keybindTv.setGravity(Gravity.CENTER);
        keybindTv.setTextSize(9);
        keybindTv.setTypeface(Typeface.DEFAULT_BOLD);
        keybindTv.setTextColor(0xFFE0E0E0);
        GradientDrawable keybindBtnBg = new GradientDrawable();
        keybindBtnBg.setCornerRadius(utils.FixDP(3));
        keybindBtnBg.setColor(KeybindBtnBgColor);
        keybindBtnBg.setStroke(utils.FixDP(1), KeybindBtnBorderColor);
        keybindTv.setBackground(keybindBtnBg);

        int savedKey = getKeybindForId(ID);
        if (savedKey != -1) {
            idToKeyCode.put(ID, savedKey);
            keybindTv.setText(keyCodeToLabel(savedKey));
        } else {
            keybindTv.setText("None");
        }
        idToKeybindTv.put(ID, keybindTv);
        keybindTv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int currentKey = getKeybindForId(ID);
                if (!keybindDialogShowing && currentKey != -1) {
                    setKeybindForId(ID, -1);
                    Toast.makeText(context, "Keybind cleared", Toast.LENGTH_SHORT).show();
                    return;
                }
                keybindTv.setText("Press");
                pendingKeybindId = ID;
                keybindDialogShowing = true;
                if (menuInstance != null) {
                    menuInstance.enableHotkeys();
                    menuInstance.requestKeybindFocus();
                }
                new android.os.Handler(android.os.Looper.getMainLooper()).postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        if (pendingKeybindId != null && pendingKeybindId == ID) {
                            pendingKeybindId = null;
                            keybindDialogShowing = false;
                            TextView tv = idToKeybindTv.get(ID);
                            if (tv != null) {
                                int k = getKeybindForId(ID);
                                tv.setText(k != -1 ? keyCodeToLabel(k) : "None");
                            }
                            Toast.makeText(context, "Key selection cancelled", Toast.LENGTH_SHORT).show();
                        }
                    }
                }, 10000);
            }
        });

        boolean isSystemControl = (ID == ID_TURN_ON_KEYBIND || ID == ID_CLEAR_KEYBINDS);
        keybindTv.setVisibility(keybindEnabled && !isSystemControl ? View.VISIBLE : View.GONE);
        if (!isSystemControl) allKeybindButtons.add(keybindTv);

        final CheckBox checkBox = new CheckBox(context);
        checkBox.setLayoutParams(new LinearLayout.LayoutParams(
                utils.FixDP(24),
                utils.FixDP(24)
        ));
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            checkBox.setButtonTintList(ColorStateList.valueOf(PrimaryColor));
        }

        idToBindSwitch.put(ID, checkBox);

        checkBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (ID == 102 && !isLinkingHacks) {
                    isLinkingHacks = true;
                    int[] espIds = {1, 2, 3, 9, 16};
                    for (int espId : espIds) {
                        CheckBox espSwitch = idToBindSwitch.get(espId);
                        if (espSwitch != null && espSwitch.isChecked() != isChecked) {
                            espSwitch.setChecked(isChecked);
                            ChangesID(espId, isChecked ? 1 : 0);
                        } else {
                            ChangesID(espId, isChecked ? 1 : 0);
                        }
                    }
                    if (isChecked) {
                        android.util.DisplayMetrics dm = context.getResources().getDisplayMetrics();
                        int maxDim = Math.max(dm.widthPixels, dm.heightPixels);
                        int fullFov = maxDim / 2;
                        if (fullFov < 960) fullFov = 960;
                        ChangesID(104, fullFov);
                    }
                    isLinkingHacks = false;
                }

                if (ID == ID_CLEAR_KEYBINDS && isChecked) {
                    clearAllKeybinds();
                    Toast.makeText(context, "Keybinds cleared", Toast.LENGTH_SHORT).show();
                    new android.os.Handler(android.os.Looper.getMainLooper()).postDelayed(new Runnable() {
                        @Override
                        public void run() { checkBox.setChecked(false); }
                    }, 300);
                    return;
                }
                if (ID == ID_TURN_ON_KEYBIND) {
                    keybindEnabled = isChecked;
                    for (TextView kbv : allKeybindButtons) {
                        kbv.setVisibility(isChecked ? View.VISIBLE : View.GONE);
                    }
                    try {
                        getKeybindPrefs().edit().putBoolean(PREF_KEYBIND_ENABLED, isChecked).apply();
                    } catch (Throwable ignored) {}
                    if (menuInstance != null) {
                        if (isChecked) menuInstance.enableHotkeys();
                        else menuInstance.disableHotkeys();
                    }
                }
                if (!isSystemControl) {
                    ChangesID(ID, isChecked ? 1 : 0);
                }
            }
        });

        if (ID == ID_TURN_ON_KEYBIND) {
            try {
                final boolean savedEnabled = getKeybindPrefs().getBoolean(PREF_KEYBIND_ENABLED, false);
                checkBox.setChecked(savedEnabled);
                keybindEnabled = savedEnabled;
                if (savedEnabled && menuInstance != null) {
                    new android.os.Handler(android.os.Looper.getMainLooper()).post(new Runnable() {
                        @Override
                        public void run() {
                            if (menuInstance != null) menuInstance.enableHotkeys();
                        }
                    });
                }
            } catch (Throwable ignored) { keybindEnabled = false; }
        }
        View.OnClickListener toggleOnClick = new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                checkBox.setChecked(!checkBox.isChecked());
            }
        };
        card.setOnClickListener(toggleOnClick);
        linearLayout.setOnClickListener(toggleOnClick);

        card.addView(linearLayout);
        if (!isSystemControl) {
            card.addView(keybindTv);
        }
        card.addView(checkBox);
        featuresScrollContainer.addView(card);
    }

    /**
     * Floating switch row: opens a draggable floating button on game screen
     */
    public static void addFloatingCheckbox(String name, final int showId, final int featureId) {
        addFloatingCheckbox(name, showId, featureId, name);
    }

    public static void addFloatingCheckbox(String name, final int showId, final int featureId, String floatLabel) {
        if (featuresScrollContainer == null) return;

        GradientDrawable cardBg = new GradientDrawable();
        cardBg.setColor(CardBgColor);
        cardBg.setCornerRadius(utils.FixDP(4));
        cardBg.setStroke(utils.FixDP(1), CardBorderColor);

        LinearLayout card = new LinearLayout(context);
        card.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        LinearLayout.LayoutParams cardParams = (LinearLayout.LayoutParams) card.getLayoutParams();
        cardParams.setMargins(0, utils.FixDP(2), 0, 0);
        card.setLayoutParams(cardParams);
        card.setBackground(cardBg);
        card.setPadding(utils.FixDP(6), utils.FixDP(4), utils.FixDP(6), utils.FixDP(4));
        card.setOrientation(LinearLayout.HORIZONTAL);
        card.setGravity(Gravity.CENTER_VERTICAL);

        LinearLayout linearLayout = new LinearLayout(context);
        linearLayout.setLayoutParams(new LinearLayout.LayoutParams(
                0,
                ViewGroup.LayoutParams.WRAP_CONTENT,
                1
        ));
        linearLayout.setOrientation(LinearLayout.HORIZONTAL);
        linearLayout.setGravity(Gravity.CENTER_VERTICAL);

        final TextView textView = new TextView(context);
        textView.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        textView.setText(name);
        textView.setTextColor(0xFFFFFFFF);
        textView.setTextSize(9);
        textView.setTypeface(Typeface.DEFAULT_BOLD);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            textView.setLetterSpacing(0.02f);
        }

        TextView badge = new TextView(context);
        LinearLayout.LayoutParams badgeLp = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, utils.FixDP(18));
        badgeLp.setMargins(utils.FixDP(4), 0, utils.FixDP(6), 0);
        badge.setLayoutParams(badgeLp);
        badge.setText("\u29C9");
        badge.setTextColor(PrimaryColor);
        badge.setTextSize(10);
        badge.setGravity(Gravity.CENTER);

        linearLayout.addView(textView);
        linearLayout.addView(badge);

        final CheckBox checkBox = new CheckBox(context);
        checkBox.setLayoutParams(new LinearLayout.LayoutParams(
                utils.FixDP(24),
                utils.FixDP(24)
        ));
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            checkBox.setButtonTintList(ColorStateList.valueOf(PrimaryColor));
        }

        checkBox.setOnCheckedChangeListener((cb, isChecked) -> {
            saveFloatShown(featureId, isChecked);
            if (isChecked) {
                createFloatingButton(floatLabel, featureId);
            } else {
                new android.os.Handler(android.os.Looper.getMainLooper()).post(() -> {
                    for (int i = floatingButtons.size() - 1; i >= 0; i--) {
                        LinearLayout btn = floatingButtons.get(i);
                        if (Integer.valueOf(featureId).equals(btn.getTag())) {
                            try {
                                if (menuInstance != null) menuInstance.windowManager.removeView(btn);
                            } catch (Exception ignored) {}
                            floatingButtons.remove(i);
                            ChangesID(featureId, 0);
                            break;
                        }
                    }
                });
            }
        });

        idToBindSwitch.put(showId, checkBox);
        floatCheckboxByFeatureId.put(featureId, checkBox);

        View.OnClickListener toggleClick = v -> checkBox.setChecked(!checkBox.isChecked());
        card.setOnClickListener(toggleClick);
        linearLayout.setOnClickListener(toggleClick);

        card.addView(linearLayout);
        card.addView(checkBox);
        featuresScrollContainer.addView(card);
    }

    private static void createFloatingButton(String floatLabel, int featureId) {
        new android.os.Handler(android.os.Looper.getMainLooper()).post(() -> {
            int LAYOUT_FLAG;
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
            } else {
                LAYOUT_FLAG = WindowManager.LayoutParams.TYPE_PHONE;
            }

            if (menuInstance == null) return;

            final LinearLayout buttonLayout = new LinearLayout(context);
            buttonLayout.setOrientation(LinearLayout.VERTICAL);
            buttonLayout.setTag(featureId);
            buttonLayout.setPadding(utils.FixDP(8), utils.FixDP(6), utils.FixDP(8), utils.FixDP(6));
            buttonLayout.setGravity(Gravity.CENTER_HORIZONTAL);
            buttonLayout.setMinimumWidth(utils.FixDP(80));

            GradientDrawable floatBg = new GradientDrawable();
            floatBg.setColor(MenuBgColor);
            floatBg.setCornerRadius(utils.FixDP(6));
            floatBg.setStroke(utils.FixDP(1), PrimaryColor);
            buttonLayout.setBackground(floatBg);

            TextView label = new TextView(context);
            label.setText(floatLabel.toUpperCase());
            label.setTextColor(Color.WHITE);
            label.setTextSize(9.5f);
            label.setGravity(Gravity.CENTER);
            label.setSingleLine(false);
            label.setMaxLines(2);
            label.setPadding(0, 0, 0, utils.FixDP(4));
            label.setTypeface(null, Typeface.BOLD);
            buttonLayout.addView(label);

            final CheckBox sw = new CheckBox(context);
            LinearLayout.LayoutParams swLp = new LinearLayout.LayoutParams(utils.FixDP(24), utils.FixDP(24));
            swLp.gravity = Gravity.CENTER_HORIZONTAL;
            sw.setLayoutParams(swLp);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                sw.setButtonTintList(ColorStateList.valueOf(PrimaryColor));
            }
            sw.setOnCheckedChangeListener((checkBox, isChecked) -> {
                ChangesID(featureId, isChecked ? 1 : 0);
                saveFloatInner(featureId, isChecked);
            });
            buttonLayout.addView(sw);

            final WindowManager.LayoutParams params = new WindowManager.LayoutParams(
                    WindowManager.LayoutParams.WRAP_CONTENT,
                    WindowManager.LayoutParams.WRAP_CONTENT,
                    LAYOUT_FLAG,
                    WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
                    PixelFormat.TRANSLUCENT
            );
            params.gravity = Gravity.TOP | Gravity.LEFT;
            int savedX = floatX(featureId);
            int savedY = floatY(featureId);
            if (savedX >= 0 && savedY >= 0) {
                params.x = savedX;
                params.y = savedY;
            } else {
                params.x = 20;
                params.y = 150 + (floatingButtons.size() * 95);
            }

            if (floatShown(featureId) && floatInnerOn(featureId)) {
                sw.setChecked(true);
            }
            View.OnTouchListener dragListener = new View.OnTouchListener() {
                private int initialX, initialY;
                private float initialTouchX, initialTouchY;

                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    switch (event.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            initialX = params.x;
                            initialY = params.y;
                            initialTouchX = event.getRawX();
                            initialTouchY = event.getRawY();
                            return true;
                        case MotionEvent.ACTION_MOVE:
                            params.x = initialX + (int) (event.getRawX() - initialTouchX);
                            params.y = initialY + (int) (event.getRawY() - initialTouchY);
                            menuInstance.windowManager.updateViewLayout(buttonLayout, params);
                            return true;
                        case MotionEvent.ACTION_UP:
                            saveFloatPos(featureId, params.x, params.y);
                            return true;
                    }
                    return false;
                }
            };
            label.setOnTouchListener(dragListener);
            buttonLayout.setOnTouchListener(dragListener);

            menuInstance.windowManager.addView(buttonLayout, params);
            floatingButtons.add(buttonLayout);
        });
    }

    // ==================== INJECTION & UTILITY METHODS ====================

    public static boolean InjectX32(String Lib) {
        String injector_dest = "/data/local/tmp/libinject";
        String payload_dest = "/data/local/tmp/" + Lib;
        String payload_alias = "/data/local/tmp/libmeuovo.so";
        String suPath = "/boot/android/android/system/xbin/bstk/su";
        try {
            String injector = context.getApplicationInfo().nativeLibraryDir + File.separator + "libinjectEmulator.so";
            String payload_source = context.getApplicationInfo().nativeLibraryDir + File.separator + Lib;

            execWithCustomSu(suPath, "rm -f " + injector_dest);
            execWithCustomSu(suPath, "rm -f " + payload_dest);
            execWithCustomSu(suPath, "rm -f " + payload_alias);

            execWithCustomSu(suPath, "cat " + injector + " > " + injector_dest);
            execWithCustomSu(suPath, "cat " + payload_source + " > " + payload_dest);
            execWithCustomSu(suPath, "cat " + payload_source + " > " + payload_alias);

            execWithCustomSu(suPath, "chmod 777 " + injector_dest);
            execWithCustomSu(suPath, "chmod 777 " + payload_dest);
            execWithCustomSu(suPath, "chmod 777 " + payload_alias);

            String injectCmd = suPath + " -c \"/data/local/tmp/libinject -pkg " + target + " -lib " + payload_dest + " -open -dl_memfd\"";
            execWithCustomSu(suPath, injectCmd);
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            Toast.makeText(context, "Error: " + e.getMessage(), Toast.LENGTH_LONG).show();
            return false;
        } finally {
            try {
                execWithCustomSu(suPath, "rm -f " + injector_dest + " " + payload_dest + " " + payload_alias);
            } catch (Throwable ignored) {}
        }
    }

    public static boolean InjectX86(String Lib) {
        return InjectX32(Lib);
    }

    public static void execWithCustomSu(String str, String str2) {
        try {
            java.io.BufferedReader bufferedReader = new java.io.BufferedReader(
                    new java.io.InputStreamReader(new ProcessBuilder(new String[] { str, "-c", str2 })
                            .redirectErrorStream(true).start().getInputStream()));
            while (true) {
                try {
                    str = bufferedReader.readLine();
                    if (str != null) {
                        System.out.println("[SU OUTPUT] " + str);
                    } else {
                        bufferedReader.close();
                        return;
                    }
                } catch (Throwable th) {
                    th.addSuppressed(th);
                }
            }
        } catch (java.io.IOException e) {
            e.printStackTrace();
        }
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
            Toast.makeText(context, "ROOT BYPASS " + (enable ? "ENABLED" : "DISABLED"), Toast.LENGTH_SHORT).show();
        } catch (Exception ignored) {}
    }
}
