
#include "Wallpaper/jniapi.h"
#include "Wallpaper/logger.h"
#include "Wallpaper/wallpaperhandler.h"

#include "Wallpaper/wallpaperutils.h"

#include "Wallpaper/native_handle.h"

#define LOG_TAG "JniApi"

void methodStart(jint id, const char *methodName)
{
	LOG_INFO_DEBUG("------------------------------");
	LOG_INFO_DEBUG("JNI %s (%d)", methodName, id);

#ifdef DEBUG_MODE_TIMER
	LOG_ERROR_DEBUG("JNI %s %llu", methodName, Timer::stopTimer(Timer::LONG_TERM));
#endif
}


//====================================================================
//JNI methods---------------------------------------------------------

JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnCreate(JNIEnv* jenv, jobject obj, jint id)
{
    Timer::startTimer(Timer::LONG_TERM);

	methodStart(id, "nativeOnCreate");

	WallpaperHandler::getWallpaperForHandle(id, obj, jenv);
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnCreate(JNIEnv* jenv, jobject obj, jint id)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnCreate(jenv, obj, id);
}

JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnStart(JNIEnv* jenv, jobject obj, jint id)
{
	methodStart(id, "nativeOnStart");

	BaseWallpaper *wallpaper = WallpaperHandler::getWallpaperForHandle(id, obj, jenv);

	if(wallpaper)
		wallpaper->startThread();
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnStart(JNIEnv* jenv, jobject obj, jint id)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnStart(jenv, obj, id);
}


JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnPause(JNIEnv* jenv, jobject obj, jint id)
{
	methodStart(id, "nativeOnPause");

	BaseWallpaper *wallpaper = WallpaperHandler::getWallpaperForHandle(id, obj, jenv);

	if(wallpaper)
		wallpaper->pause();
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnPause(JNIEnv* jenv, jobject obj, jint id)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnPause(jenv, obj, id);
}

JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnResume(JNIEnv* jenv, jobject obj, jint id)
{
	methodStart(id, "nativeOnResume");

	BaseWallpaper *wallpaper = WallpaperHandler::getWallpaperForHandle(id, obj, jenv);

	if(wallpaper)
		wallpaper->resume();
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnResume(JNIEnv* jenv, jobject obj, jint id)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnResume(jenv, obj, id);
}


JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnStop(JNIEnv* jenv, jobject obj, jint id)
{
	methodStart(id, "nativeOnStop");

	BaseWallpaper *wallpaper = WallpaperHandler::getWallpaperForHandle(id, obj, jenv);

	if(wallpaper)
		wallpaper->stopAndExitThread();
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnStop(JNIEnv* jenv, jobject obj, jint id)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnStop(jenv, obj, id);
}

JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnFinish(JNIEnv* jenv, jobject obj, jint id)
{
	methodStart(id, "nativeOnFinish");

	BaseWallpaper *wallpaper = WallpaperHandler::getWallpaperForHandle(id, obj, jenv);

	if(wallpaper){
		wallpaper->finish();
		delete wallpaper;
		wallpaper = 0;

		NativeHandle::setHandle(jenv, obj, wallpaper);
	}
}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnFinish(JNIEnv* jenv, jobject obj, jint id)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnFinish(jenv, obj, id);
}

JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeSetSurface(JNIEnv* jenv, jobject obj, jint id, jobject surface)
{
	methodStart(id, "nativeSetSurface");

	BaseWallpaper *wallpaper = WallpaperHandler::getWallpaperForHandle(id, obj, jenv);

    if (surface != 0) {
	    ANativeWindow *newWindow = ANativeWindow_fromSurface(jenv, surface);

	    LOG_INFO_DEBUG("  Retrieved window %p (%d)", newWindow, id);
		if(wallpaper && newWindow)
			wallpaper->setOrResetWindow(newWindow);
    } else {
		LOG_INFO_DEBUG("  Releasing window (%d)", id);

	    ANativeWindow *window = wallpaper->_window;

		if(window){
			wallpaper->pause();

			wallpaper->disposeWindow();
		}
		else
			LOG_ERROR("No window (%d)", id);
    }

}
JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeSetSurface(JNIEnv* jenv, jobject obj, jint id, jobject surface)
{
	Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeSetSurface(jenv, obj, id, surface);
}
