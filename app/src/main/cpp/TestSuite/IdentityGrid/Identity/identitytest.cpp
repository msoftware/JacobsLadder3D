#include "TestSuite/Identitygrid/Identity/identitytest.h"
#include "Geometry/CubePos.h"
#include "Drivers/IdentityGrid/identity.h"

Identity *testIdentity = 0;

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityTests_setUpIdentity(JNIEnv* jenv, jobject obj)
{
	if(testIdentity){
		delete testIdentity;
		testIdentity = 0;
	}

	testIdentity = new Identity();
	return (testIdentity != 0);
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityTests_setProperties(JNIEnv* jenv, jobject obj)
{
	if(!testIdentity)
		return false;

	CubePos *c = new CubePos();

	testIdentity->setProperties(c, STATE_STATIC, LEFT_SIDE);

	return 	(testIdentity->cube == c) &&
			(testIdentity->state == STATE_STATIC) &&
			(testIdentity->movingDirection == LEFT_SIDE) &&
			(testIdentity->isInUntenablePosition());

}


JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityTests_setContactSides(JNIEnv* jenv, jobject obj)
{
	if(!testIdentity)
		return false;

	testIdentity->sidesTouchingStaticCubes |= RIGHT_SIDE;

	return !testIdentity->isInUntenablePosition();
}


JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityTests_tearDownIdentity(JNIEnv* jenv, jobject obj)
{
	if(testIdentity){
		delete testIdentity;
		testIdentity = 0;
	}
	return (testIdentity == 0);
}


