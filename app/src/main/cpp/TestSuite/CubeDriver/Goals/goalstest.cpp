#include "TestSuite/CubeDriver/Goals/goalstest.h"
#include "TestSuite/testutils.h"

#define LOG_TAG "tags"

#define CHECK_ASSERTION if(returnValue)return returnValue;


void GoalsTest::setupIdentityArray(){

	_identityArray = new Identity***[X_BLOCKS];

	for (int i = 0; i < X_BLOCKS; ++i) {
		_identityArray[i] = new Identity**[Y_BLOCKS];

		for (int j = 0; j < Y_BLOCKS; ++j){
			_identityArray[i][j] = new Identity*[Z_BLOCKS];

			for (int k = 0; k < Z_BLOCKS; ++k){
				_identityArray[i][j][k] = new Identity(0, 0, STATE_NONE, i, j, k);

				Identity *ident = _identityArray[i][j][k];
				if(j > i){
					if(i < (Y_BLOCKS - j))
						ident->pullToWallsDirection = LEFT_SIDE;
					else
						ident->pullToWallsDirection = TOP_SIDE;
				}
				else{
					if((X_BLOCKS - i) <= j)
						ident->pullToWallsDirection = RIGHT_SIDE;
					else
						ident->pullToWallsDirection = BOTTOM_SIDE;
				}

				ident->setPullToWallsIdentity();
			}
		}
	}

	allCollections = new AllIterativeCollections();

}

const char * GoalsTest::initializeGoals(){
	X_BLOCKS = 14;
	Y_BLOCKS = 15;
	Z_BLOCKS = 20;

	setupIdentityArray();

	goals = new Goals();
	goals->createNewGoalsGrid(GOALS_EDGE_WALK, new Dimension(X_BLOCKS, Y_BLOCKS, Z_BLOCKS, 0));

	ASSERT_IS_EQUAL(14, goals->X_BLOCKS, "Incorrect number of X Blocks");
	ASSERT_IS_EQUAL(15, goals->Y_BLOCKS, "Incorrect number of Y Blocks");
	ASSERT_IS_EQUAL(20, goals->Z_BLOCKS, "Incorrect number of Z Blocks");

	ASSERT_IS_NOT_NULL(goals->goalList, "Goals list should not be null");
	ASSERT_IS_EQUAL(goals->goalList->getSize(), 0, "Goals list should be empty");

	ASSERT_IS_EQUAL(goals->currentGoalType, GOALS_EDGE_WALK, "Goals list type was not set");

	return 0;
}

const char * GoalsTest::createNewGoals(){

	GoalObject *goal1 = goals->createRandomGoal(_identityArray, allCollections);
	GoalObject *goal2 = goals->createRandomGoal(_identityArray, allCollections);
	GoalObject *goal3 = goals->createRandomGoal(_identityArray, allCollections);
	GoalObject *goal4 = goals->createRandomGoal(_identityArray, allCollections);

	ASSERT_ARE_NOT_NULL(goal1, goal2, goal3, goal4, "At least one of the 4 created goals should not be null");

	return 0;
}


const char * GoalsTest::runGoals(){

	const char * returnValue = 0;

	returnValue = initializeGoals();
	CHECK_ASSERTION;

	returnValue = createNewGoals();
	CHECK_ASSERTION;

	return "No error";
}


JNIEXPORT jstring JNICALL Java_com_mycardboarddreams_jacobsladder_deluxe_drivers_goals_GoalsTest_runGoalsTests(JNIEnv* jenv, jobject obj){

	GoalsTest *goalsTest = new GoalsTest();

	const char *response = goalsTest->runGoals();
	jstring answerString = (jenv)->NewStringUTF(response);
//	free(response);
	return answerString;
}

