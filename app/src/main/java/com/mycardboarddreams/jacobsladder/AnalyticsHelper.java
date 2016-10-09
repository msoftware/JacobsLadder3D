package com.mycardboarddreams.jacobsladder;

import android.content.Context;

import com.google.android.gms.analytics.GoogleAnalytics;
import com.google.android.gms.analytics.HitBuilders;
import com.google.android.gms.analytics.Tracker;
import com.mycardboarddreams.jacobsladder.deluxe.R;

public class AnalyticsHelper {
    private static final String CATEGORY_USAGE = "Usage";
    private static final String CATEGORY_SETTINGS = "Settings";

    private static final String TRACK_SET_WALLPAPER = "Set wallpaper";
    private static final String TRACK_UNSET_WALLPAPER = "Unset wallpaper";
    private static final String TRACK_PAUSE_TAP_WALLPAPER = "Pause by tap";
    private static final String TRACK_UNPAUSE_TAP_WALLPAPER = "Unpause by tap";
    private static final String TRACK_TOUCH_TO_MANIPULATE = "Touch manipulate";

    private static final String TRACK_CHANGE_DIMENSIONS = "Change dimensions";
    private static final String TRACK_CHANGE_THEME = "Change theme";
    private static final String TRACK_CHANGE_BRIGHTNESS = "Change brightness";

    private static Tracker mTracker;

    private static Tracker getInstance(Context context) {
        if (mTracker == null) {
            GoogleAnalytics analytics = GoogleAnalytics.getInstance(context);
            mTracker = analytics.newTracker(R.xml.global_tracker);
            mTracker.enableAutoActivityTracking(true);
        }
        return mTracker;

    }

    public static void trackSetWallpaper(Context context) {
        Tracker Tracker = getInstance(context);

        Tracker.send(new HitBuilders.EventBuilder()
                .setCategory(CATEGORY_USAGE)
                .setAction(TRACK_SET_WALLPAPER)
                .build()
        );
    }

    public static void trackUnSetWallpaper(Context context) {
        Tracker Tracker = getInstance(context);

        Tracker.send(new HitBuilders.EventBuilder()
                .setCategory(CATEGORY_USAGE)
                .setAction(TRACK_UNSET_WALLPAPER)
                .build()
        );
    }

    public static void trackPauseWallpaperByTouch(Context context) {
        Tracker Tracker = getInstance(context);

        Tracker.send(new HitBuilders.EventBuilder()
                .setCategory(CATEGORY_USAGE)
                .setAction(TRACK_PAUSE_TAP_WALLPAPER)
                .build()
        );
    }

    public static void trackUnpauseWallpaperByTouch(Context context) {
        Tracker Tracker = getInstance(context);

        Tracker.send(new HitBuilders.EventBuilder()
                .setCategory(CATEGORY_USAGE)
                .setAction(TRACK_UNPAUSE_TAP_WALLPAPER)
                .build()
        );
    }

    public static void trackTouchToManipulate(Context context) {
        Tracker Tracker = getInstance(context);

        Tracker.send(new HitBuilders.EventBuilder()
                .setCategory(CATEGORY_USAGE)
                .setAction(TRACK_TOUCH_TO_MANIPULATE)
                .build()
        );
    }

    public static void trackDimensionsChanged(Context context, int size) {
        Tracker Tracker = getInstance(context);

        Tracker.send(new HitBuilders.EventBuilder()
                .setCategory(CATEGORY_SETTINGS)
                .setAction(TRACK_CHANGE_DIMENSIONS)
                .setLabel(String.valueOf(size))
                .build()
        );
    }

    public static void trackThemeChanged(Context context, String theme) {
        Tracker Tracker = getInstance(context);

        Tracker.send(new HitBuilders.EventBuilder()
                .setCategory(CATEGORY_SETTINGS)
                .setAction(TRACK_CHANGE_THEME)
                .setLabel(theme)
                .build()
        );
    }

    public static void trackBrightnessChanged(Context context, int brightnessLevel) {
        Tracker Tracker = getInstance(context);

        Tracker.send(new HitBuilders.EventBuilder()
                .setCategory(CATEGORY_SETTINGS)
                .setAction(TRACK_CHANGE_BRIGHTNESS)
                .setLabel(String.valueOf(brightnessLevel))
                .build()
        );
    }

}
