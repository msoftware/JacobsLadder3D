
#ifndef ENGINETEST_H_
#define ENGINETEST_H_

#include <jni.h>

extern "C" {

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_EngineTests_generateTexture(JNIEnv* jenv, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_EngineTests_createEngine(JNIEnv* jenv, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_EngineTests_setGridSizes(JNIEnv* jenv, jobject obj);
	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_EngineTests_setBackgroundColor(JNIEnv* jenv, jobject obj);

};



#endif /* ENGINETEST_H_ */
