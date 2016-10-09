
#ifndef CUBEDRIVERTEST_H_
#define CUBEDRIVERTEST_H_

#include <jni.h>

extern "C" {

JNIEXPORT jstring JNICALL Java_com_mycardboarddreams_jacobsladder_deluxe_drivers_goals_GoalObjectTest_runGoalObjectTests(
		JNIEnv* jenv, jobject obj, jint x_blocks, jint y_blocks, jint z_blocks);

}

#endif /* CUBEDRIVERTEST_H_ */
