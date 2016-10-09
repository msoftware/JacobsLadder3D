

#ifndef PERSISTPREFERENCESTEST_H_
#define PERSISTPREFERENCESTEST_H_

#include <jni.h>

extern "C" {

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_PersistPreferencesTests_setUpPersistPreferences(JNIEnv* jenv, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_PersistPreferencesTests_checkSimilarDimensionsAndColor(JNIEnv* jenv, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_PersistPreferencesTests_changedPreferences(JNIEnv* jenv, jobject obj);

};


#endif /* PERSISTPREFERENCESTEST_H_ */
