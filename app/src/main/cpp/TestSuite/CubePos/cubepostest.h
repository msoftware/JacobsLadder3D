
#ifndef CUBEPOSTEST_H_
#define CUBEPOSTEST_H_


#include <jni.h>

extern "C" {

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_CubePosTests_createNewCubePos(JNIEnv* jenv, jobject obj);
	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_CubePosTests_setPositionInCubePos(JNIEnv* jenv, jobject obj, jint x, jint y, jint z);
	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_CubePosTests_setCubeAsMoving(JNIEnv* jenv, jobject obj, jint x, jint y, jint z, jbyte side);
	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_CubePosTests_setCubeAsLanded(JNIEnv* jenv, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_CubePosTests_hasTraveledTooLong(JNIEnv* jenv, jobject obj, jint expect, jint actual);
	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_CubePosTests_hasDitheredTooLong(JNIEnv* jenv, jobject obj);

};




#endif /* CUBEPOSTEST_H_ */
