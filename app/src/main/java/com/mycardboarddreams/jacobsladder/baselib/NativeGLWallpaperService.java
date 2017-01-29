package com.mycardboarddreams.jacobsladder.baselib;

import com.mycardboarddreams.jacobsladder.deluxe.BuildConfig;
import com.mycardboarddreams.jacobsladder.deluxe.R;

import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.preference.PreferenceManager;
import android.service.wallpaper.WallpaperService;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;

public abstract class NativeGLWallpaperService<T extends NativeGLWallpaperService<T>.NativeGLWallpaperEngine> extends WallpaperService {

	private static final String LOG_TAG = NativeGLWallpaperService.class.getSimpleName();

	private static boolean startingSettings = false;

	private static int ENGINE_ID = 1;

	/**
	 * Get an instance of your subclass of NativeGLWallpaperEngine.
	 *
	 * @return instance of your Engine subclass
	 */
	protected abstract T getInstance();


	/**
	 * Get your subclass of BaseSettingsActivity.  Return null
	 * to indicate you do not have one
	 *
	 * @return instance of your Engine subclass
	 */
	protected abstract Class<? extends BaseSettingsActivity> getSettingsActivityClass();

	@Override
	public Engine onCreateEngine() {
		Log.w(LOG_TAG, "Creating Engine Instance..");

		T engine = getInstance();

		return engine;
	}

	/**
	 * Call this method to indicate that the settings are starting (true)
	 * or have finished (false)
	 *
	 * @param starting
	 */
	protected synchronized void setSettingsStarting(boolean starting)
	{
		startingSettings = starting;
	}

	protected boolean isSettingsStarting()
	{
		return startingSettings;
	}

	/**
	 * The actual wallpaper engine. Many instances of your engine will be created,
	 * some simultaneously. Each has its own unique id in the field {@link #engineId engineId}
	 *
	 */
	public abstract class NativeGLWallpaperEngine extends Engine implements OnSharedPreferenceChangeListener {

		protected long nativeHandle;

		private boolean initialized = false;
		private boolean paused = false;

		/**
		 * The id of this specific Engine instance
		 */
		protected int engineId = 0;

		{
			engineId = ENGINE_ID++;
		}

		/**
		 * Called when the wallpaper is going from an off state to
		 * on; this includes on orientation change.  Use this to set up
		 * preferences in the wallpaper
		 */
		protected abstract void onWallpaperResuming();

		@Override
		public void onCreate(SurfaceHolder surfaceHolder) {
			super.onCreate(surfaceHolder);
			if(BuildConfig.DEBUG)
				Log.w(LOG_TAG, "onCreate called (" + engineId + ")");

			nativeOnCreate(engineId);
			nativeOnStart(engineId);

			if(BuildConfig.DEBUG)
				Log.w(LOG_TAG, "onCreate done with pointer (" + Long.toHexString(nativeHandle) + ")");

			PreferenceManager.getDefaultSharedPreferences(NativeGLWallpaperService.this).registerOnSharedPreferenceChangeListener(this);
		}

		@Override
		public void onDestroy() {
			super.onDestroy();

			if(BuildConfig.DEBUG)
				Log.w(LOG_TAG, "On Destroy (" + engineId + ")");

			nativeOnPause(engineId);
			nativeOnStop(engineId);
			nativeOnFinish(engineId);

			Log.d(LOG_TAG, "onDestroy finished (" + engineId + ")");

			PreferenceManager.getDefaultSharedPreferences(NativeGLWallpaperService.this).unregisterOnSharedPreferenceChangeListener(this);
		}

		@Override
		public void onSurfaceCreated(SurfaceHolder holder) {
			super.onSurfaceCreated(holder);

			if(BuildConfig.DEBUG)
				Log.w(LOG_TAG, "On Surface Created (" + engineId + ")");
		}

		@Override
		public void onSurfaceDestroyed(SurfaceHolder holder)
		{
			super.onSurfaceDestroyed(holder);
			if(BuildConfig.DEBUG)
				Log.w(LOG_TAG, "Surface destroyed (" + engineId + ")");

			setInitialized(false);

			nativeSetSurface(engineId, null);
		}

		@Override
		public void onSurfaceChanged(SurfaceHolder holder, int format, int _width, int _height) {
			super.onSurfaceChanged(holder, format, _width, _height);
			if(BuildConfig.DEBUG)
				Log.w(LOG_TAG, "On Surface Changed.  Width: " + _width + "  height: " + _height + "  (" + engineId + ")");

			nativeOnPause(engineId);

			nativeSetSurface(engineId, holder.getSurface());

			if(!isPaused()) {
				onWallpaperResuming();

				nativeOnResume(engineId);
			}

		}


		@Override
		public void onVisibilityChanged(boolean visible) {
			super.onVisibilityChanged(visible);
			if(BuildConfig.DEBUG)
				Log.w(LOG_TAG, "Visibility set to " + (visible ? "visible" : "invisible") + " (" + engineId + ")");

			setInitialized(true);

			if (visible){
				pauseWallpaper(false);

			}else{
				pauseWallpaper(true);
			}
		}

		public void pauseWallpaper(boolean pause)
		{
			if(BuildConfig.DEBUG)
				Log.w(LOG_TAG, "Paused Wallpaper: " + pause);

			if(pause) {
				nativeOnPause(engineId);
			}else{
				onWallpaperResuming();

				nativeOnResume(engineId);
			}

			setPaused(pause);

		}


		private synchronized void setPaused(boolean paused)
		{
			this.paused = paused;
		}

		private void setInitialized(boolean init)
		{
			this.initialized = init;
		}


		protected synchronized boolean isPaused()
		{
			return paused;
		}

		/**
		 * Check to see if the wallpaper is valid (ready/running)
		 */
		protected boolean isInitialized()
		{
			return initialized;
		}

		protected void callSettingsActivity()
		{
			if(BuildConfig.DEBUG)
				Log.d(LOG_TAG, "Calling Settings Activity.");

			Class<? extends BaseSettingsActivity> settingsClass = getSettingsActivityClass();

			if(settingsClass == null)
				return;

			setSettingsStarting(true);
			Intent intent = new Intent(NativeGLWallpaperService.this, settingsClass);
			intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			startActivity(intent);
		}


		@Override
		public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {

			if(BuildConfig.DEBUG)
				Log.d(LOG_TAG, "Shared Prefs changed. Key: " + key);

			if(key.equals(getResources().getString(R.string.key_signal_settings_started))){
				setSettingsStarting(false);
				pauseWallpaper(true);
			}
			if(key.equals(getResources().getString(R.string.key_signal_settings_stopped))){
				setSettingsStarting(false);
			}
		}

		private native void nativeOnCreate(int engineId);
		private native void nativeOnResume(int engineId);
		private native void nativeOnPause(int engineId);
		private native void nativeOnStop(int engineId);
		private native void nativeOnFinish(int engineId);
		private native void nativeOnStart(int engineId);

		private native void nativeSetSurface(int engineId, Surface surface);
	}


	static {
		if(BuildConfig.DEBUG)
			Log.d(LOG_TAG, "Loading C++ library");
		try{
			System.loadLibrary("native-lib");
		} catch (UnsatisfiedLinkError e) {
			// only ignore exception in non-android env
			if ("Dalvik".equals(System.getProperty("java.vm.name"))) throw e;
		}
		if(BuildConfig.DEBUG)
			Log.d(LOG_TAG, "Done loading C++ library");
	}


}
