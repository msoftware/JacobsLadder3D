#ifndef JNIAPITEST_H_
#define JNIAPITEST_H_

#include <jni.h>

extern "C" {

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityGridTests_setUpIdentityGrid(JNIEnv* jenv, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityGridTests_resizeIdentityGrid(JNIEnv* jenv, jobject obj, jint x, jint y, jint z);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityGridTests_tearDownIdentityGrid(JNIEnv* jenv, jobject obj);

};


#endif /* JNIAPITEST_H_ */
