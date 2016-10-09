package com.mycardboarddreams.jacobsladder.template;

import android.content.SharedPreferences;

import com.mycardboarddreams.jacobsladder.baselib.BaseSettingsActivity;
import com.mycardboarddreams.jacobsladder.baselib.NativeGLWallpaperService;

public class TemplateWallpaperService extends NativeGLWallpaperService<TemplateWallpaperService.TemplateWallpaperEngine>{

	public static final String LOG_TAG = TemplateWallpaperService.class.getName();

	@Override
	protected TemplateWallpaperEngine getInstance() {
		return new TemplateWallpaperEngine();
	}

	@Override
	protected Class<? extends BaseSettingsActivity> getSettingsActivityClass() {
		return TemplateSettingsActivity.class;
	}

	public class TemplateWallpaperEngine extends NativeGLWallpaperService<TemplateWallpaperService.TemplateWallpaperEngine>.NativeGLWallpaperEngine{

		@Override
		protected void onWallpaperResuming() {

		}

		@Override
		public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {

		}

	}

}
