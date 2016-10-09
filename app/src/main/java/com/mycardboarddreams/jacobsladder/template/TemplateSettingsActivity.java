package com.mycardboarddreams.jacobsladder.template;

import android.content.SharedPreferences;

import com.mycardboarddreams.jacobsladder.baselib.BaseSettingsActivity;

public class TemplateSettingsActivity extends BaseSettingsActivity {

	@Override
	protected Class<TemplateWallpaperService> getWallpaperService() {
		return TemplateWallpaperService.class;
	}

	@Override
	public void onSharedPreferenceChanged(SharedPreferences arg0, String arg1) {
		// Replace with you code
		
	}

}
