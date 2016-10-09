
#ifndef WALLPAPERUTILS_H_
#define WALLPAPERUTILS_H_


#include "Wallpaper/timer.h"
#include "Wallpaper/logger.h"
#include <assert.h>

class Constants{
public:
	static bool DEBUG_DETAIL_MODE;
	static bool RENDER_OPENGL;

};

#define DEBUG_MODE_TIMER true
#define DEBUG_OPENGL true

#define FLOATING_POINT_PRECISION 65536

#define RANDOM(n) (rand()%(n))


#define LOG_INFO_DEBUG(...) if(Constants::DEBUG_DETAIL_MODE)LOG_INFO(__VA_ARGS__)
#define LOG_ERROR_DEBUG(...) if(Constants::DEBUG_DETAIL_MODE)LOG_ERROR(__VA_ARGS__)

#ifdef DEBUG_MODE_TIMER

#define START_TIMER Timer::startTimer();
#define END_TIMER(num) Timer::SavedTimes[num] = Timer::stopTimer();

#else

#define START_TIMER
#define END_TIMER(num)

#endif

#endif
