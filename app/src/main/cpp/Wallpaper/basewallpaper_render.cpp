#include "Wallpaper/basewallpaper.h"
#include "Wallpaper/logger.h"
#include "Wallpaper/timer.h"
#include "Wallpaper/wallpaperutils.h"
#include <unistd.h>

#define LOG_TAG "BaseWallpaper Render"

void BaseWallpaper::renderLoopInThread()
{
    bool renderingEnabled = true;
    bool renderThisFrame = true;


    LOG_INFO_DEBUG("  Entering renderLoop() (%d)", wallpaper_id);

    while (renderingEnabled)
    {
        pthread_mutex_lock(&_mutex_rendering);

        // process incoming messages---------------
        switch (_msg_renderer) {
			case MSG_NONE:
			case MSG_PAUSE:
				break;

    		case MSG_WINDOW_SET:
    			LOG_INFO_DEBUG("case SET WINDOW (%d)", wallpaper_id);
            	setupOpenGlContextInThread();
            	LOG_INFO_DEBUG("finished case SET WINDOW (%d)", wallpaper_id);
            	Timer::printTimes();
                break;

            case MSG_RESET_SCENE_VBOS:
            case MSG_WINDOW_RESET:
            	LOG_INFO_DEBUG((_msg_renderer == MSG_RESET_SCENE_VBOS) ? "case reset SCENE VBOs (%d)" : "case RESET WINDOW (%d)", wallpaper_id);
            	setupOpenGlContextInThread(true);
            	LOG_INFO_DEBUG((_msg_renderer == MSG_RESET_SCENE_VBOS) ? "finished case reset SCENE VBOs (%d)" : "finished case RESET WINDOW (%d)", wallpaper_id);
				break;

			case MSG_RENDER_LOOP_EXIT:
				LOG_INFO_DEBUG("case EXIT LOOP (%d)", wallpaper_id);
            	destroyGLContextInThread();
                renderingEnabled = false;
            	renderThisFrame = false;
            	LOG_INFO_DEBUG("finished case EXIT LOOP (%d)", wallpaper_id);
            	break;

        }

        _msg_renderer = MSG_NONE;

        if(renderingEnabled && isPaused){
        	renderThisFrame = false;
			pthread_cond_wait(&_condition_renderer, &_mutex_rendering);
			LOG_INFO_DEBUG("Resuming thread from pause condition.");
			setThisFrameDuration();
        }
        else if(renderThisFrame){
        	renderThread();
        }

        pthread_mutex_unlock(&_mutex_rendering);

        if(renderingEnabled)
        	usleep(20000);

        if(renderThisFrame)
        {
            pthread_cond_signal(&_condition_driver);
        }

		setThisFrameDuration();

		renderThisFrame = true;

    }

    LOG_INFO_DEBUG("Render loop exits (%d)", wallpaper_id);

}

void BaseWallpaper::iterateDriver(){

	while(!_msg_driver){
		pthread_mutex_lock(&_mutex_driver);
		pthread_cond_wait(&_condition_driver, &_mutex_driver);

		if(!_msg_driver){
			iterateFrameInThread(fractionOfPreferredFrame);
		}

		pthread_mutex_unlock(&_mutex_driver);
	}
}


void BaseWallpaper::renderThread()
{
	static GLuint debugTimeIteration = 0;
	static bool hasRendered = false;
	static long long lastFrameRenderTime = 0;

	if (!_display || !_surface)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return;
	}

#ifdef DEBUG_MODE_TIMER
	if(!hasRendered){
		LOG_ERROR_DEBUG("First render loop %llu", Timer::stopTimer(Timer::LONG_TERM));
		Timer::startTimer(Timer::LONG_TERM);
		hasRendered = true;
	}
#endif

	if(DEBUG_OPENGL)
		Timer::startTimer();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderFrameInThread(fractionOfPreferredFrame);

	if (!eglSwapBuffers(_display, _surface)) {
		LOG_ERROR("eglSwapBuffers() returned error %d", eglGetError());
	}
	if(DEBUG_OPENGL && Constants::DEBUG_DETAIL_MODE && ++debugTimeIteration > 30){
		lastFrameRenderTime = Timer::stopTimer();
		LOG_INFO_DEBUG("Last frame took %lld  (%d)", lastFrameRenderTime, wallpaper_id);
		debugTimeIteration = 0;
	}
}


void BaseWallpaper::setThisFrameDuration()
{
	static long long lastFrameEnded = 0;

	long long thisFrameTimestamp = Timer::checkCurrentTimestamp();
	long long timeSinceLastFrame = thisFrameTimestamp - lastFrameEnded;
	lastFrameEnded = thisFrameTimestamp;
	fractionOfPreferredFrame = (GLfloat)timeSinceLastFrame/(GLfloat)PREFERRED_FRAME_DURATION;
}

