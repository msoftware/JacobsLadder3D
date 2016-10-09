#include "TestSuite/Utils/utilstest.h"
#include "Utils/utils.h"
#include "Drivers/IdentityGrid/identity.h"
#include "Wallpaper/wallpaperutils.h"
#include "Wallpaper/timer.h"

#include <unistd.h>

#define LOG_TAG "UtilsTests"


JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_UtilsTests_areOppositeDirections(JNIEnv* jenv, jobject obj)
{
	return Utils::areOppositeDirections(LEFT_SIDE, RIGHT_SIDE) &&
			!Utils::areOppositeDirections(LEFT_SIDE, TOP_SIDE) &&
			!Utils::areOppositeDirections(NEAR_SIDE, TOP_SIDE);
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_UtilsTests_startStopTimer(JNIEnv* jenv, jobject obj)
{
	long long startTime = Timer::checkCurrentTimestamp();
	Timer::startTimer();
	usleep(2000);
	long long stopTime = Timer::stopTimer();
	long long nowTime = Timer::checkCurrentTimestamp();

	return (stopTime > 0) && (nowTime - startTime >= stopTime);
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_UtilsTests_startStopSecondTimer(JNIEnv* jenv, jobject obj)
{
	Timer::startTimer();

	long long secondStartTime = Timer::checkCurrentTimestamp();
	Timer::startTimer(Timer::LONG_TERM);
	usleep(2000);
	long long secondStopTime = Timer::stopTimer(Timer::LONG_TERM);
	long long nowTime = Timer::checkCurrentTimestamp();

	usleep(2000);

	long long firstStopTime = Timer::stopTimer();

	return (secondStopTime > 0) && (nowTime - secondStartTime >= secondStopTime) && (firstStopTime > secondStopTime);
}
