package com.mycardboarddreams.jacobsladder;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.text.TextUtils;

import com.mycardboarddreams.jacobsladder.deluxe.R;

class PreferenceTracker {

	private static boolean isInitialized = false;
	private static void initialize(Context context){
		if(!isInitialized){

			LAST_SET_THEME = getCurrentTheme(context);
			LAST_SET_BRIGHTNESS = getCurrentBrightness(context);
			LAST_SET_COMPLEXITY = getCurrentComplexity(context);

			isInitialized = true;
		}
	}


	static void sendTrackerInfoIfChanged(Context context){
		initialize(context);

		trackIfBrightnessChanged(context);
		trackIfComplexityChanged(context);
		trackIfThemeHasChanged(context);
	}

	private static String LAST_SET_THEME = "";

	private static boolean themeHasChanged(Context context)
	{
		String themeName = getCurrentTheme(context);
		boolean hasChanged = false;
		if(!TextUtils.equals(LAST_SET_THEME, themeName))
			hasChanged = true;

		LAST_SET_THEME = themeName;

		return hasChanged;
	}

	private static String getCurrentTheme(Context context){
		SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(context);

		return sharedPreferences.getString(context.getString(R.string.key_select_theme), context.getString(R.string.default_color_select));
	}

	private static void trackIfThemeHasChanged(Context context)
	{
		if(PreferenceTracker.themeHasChanged(context)){
			String themeName = PreferenceTracker.getCurrentTheme(context);

			AnalyticsHelper.trackThemeChanged(context, themeName);
		}
	}




	private static int LAST_SET_BRIGHTNESS = 0;

	private static boolean brightnessHasChanged(Context context)
	{
		int brightness = getCurrentBrightness(context);
		boolean hasChanged = false;
		if(LAST_SET_BRIGHTNESS != brightness)
			hasChanged = true;

		LAST_SET_BRIGHTNESS = brightness;

		return hasChanged;
	}

	private static int getCurrentBrightness(Context context){
		SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(context);

		return sharedPreferences.getInt(context.getString(R.string.key_color_brightness), context.getResources().getInteger(R.integer.default_brightness));
	}

	private static void trackIfBrightnessChanged(Context context){
		if(brightnessHasChanged(context)){
			int brightnessLevel = getCurrentBrightness(context);

			AnalyticsHelper.trackBrightnessChanged(context, brightnessLevel);
		}
	}




	private static int LAST_SET_COMPLEXITY = 0;

	private static boolean complexityHasChanged(Context context)
	{
		int complexity = getCurrentComplexity(context);
		boolean hasChanged = false;
		if(LAST_SET_COMPLEXITY != complexity)
			hasChanged = true;

		LAST_SET_COMPLEXITY = complexity;

		return hasChanged;
	}

	private static int getCurrentComplexity(Context context){
		SharedPreferences sharedPreferences = PreferenceManager.getDefaultSharedPreferences(context);

		return sharedPreferences.getInt(context.getString(R.string.key_complexity_level), context.getResources().getInteger(R.integer.default_complexity_level));
	}

	private static void trackIfComplexityChanged(Context context){
		if(complexityHasChanged(context)){
			int complexity = getCurrentComplexity(context);

			AnalyticsHelper.trackDimensionsChanged(context, complexity);
		}
	}
}
