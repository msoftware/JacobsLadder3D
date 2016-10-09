
#include <time.h>
#include <math.h>

#include "Wallpaper/timer.h"
#include "Wallpaper/wallpaperutils.h"

long long Timer::currentTimestamp0 = 0;
long long Timer::currentTimestamp1 = 0;

unsigned long long *Timer::SavedTimes = new unsigned long long[20];

#define LOG_TAG "Timer"

void Timer::startTimer(char timerNumber)
{
#ifdef DEBUG_MODE_TIMER
	(timerNumber == AD_HOC ? currentTimestamp0 : currentTimestamp1) = checkCurrentTimestamp();
#endif
}

long long Timer::stopTimer(char timerNumber)
{
#ifdef DEBUG_MODE_TIMER
	return checkCurrentTimestamp() - (timerNumber == AD_HOC ? currentTimestamp0 : currentTimestamp1);
#else
	return 0;
#endif
}

long long Timer::checkCurrentTimestamp(){

	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);
	long long secs = ((long long) spec.tv_sec)*1000ll;
	long long xx = (long long)round(spec.tv_nsec / 1.0e6);

	return secs + xx;

}

void Timer::printTimes()
{
#ifdef DEBUG_MODE_TIMER

	if(!Constants::DEBUG_DETAIL_MODE)
		return;

    LOG_ERROR_DEBUG("Wallpaper::start() took %llu ms", SavedTimes[START]);
    LOG_ERROR_DEBUG("Wallpaper::stop() took %llu ms", SavedTimes[STOP]);
    LOG_ERROR_DEBUG("Wallpaper::setWindow() took %llu ms", SavedTimes[SET_WINDOW]);
	LOG_ERROR_DEBUG("Wallpaper::setBlocks() took %llu ms", SavedTimes[SET_BLOCKS]);
	LOG_ERROR_DEBUG("Wallpaper::setInitialCubePositions() took %llu ms", SavedTimes[SET_CUBE_POSITIONS]);
	LOG_ERROR_DEBUG("Wallpaper::setColors() took %llu ms", SavedTimes[SET_COLORS]);
	LOG_ERROR_DEBUG("Wallpaper::setOpenGlContext() took %llu ms", SavedTimes[SET_CUSTOM_OPENGL]);
    LOG_ERROR_DEBUG("Wallpaper::destroyGLContext() took %llu ms", SavedTimes[DESTROY_OPENGL_CONTEXT]);
    LOG_ERROR_DEBUG("Engine::unsetVertices() took %llu ms", SavedTimes[UNSET_VERTICES]);
    LOG_ERROR_DEBUG("Engine::setVertices() took %llu ms", SavedTimes[SET_VERTICES]);
    LOG_ERROR_DEBUG("FaceColors::buildColors() init took %llu ms", SavedTimes[BUILD_COLORS_INIT]);
    LOG_ERROR_DEBUG("FaceColors::buildColors() derive took %llu ms", SavedTimes[BUILD_COLORS_DERIVE]);
    LOG_ERROR_DEBUG("Engine::deriveVerticies() took %llu ms", SavedTimes[DERIVE_VERTICES]);
    LOG_ERROR_DEBUG("Engine::deriveBackground() took %llu ms", SavedTimes[DERIVE_BACKGROUND]);
    LOG_ERROR_DEBUG("Wallpaper::initializeOpenGLContext() took %llu ms", SavedTimes[INIT_OPENGL_CONTEXT]);

#endif
}

