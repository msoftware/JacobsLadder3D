
#ifndef IDENTITYTEST_H_
#define IDENTITYTEST_H_

#include <jni.h>

extern "C" {

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityTests_setUpIdentity(JNIEnv* jenv, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityTests_setProperties(JNIEnv* jenv, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityTests_setContactSides(JNIEnv* jenv, jobject obj);

	JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityTests_tearDownIdentity(JNIEnv* jenv, jobject obj);

};


#endif /* IDENTITYTEST_H_ */
