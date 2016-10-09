#include "Wallpaper/logger.h"
#include "Wallpaper/basewallpaper.h"
#include "Wallpaper/wallpaperutils.h"

#define LOG_TAG "Wallpaper"

//LIFE CYCLE===============================================
BaseWallpaper::BaseWallpaper(GLuint ident)
{
    LOG_INFO_DEBUG("  Creating Wallpaper instance (%d)", ident);
    pthread_mutex_init(&_mutex_rendering, 0);
    pthread_mutex_init(&_mutex_driver, 0);

    pthread_cond_init(&_condition_renderer, 0);
    pthread_cond_init(&_condition_driver, 0);

    _msg_renderer = MSG_NONE;
    _msg_driver = 0;

    isPaused = false;

    _display = 0;
    _surface = 0;
    _context = 0;
    _width = 0;
    _height = 0;
    _window = 0;
    _renderThreadId = 0;
    _driverThreadId = 0;

	_screenWidthHalf = 0.0f;
	_screenHeightHalf = 0.0f;

    _screenRatio = 0;

    wallpaper_id = ident;

    for(int i = 0; i < 20; i++)
    	Timer::SavedTimes[i] = 0;

    LOG_INFO_DEBUG("  Wallpaper instance created (%d)", wallpaper_id);
    return;
}

BaseWallpaper::~BaseWallpaper()
{
    LOG_INFO_DEBUG("  Destroying wallpaper instance ...(%d)", wallpaper_id);
    pthread_mutex_destroy(&_mutex_rendering);
    pthread_mutex_destroy(&_mutex_driver);

    pthread_cond_destroy(&_condition_renderer);
    pthread_cond_destroy(&_condition_driver);

    LOG_INFO_DEBUG("  Wallpaper instance destroyed (%d)", wallpaper_id);
}

void BaseWallpaper::disposeWindow()
{
	if(_window){
		ANativeWindow_release(_window);
		_window = 0;
	}
}

bool BaseWallpaper::setOrResetWindow(ANativeWindow *window)
{
	START_TIMER

	LOG_INFO_DEBUG("    Setting window (%d)", wallpaper_id);

	bool wasSet = false;

    pthread_mutex_lock(&_mutex_rendering);

    if(window != _window){
    	_msg_renderer = MSG_WINDOW_SET;
    	_window = window;
    	LOG_INFO_DEBUG("    Window set (%d)", wallpaper_id);

		wasSet = true;
    }
    else {
    	_msg_renderer = MSG_WINDOW_RESET;
        pthread_cond_signal(&_condition_renderer);
    	LOG_INFO_DEBUG("    This window already set (%d)", wallpaper_id);

    }

	pthread_mutex_unlock(&_mutex_rendering);

	END_TIMER(Timer::SET_WINDOW)

    return wasSet;

}


void BaseWallpaper::startThread()
{
	START_TIMER

	LOG_INFO_DEBUG("Creating renderer thread (%d)", wallpaper_id);
    pthread_create(&_renderThreadId, 0, renderThreadStartCallback, this);
    pthread_create(&_driverThreadId, 0, driverThreadStartCallback, this);

    END_TIMER(Timer::START)
}

void* BaseWallpaper::renderThreadStartCallback(void *myself)
{
	LOG_INFO_DEBUG("-----------------------------\nRender thread start callback");
    BaseWallpaper *wallpaper = (BaseWallpaper*)myself;

	wallpaper->renderLoopInThread();

    pthread_exit(0);

    return 0;
}

void* BaseWallpaper::driverThreadStartCallback(void *myself)
{
	LOG_INFO_DEBUG("-----------------------------\nDriver thread start callback");
    BaseWallpaper *wallpaper = (BaseWallpaper*)myself;

	wallpaper->iterateDriver();

    pthread_exit(0);

    return 0;
}


void BaseWallpaper::stopAndExitThread()
{
	START_TIMER

    LOG_INFO_DEBUG("Stopping renderer thread (%d)", wallpaper_id);

    pthread_mutex_lock(&_mutex_rendering);
    _msg_renderer = MSG_RENDER_LOOP_EXIT;
    pthread_cond_signal(&_condition_renderer);
    pthread_mutex_unlock(&_mutex_rendering);

    pthread_mutex_lock(&_mutex_driver);
    _msg_driver = 1;
    pthread_cond_signal(&_condition_driver);
    pthread_mutex_unlock(&_mutex_driver);

    LOG_INFO_DEBUG("Joining thread (%d)", wallpaper_id);
    pthread_join(_renderThreadId, 0);
    pthread_join(_driverThreadId, 0);

    LOG_INFO_DEBUG("Renderer thread stopped (%d)", wallpaper_id);

    END_TIMER(Timer::STOP)

    return;
}



void BaseWallpaper::finish()
{
	LOG_INFO_DEBUG("  Finishing wallpaper (%d)", wallpaper_id);

	LOG_INFO_DEBUG("  Wallpaper finished (%d)", wallpaper_id);

    return;
}

void BaseWallpaper::pause()
{
	LOG_INFO_DEBUG("Pausing renderer thread (%d)", wallpaper_id);

    pthread_mutex_lock(&_mutex_rendering);
    isPaused = true;
    pthread_mutex_unlock(&_mutex_rendering);
}


void BaseWallpaper::resume()
{
	LOG_INFO_DEBUG("Resume renderer thread (%d)", wallpaper_id);

    pthread_mutex_lock(&_mutex_rendering);

    isPaused = false;
    pthread_cond_signal(&_condition_renderer);

    pthread_mutex_unlock(&_mutex_rendering);

}


void BaseWallpaper::setupOpenGlContextInThread(bool resettingSurface)
{
	initializeOpenGLContextInThread(resettingSurface);

	START_TIMER

	setupCustomOpenGLInThread(_width, _height);

	END_TIMER(Timer::SET_CUSTOM_OPENGL)
}




bool BaseWallpaper::initializeOpenGLContextInThread(bool reset)
{
	if(!Constants::RENDER_OPENGL)
		return false;

	START_TIMER

	if(!_window){
		LOG_ERROR("  No window. (%d)", wallpaper_id);
		return false;
	}

    const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 8,
        EGL_NONE
    };
    EGLDisplay display;
    EGLConfig frame_buffer_config;
    EGLint numConfigs;
    EGLint format;
    EGLSurface surface;
    EGLContext context;
    EGLint width;
    EGLint height;

    if(reset){
    	if(_display){
			eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			eglDestroyContext(_display, _context);
			eglDestroySurface(_display, _surface);
			eglTerminate(_display);
    	} else {
    		LOG_ERROR_DEBUG("Tried to reset without a display");
    	}
    }


    LOG_INFO_DEBUG("   Initializing OpenGL context (%d)", wallpaper_id);

    if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
        LOG_ERROR("eglGetDisplay() returned error %X", eglGetError());
        return false;
    }
    if (!eglInitialize(display, 0, 0)) {
        LOG_ERROR("eglInitialize() returned error %X", eglGetError());
        return false;
    }

    if (!eglChooseConfig(display, attribs, &frame_buffer_config, 1, &numConfigs)) {
        LOG_ERROR("eglChooseConfig() returned error %X", eglGetError());
        destroyGLContextInThread();
        return false;
    }

    if (!eglGetConfigAttrib(display, frame_buffer_config, EGL_NATIVE_VISUAL_ID, &format)) {
        LOG_ERROR("eglGetConfigAttrib() returned error %X", eglGetError());
        destroyGLContextInThread();
        return false;
    }

    ANativeWindow_setBuffersGeometry(_window, 0, 0, format);

    if (!(surface = eglCreateWindowSurface(display, frame_buffer_config, _window, 0))) {
        LOG_ERROR("eglCreateWindowSurface() returned error %X", eglGetError());
        destroyGLContextInThread();
        return false;
    }

	if (!(context = eglCreateContext(display, frame_buffer_config, 0, 0))) {
        LOG_ERROR("eglCreateContext() returned error %X", eglGetError());
        destroyGLContextInThread();
        return false;
    }

	if (!eglMakeCurrent(display, surface, surface, context)) {
        LOG_ERROR("eglMakeCurrent() returned error %X", eglGetError());
        destroyGLContextInThread();
        return false;
    }

    if (!eglQuerySurface(display, surface, EGL_WIDTH, &width) ||
        !eglQuerySurface(display, surface, EGL_HEIGHT, &height)) {
        LOG_ERROR("eglQuerySurface() returned error %X", eglGetError());
        destroyGLContextInThread();
        return false;
    }

    _display = display;
    _surface = surface;
    _context = context;
    _width = width;
    _height = height;

	_screenWidthHalf = (GLfloat)_width/2.0f;
	_screenHeightHalf = (GLfloat)_height/2.0f;

	_screenRatio = (GLfloat) _width / (GLfloat) _height;

    LOG_INFO_DEBUG("   Finished initializing surface. Width %d, height %d", _width, _height);


	END_TIMER(Timer::INIT_OPENGL_CONTEXT)

    return true;
}


void BaseWallpaper::destroyGLContextInThread()
{
	if(!Constants::RENDER_OPENGL)
		return;

	START_TIMER

	LOG_INFO_DEBUG("Destroying context (%d)", wallpaper_id);

    destroyingCustomGLContextInThread();

    eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(_display, _context);
    eglDestroySurface(_display, _surface);
    eglTerminate(_display);

    _display = EGL_NO_DISPLAY;
    _surface = EGL_NO_SURFACE;
    _context = EGL_NO_CONTEXT;
    _width = 0;
    _height = 0;

    LOG_INFO_DEBUG("Finished destroying GL context. (%d)", wallpaper_id);

	END_TIMER(Timer::DESTROY_OPENGL_CONTEXT)

    return;
}





