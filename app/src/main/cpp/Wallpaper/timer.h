
#ifndef TIMER_H
#define TIMER_H

class Timer
{


public:

	enum TimerType
	{
		AD_HOC = 0,
		LONG_TERM = 1,
	};

	enum ModuleNames{
		START,
		STOP,
		SET_BLOCKS,
		SET_COLORS,
		SET_WINDOW,
		SET_CUSTOM_OPENGL,
		DESTROY_OPENGL_CONTEXT,
		INIT_OPENGL_CONTEXT,
		BUILD_COLORS_INIT,
		BUILD_COLORS_DERIVE,
		UNSET_VERTICES,
		SET_CUBE_POSITIONS,
		SET_VERTICES,
		DERIVE_VERTICES,
		DERIVE_BACKGROUND,
	};

	static void startTimer(char timerNumber = AD_HOC);
	static long long stopTimer(char timerNumber = AD_HOC);
	static long long checkCurrentTimestamp();

    // store timer variables
	static unsigned long long *SavedTimes;

    static void printTimes();

private:
	static long long currentTimestamp0;
	static long long currentTimestamp1;

};

#endif
