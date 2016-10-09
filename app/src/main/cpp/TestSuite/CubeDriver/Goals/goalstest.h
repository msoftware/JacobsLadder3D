
#ifndef CUBEDRIVERTEST_H_
#define CUBEDRIVERTEST_H_

#include <jni.h>
#include "Utils/alliterativecollections.h"
#include "Drivers/Goals/goals.h"

extern "C" {

JNIEXPORT jstring JNICALL Java_com_mycardboarddreams_jacobsladder_deluxe_drivers_goals_GoalsTest_runGoalsTests(JNIEnv* jenv, jobject obj);

}

class GoalsTest{
public:
	int X_BLOCKS, Y_BLOCKS, Z_BLOCKS;

	AllIterativeCollections *allCollections;
	Identity ****_identityArray;

	Goals *goals;

	const char *runGoals();

private:
	const char * initializeGoals();
	const char * createNewGoals();

	void setupIdentityArray();
};

#endif /* CUBEDRIVERTEST_H_ */
