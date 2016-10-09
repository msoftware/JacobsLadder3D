
#ifndef ITERATIVEARRAYTEST_H_
#define ITERATIVEARRAYTEST_H_

#include <jni.h>

extern "C" {

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_setUpIterativeArray(JNIEnv* jenv, jobject obj, jint size);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_addItemToArray(JNIEnv* jenv, jobject obj);
	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_removeItemFromArray(JNIEnv* jenv, jobject obj);
	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_overflowArray(JNIEnv* jenv, jobject obj);
	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_resetArray(JNIEnv* jenv, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_getRandomElement(JNIEnv* jenv, jobject obj);
	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_getRandomElementAndRemove(JNIEnv* jenv, jobject obj);
	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_dontAddDuplicates(JNIEnv* jenv, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_tearDownIterativeArray(JNIEnv* jenv, jobject obj);

};



#endif /* ITERATIVEARRAYTEST_H_ */
