package com.kanishkcheat;

import android.animation.ValueAnimator;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.View;
import android.view.animation.LinearInterpolator;

/**
 * Animated Circular Loading Indicator matching UI theme.
 */
public class CircleLoadingView extends View {

    private final Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final Paint bgPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    private final RectF bounds = new RectF();
    private int primaryColor = 0xFFC62828;
    private float strokeWidthPx;
    private ValueAnimator rotateAnimator;
    private float currentAngle = 0f;

    public CircleLoadingView(Context context) {
        super(context);
        init();
    }

    public CircleLoadingView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public CircleLoadingView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        strokeWidthPx = 3.5f * getContext().getResources().getDisplayMetrics().density;

        bgPaint.setStyle(Paint.Style.STROKE);
        bgPaint.setStrokeWidth(strokeWidthPx);
        bgPaint.setColor(0x25FFFFFF);
        bgPaint.setStrokeCap(Paint.Cap.ROUND);

        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeWidth(strokeWidthPx);
        paint.setColor(primaryColor);
        paint.setStrokeCap(Paint.Cap.ROUND);
    }

    public void setPrimaryColor(int color) {
        primaryColor = color;
        paint.setColor(primaryColor);
        invalidate();
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        float pad = strokeWidthPx + 2f;
        bounds.set(pad, pad, w - pad, h - pad);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        // Draw track circle
        canvas.drawOval(bounds, bgPaint);

        // Draw animated rotating arc (270 degrees sweep)
        canvas.drawArc(bounds, currentAngle, 270f, false, paint);
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        startAnimation();
    }

    @Override
    protected void onDetachedFromWindow() {
        stopAnimation();
        super.onDetachedFromWindow();
    }

    @Override
    protected void onVisibilityChanged(View changedView, int visibility) {
        super.onVisibilityChanged(changedView, visibility);
        if (visibility == View.VISIBLE) {
            startAnimation();
        } else {
            stopAnimation();
        }
    }

    private void startAnimation() {
        if (rotateAnimator != null && rotateAnimator.isRunning()) return;
        rotateAnimator = ValueAnimator.ofFloat(0f, 360f);
        rotateAnimator.setDuration(900);
        rotateAnimator.setRepeatCount(ValueAnimator.INFINITE);
        rotateAnimator.setInterpolator(new LinearInterpolator());
        rotateAnimator.addUpdateListener(anim -> {
            currentAngle = (float) anim.getAnimatedValue();
            invalidate();
        });
        rotateAnimator.start();
    }

    private void stopAnimation() {
        if (rotateAnimator != null) {
            rotateAnimator.cancel();
            rotateAnimator = null;
        }
    }
}
