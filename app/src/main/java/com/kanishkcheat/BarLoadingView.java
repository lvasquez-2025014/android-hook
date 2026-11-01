package com.kanishkcheat;

import android.content.Context;
import android.util.AttributeSet;

/**
 * Backward compatibility alias for CircleLoadingView.
 */
public class BarLoadingView extends CircleLoadingView {

    public BarLoadingView(Context context) {
        super(context);
    }

    public BarLoadingView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public BarLoadingView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }
}
