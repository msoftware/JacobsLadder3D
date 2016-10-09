package com.mycardboarddreams.jacobsladder.preferences.custom;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

public class CustomColorsLargePreference extends CustomColorsCategoryPreference{
	
	public CustomColorsLargePreference(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}
	
	public CustomColorsLargePreference(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	@Override
	protected void onBindView(View view) {
		super.onBindView(view);
		dropDownContainer.expand();
	}
	
}
