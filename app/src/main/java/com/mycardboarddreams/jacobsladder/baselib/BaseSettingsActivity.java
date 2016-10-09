package com.mycardboarddreams.jacobsladder.baselib;

import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.preference.PreferenceActivity;
import android.preference.PreferenceManager;

import com.mycardboarddreams.jacobsladder.deluxe.R;
import com.mycardboarddreams.jacobsladder.baselib.NativeGLWallpaperService.NativeGLWallpaperEngine;

public abstract class BaseSettingsActivity extends PreferenceActivity implements SharedPreferences.OnSharedPreferenceChangeListener{
    
    /**
     * This method is needed to indicate the wallpaper engine class that the 
     * Settings activity is bound to.  It uses this to signal when it has returned
     * in the method {@link #onSettingsActivityReturned() onSettingsActivityReturned}
     * 
     * @return your Wallpaper Engine class 
     */
    protected abstract Class<? extends NativeGLWallpaperService<? extends NativeGLWallpaperEngine>> getWallpaperService();
	
    @Override
    protected void onResume() {
    	super.onResume();

		SharedPreferences defaultSharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
		defaultSharedPreferences.registerOnSharedPreferenceChangeListener(this);
		Editor editor = defaultSharedPreferences.edit();
		editor.putString(getString(R.string.key_signal_settings_started), String.valueOf(Math.random()));
		editor.apply();
	}
	
    @Override
    protected void onPause() {
    	super.onPause();

		SharedPreferences defaultSharedPreferences = PreferenceManager.getDefaultSharedPreferences(this);
		defaultSharedPreferences.unregisterOnSharedPreferenceChangeListener(this);
		
		Editor editor = defaultSharedPreferences.edit();
		editor.putString(getString(R.string.key_signal_settings_stopped), String.valueOf(Math.random()));
		editor.apply();
	}
}
