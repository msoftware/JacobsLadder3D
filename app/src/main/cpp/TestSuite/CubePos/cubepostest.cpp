#include "TestSuite/CubePos/cubepostest.h"
#include "Geometry/cubepos.h"

#define LOG_TAG "CubePosTest"

bool isClearCubePos(CubePos *c)
{
	return 	c != 0 &&
			c->stepMovedFragment == 0 &&
			c->_canMoveToBits == 0 &&
			c->addedToList == NO_LIST &&
			c->diff_destx == 0 &&
			c->diff_desty == 0 &&
			c->diff_destz == 0;
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_CubePosTests_createNewCubePos(JNIEnv* jenv, jobject obj)
{
	CubePos *c = new CubePos();
	return c != 0;
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_CubePosTests_setPositionInCubePos(JNIEnv* jenv, jobject obj, jint x, jint y, jint z)
{
	CubePos *c = new CubePos();
	Position *pos = new Position();
	pos->x = x;
	pos->y = y;
	pos->z = z;
	c->placeCubeAt(0);
	delete pos;
	return c != 0 && c->x == x && c->y == y && c->z == z && isClearCubePos(c);
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_CubePosTests_setCubeAsMoving(JNIEnv* jenv, jobject obj, jint x, jint y, jint z, jbyte side)
{
	CubePos *c = new CubePos();
	Position *pos = new Position();
	pos->x = x;
	pos->y = y;
	pos->z = z;
	c->placeCubeAt(0);
	delete pos;

	c->setNewDirectionVariables(side);

	Identity *target = c->getNextIdentity();
	bool negativeDestination = (target->x < 0 || target->y < 0 || target->z < 0);

	return !negativeDestination && (target->x == x-1 && target->y == y && target->z == z) && c->moveDirection == side;
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_CubePosTests_setCubeAsLanded(JNIEnv* jenv, jobject obj)
{
	CubePos *c = new CubePos();
	Position *pos = new Position();
	c->placeCubeAt(0);
	delete pos;

	c->setNewDirectionVariables(RIGHT_SIDE);
	c->updateVariablesAfterMoving();

	Identity *target = c->getNextIdentity();
	bool negativePosition = (target->x < 0 || target->y < 0 || target->z < 0);

	return !negativePosition && (target->x == 1 && target->y == 0 && target->z == 0) && c->moveDirection == 0;
}


JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_CubePosTests_hasTraveledTooLong(JNIEnv* jenv, jobject obj, jint expect, jint actual)
{
//	CubePos *c = new CubePos(10);
//	c->expectedJourneyLength = expect;
//	c->currentJourneyLength = actual;
//
	return true;
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_CubePosTests_hasDitheredTooLong(JNIEnv* jenv, jobject obj)
{
	return false;
}
