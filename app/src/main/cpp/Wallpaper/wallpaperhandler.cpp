#include "Wallpaper/basewallpaper.h"
#include "Wallpaper/logger.h"

#include "Wallpaper/wallpaperhandler.h"
#include "Wallpaper/native_handle.h"
#include "Wallpaper/wallpaperutils.h"

#define LOG_TAG "Wallpaper handler"

BaseWallpaper* WallpaperHandler::getWallpaperForHandle(jint id, jobject obj, JNIEnv* jenv)
{
	BaseWallpaper *existingWallpaper = NativeHandle::getHandle<BaseWallpaper>(jenv, obj);

	if(!existingWallpaper){
		LOG_ERROR_DEBUG("Creating new wallpaper (%p) and setting handle", existingWallpaper);
		existingWallpaper = WallpaperHandler::getNewWallpaper(id);

		NativeHandle::setHandle(jenv, obj, existingWallpaper);
	}

	if(!existingWallpaper)
		LOG_ERROR("Unable to create wallpaper (%d)", id);

	return existingWallpaper;
}




