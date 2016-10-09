#ifndef UTILSTEST_H_
#define UTILSTEST_H_

#include <jni.h>

extern "C" {

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_UtilsTests_areOppositeDirections(JNIEnv* jenv, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_UtilsTests_startStopTimer(JNIEnv* jenv, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_UtilsTests_startStopSecondTimer(JNIEnv* jenv, jobject obj);

};


#endif /* UTILSTEST_H_ */
