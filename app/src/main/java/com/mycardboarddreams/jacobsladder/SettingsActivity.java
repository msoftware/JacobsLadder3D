package com.mycardboarddreams.jacobsladder;


import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceFragment;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;

import com.mycardboarddreams.jacobsladder.baselib.BaseSettingsActivity;
import com.mycardboarddreams.jacobsladder.deluxe.BuildConfig;
import com.mycardboarddreams.jacobsladder.deluxe.R;

public class SettingsActivity extends BaseSettingsActivity
{

	AllSettingsFragment settingsFragment;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// Display the fragment as the main content.
		if(savedInstanceState == null) {
			settingsFragment = new AllSettingsFragment();

			getFragmentManager().beginTransaction()
					.replace(android.R.id.content, settingsFragment)
					.commit();
		}

		if(!ThemesHelper.isCustomColorsSet(this)){
			ThemesHelper.setCurrentThemeToComposer(this);
		}

		JacobsLadderService.nativeIsInDebugMode(BuildConfig.DEBUG);
	}


	@Override
	protected boolean isValidFragment(String fragmentName) {
		return AllSettingsFragment.class.getName().equals(fragmentName);
	}

	@Override
	public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {

		if(TextUtils.equals(key, getString(R.string.select_color_theme))){
			AnalyticsHelper.trackThemeChanged(this, sharedPreferences.getString(key, getString(R.string.default_color_select)));
		} else if(TextUtils.equals(key, getString(R.string.key_color_brightness))){
			AnalyticsHelper.trackBrightnessChanged(this, sharedPreferences.getInt(key, getResources().getInteger(R.integer.default_brightness)));
		} else if(TextUtils.equals(key, getString(R.string.key_complexity_level))){
			AnalyticsHelper.trackDimensionsChanged(this, sharedPreferences.getInt(key, getResources().getInteger(R.integer.default_complexity_level)));
		}

	}


	@Override
	protected Class<JacobsLadderService> getWallpaperService() {
		return JacobsLadderService.class;
	}

	public static class AllSettingsFragment extends PreferenceFragment {
		@Override
		public void onCreate(Bundle savedInstanceState) {
			super.onCreate(savedInstanceState);

			addPreferencesFromResource(R.xml.whitewallpaper_settings);
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
			View v = super.onCreateView(inflater, container, savedInstanceState);
			ListView listView = (ListView)v.findViewById(android.R.id.list);
			listView.setDivider(getResources().getDrawable(R.drawable.preference_list_divider));
			listView.setBackgroundResource(R.color.background_dark_preference);
			listView.setPadding(0, 0, 0, 0);
			return v;
		}
	}

}
