package com.kanishkcheat;

import android.content.Context;
import android.util.TypedValue;

public class Utils {

    Context context;

    public Utils(Context globContext) {
        context = globContext;
    }

    public int FixDP(int i) {
        return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, (float) i, context.getResources().getDisplayMetrics());
    }

    public float FixDP(float i) {
        return TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, i, context.getResources().getDisplayMetrics());
    }

    public static android.graphics.drawable.Drawable createRoundedDrawable(int radiusDp, int color) {
        float radiusPx = TypedValue.applyDimension(
                TypedValue.COMPLEX_UNIT_DIP, radiusDp, android.content.res.Resources.getSystem().getDisplayMetrics());
        android.graphics.drawable.GradientDrawable drawable = new android.graphics.drawable.GradientDrawable();
        drawable.setColor(color);
        drawable.setCornerRadius(radiusPx);
        return drawable;
    }

    public static android.graphics.drawable.Drawable createVerticalGradientBackground() {
        return new android.graphics.drawable.GradientDrawable(
                android.graphics.drawable.GradientDrawable.Orientation.TOP_BOTTOM,
                new int[]{android.graphics.Color.parseColor("#121212"), android.graphics.Color.parseColor("#1C1C1C")}
        );
    }

    public static android.graphics.drawable.Drawable createEyeDrawable(android.content.Context context, boolean visible, int color) {
        final float sizePx = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 24, context.getResources().getDisplayMetrics());
        final int size = (int) sizePx;
        android.graphics.Bitmap bitmap = android.graphics.Bitmap.createBitmap(size, size, android.graphics.Bitmap.Config.ARGB_8888);
        android.graphics.Canvas canvas = new android.graphics.Canvas(bitmap);
        android.graphics.Paint paint = new android.graphics.Paint(android.graphics.Paint.ANTI_ALIAS_FLAG);
        paint.setColor(color);
        paint.setStyle(android.graphics.Paint.Style.STROKE);
        paint.setStrokeWidth(TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 2, context.getResources().getDisplayMetrics()));

        float cx = size / 2f;
        float cy = size / 2f;
        float r = size * 0.35f;

        // Draw eye shape
        android.graphics.Path path = new android.graphics.Path();
        path.moveTo(cx - r, cy);
        path.quadTo(cx, cy - r*1.2f, cx + r, cy);
        path.quadTo(cx, cy + r*1.2f, cx - r, cy);
        canvas.drawPath(path, paint);

        // Draw pupil
        paint.setStyle(android.graphics.Paint.Style.FILL);
        canvas.drawCircle(cx, cy, r * 0.4f, paint);

        if (!visible) {
            // Draw slash for hidden state
            paint.setStyle(android.graphics.Paint.Style.STROKE);
            canvas.drawLine(cx - r*0.7f, cy - r*0.7f, cx + r*0.7f, cy + r*0.7f, paint);
        }

        return new android.graphics.drawable.BitmapDrawable(context.getResources(), bitmap);
    }

}

