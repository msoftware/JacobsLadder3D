
#ifndef CUBEDRIVERTEST_H_
#define CUBEDRIVERTEST_H_

#include <jni.h>

extern "C" {

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_deluxe_drivers_CubeDriverTest_createDriverInstance(JNIEnv* jenv, jobject obj,
																													  jstring path, jint xBlocks, jint yBlocks, jint zBlocks, jint iterations, jint iterationSpan, jfloat speed);

}

#endif /* CUBEDRIVERTEST_H_ */
