#include "TestSuite/CubeDriver/Goals/goalobjecttest.h"
#include "Drivers/Goals/goalpointtype.h"
#include "Drivers/Goals/goalcuboid.h"
#include "Drivers/Goals/goalplane.h"
#include "Drivers/Goals/goalline.h"
#include "Drivers/IdentityGrid/identitygrid.h"
#include "TestSuite/testutils.h"
#include "Utils/alliterativecollections.h"

#define LOG_TAG "GoalObject Test"

#define CHECK_ASSERTION if(returnValue)return returnValue;

enum GoalObjectTests{
	GoalTypesValid=1,
	NoTargetsInitially,
	IsGrounded,
	CanPopulateGrid,
	NumberOfTargets,
	DistanceToIdent,
	HasRandomTarget,
	TargetIsInDimensions,
	ZOverlapIsInTargets,
	ShouldntBeReplaced,
	PendingRequests,
};

AllIterativeCollections *allCollections = 0;
Identity ****_identityArray = 0;

GoalCuboid *goalCuboid = 0;
GoalPlane *goalPlane = 0;
GoalLine *goalLine = 0;

int X_BLOCKS, Y_BLOCKS, Z_BLOCKS;

void setup(){

	_identityArray = new Identity***[X_BLOCKS];

	for (int i = 0; i < X_BLOCKS; ++i) {
		_identityArray[i] = new Identity**[Y_BLOCKS];

		for (int j = 0; j < Y_BLOCKS; ++j){
			_identityArray[i][j] = new Identity*[Z_BLOCKS];

			for (int k = 0; k < Z_BLOCKS; ++k){
				_identityArray[i][j][k] = new Identity(0, 0, STATE_NONE, i, j, k);

				Identity *ident = _identityArray[i][j][k];
				if(j > i){
					if(i < (Y_BLOCKS - j)){
						ident->pullToWallsDirection = LEFT_SIDE;
						ident->pullAgainstWallsDirection = RIGHT_SIDE;
					}
					else{
						ident->pullToWallsDirection = TOP_SIDE;
						ident->pullAgainstWallsDirection = BOTTOM_SIDE;
					}
				}
				else{
					if((X_BLOCKS - i) <= j){
						ident->pullToWallsDirection = RIGHT_SIDE;
						ident->pullAgainstWallsDirection = LEFT_SIDE;
					}
					else{
						ident->pullToWallsDirection = BOTTOM_SIDE;
						ident->pullAgainstWallsDirection = TOP_SIDE;
					}
				}

				ident->setPullToWallsIdentity();
			}
		}
	}

	allCollections = new AllIterativeCollections();

	goalCuboid = new GoalCuboid(0, 0, 0, 10, 10, 10, X_BLOCKS, Y_BLOCKS, Z_BLOCKS, ATTRACT, BOTTOM_SIDE);

	goalPlane = new GoalPlane(5, Z_DIR, X_BLOCKS, Y_BLOCKS, Z_BLOCKS, ATTRACT, LEFT_SIDE);

	goalLine= new GoalLine(15, 5, 4, 4, X_BLOCKS, Y_BLOCKS, Z_BLOCKS, Y_DIR, ATTRACT, TOP_SIDE);

}

void cleanup(){
	if(_identityArray){
		for(GLint x = 0; x < X_BLOCKS; x++){

			for(GLint y = 0; y < Y_BLOCKS; y++){

				for(GLint z = 0; z < Z_BLOCKS; z++){
					delete _identityArray[x][y][z];
				}
				delete[] _identityArray[x][y];
			}
			delete[] _identityArray[x];
		}
		delete[] _identityArray;
		_identityArray = 0;
	}
}

const char * unpopulatedCuboidSetup(){

	ASSERT_IS_EQUAL(goalCuboid->getGoalType(), ATTRACT, "Cuboid goal type should be ATTRACT");

	Identity *ident = goalCuboid->getRandomTarget();

	ASSERT_IS_NULL(ident,"Cuboid shouldn't have random target");
	ASSERT_TRUE(goalCuboid->isGrounded(),"Cuboid should be grounded");

	return 0;
}

const char * populatedCuboidSetup(){

	bool couldPopulate = goalCuboid->populateIdentityGrid(_identityArray);

	ASSERT_TRUE(couldPopulate,"Cuboid should be able to populate");
	ASSERT_IS_EQUAL(1000, goalCuboid->availableTargets->getSize(), "Cuboid should have 1000 useful targets");
	ASSERT_IS_EQUAL(0, goalCuboid->getPendingRequests(), "Cuboid shouldn't have pending requests");
	ASSERT_FALSE(goalCuboid->getReasonForDestruction(200) != notDestroyed,"Cuboid shouldn't be replaceable");

	return 0;
}

const char * checkDistanceAndDimensions(){

	Identity *checkDistance = _identityArray[5][5][Z_BLOCKS-5];
	int distance = goalCuboid->distanceToCenterPoint(checkDistance);
	ASSERT_IS_EQUAL(distance,15,"Distance to Cuboid should be 15");

	checkDistance = _identityArray[5][Y_BLOCKS-1][5];
	distance = goalCuboid->distanceToCenterPoint(checkDistance);
	ASSERT_IS_EQUAL(distance,INT_MAX,"Distance to Cuboid should be Infinity");


	Identity *ident = goalCuboid->getRandomTarget();
	ASSERT_IS_EQUAL(1, goalCuboid->getPendingRequests(), "Cuboid should have 1 pending request");

	ASSERT_IS_NOT_NULL(ident,"Cuboid should have random target");

	ASSERT_LESS_THAN(ident->x,10,"Cuboid should be inside dimensions");
	ASSERT_GREATER_THAN(ident->x,-10,"Cuboid should be inside dimensions");
	ASSERT_LESS_THAN(ident->y,10,"Cuboid should be inside dimensions");
	ASSERT_GREATER_THAN(ident->y,-10,"Cuboid should be inside dimensions");
	ASSERT_LESS_THAN(ident->z,10,"Cuboid should be inside dimensions");
	ASSERT_GREATER_THAN(ident->z,-10,"Cuboid should be inside dimensions");

	return 0;
}

const char * changesInVariables(){

	Identity *ident1 = goalCuboid->getRandomTarget();
	ASSERT_IS_EQUAL(2, goalCuboid->getPendingRequests(), "Cuboid should have 2 pending requests");
	goalCuboid->targetWasOccupied(ident1);
	ASSERT_IS_EQUAL(1, goalCuboid->getPendingRequests(), "Cuboid should have 1 pending requests");
	ASSERT_IS_EQUAL(999, goalCuboid->availableTargets->getSize(), "Cuboid should have one less useful target");

	Identity *ident2 = goalCuboid->getRandomTarget();
	goalCuboid->getRandomTarget();
	ASSERT_IS_EQUAL(3, goalCuboid->getPendingRequests(), "Cuboid should have 3 pending requests");
	goalCuboid->targetWasFailed(ident2, 0);
	ASSERT_IS_EQUAL(2, goalCuboid->getPendingRequests(), "Cuboid should have 2 pending request");
	ASSERT_IS_EQUAL(999, goalCuboid->availableTargets->getSize(), "Cuboid should have 999 useful targets");
	goalCuboid->targetWasFailed(ident2, 0);
	ASSERT_IS_EQUAL(999, goalCuboid->availableTargets->getSize(), "Cuboid should have 999 useful targets");

	return 0;
}


const char * failedToCreate(){

	GoalCuboid *unusedCube = new GoalCuboid(5, 5, 5, 2, 2, 2, X_BLOCKS, Y_BLOCKS, Z_BLOCKS, ATTRACT, BOTTOM_SIDE);

	bool couldPopulate = unusedCube->populateIdentityGrid(_identityArray);

	ASSERT_FALSE(couldPopulate,"Small cuboid should not be able to populate");

	Identity *checkContains = _identityArray[7][7][7];
	ASSERT_TRUE(goalCuboid->containsPoint(checkContains),"Cuboid should contain point");

	return 0;
}

const char * failedToCreateFloating(){

	GoalCuboid *unusedCube = new GoalCuboid(5, 5, 14, 2, 2, 2, X_BLOCKS, Y_BLOCKS, Z_BLOCKS, ATTRACT, BOTTOM_SIDE);

	bool couldPopulate = unusedCube->populateIdentityGrid(_identityArray);

	ASSERT_FALSE(couldPopulate,"Floating cuboid should not be able to populate");

	return 0;
}


const char * checkCuboidContains(){
	Identity *checkContains = _identityArray[7][7][7];

	ASSERT_TRUE(goalCuboid->containsPoint(checkContains),"Cuboid should contain point");
	return 0;
}

const char * unpopulatedPlane(){

	ASSERT_IS_EQUAL(goalPlane->getGoalType(), ATTRACT, "Plane Goal type should be ATTRACT");
	ASSERT_TRUE(goalPlane->isGrounded(),"Plane should be grounded");

	return 0;
}

const char * populatedPlane(){

	bool couldPopulate = goalPlane->populateIdentityGrid(_identityArray);

	ASSERT_TRUE(couldPopulate,"Should be able to populate Plane");
	ASSERT_IS_EQUAL(X_BLOCKS*Y_BLOCKS - 100, goalPlane->availableTargets->getSize(), "Plane should have 183 useful targets");

	return 0;
}

const char * checkPlaneContains(){

	Identity *checkContains = _identityArray[12][12][5];

	ASSERT_TRUE(goalPlane->containsPoint(checkContains),"Plane should contain point");
	ASSERT_TRUE(goalPlane->containsPoint(checkContains),"Plane should contain point");

	checkContains = _identityArray[5][5][5];

	ASSERT_FALSE(goalPlane->containsPoint(checkContains),"Plane shouldn't contain point");
	return 0;
}

const char * unpopulatedLine(){

	ASSERT_IS_EQUAL(goalLine->getGoalType(), ATTRACT, "Line goal type should be ATTRACT");
	ASSERT_TRUE(goalLine->isGrounded(),"Line should be grounded");

	return 0;
}

const char * populatedLine(){

	bool couldPopulate = goalLine->populateIdentityGrid(_identityArray);

	ASSERT_TRUE(couldPopulate,"Line should be able to populate");
	ASSERT_IS_EQUAL(6*(7*Y_BLOCKS), goalLine->availableTargets->getSize(), "Line should have useful targets");

	return 0;
}

const char * checkLineContains(){
	Identity *checkContains = _identityArray[16][7][6];

	ASSERT_TRUE(goalLine->containsPoint(checkContains),"Line should contain point");

	checkContains = _identityArray[16][5][5];

	ASSERT_FALSE(goalLine->containsPoint(checkContains),"Line shouldn't contain point");

	return 0;
}

const char * removeCuboid(){

	goalCuboid->removeFromIdentityGrid(allCollections, goalIsViolated);

	Identity *checkRemoved = _identityArray[7][7][7];

	ASSERT_TRUE(checkRemoved->addedToList == NO_LIST,"Identity should be removed from target (type)");
	ASSERT_TRUE(checkRemoved->goalNum == 0, "Identity should be neutral");

	return 0;
}

const char * runGoals(jint x_blocks, jint y_blocks, jint z_blocks){
	X_BLOCKS = x_blocks;
	Y_BLOCKS = y_blocks;
	Z_BLOCKS = z_blocks;

	setup();

	/* ADD GOAL CUBOID */
	const char *returnValue = "No error";

	returnValue = unpopulatedCuboidSetup();
	CHECK_ASSERTION;

	returnValue = populatedCuboidSetup();
	CHECK_ASSERTION;

	returnValue = checkDistanceAndDimensions();
	CHECK_ASSERTION;

	returnValue = changesInVariables();
	CHECK_ASSERTION;

	returnValue = checkCuboidContains();
	CHECK_ASSERTION;

	returnValue = failedToCreate();
	CHECK_ASSERTION;

	returnValue = failedToCreateFloating();
	CHECK_ASSERTION;


	/* ADD GOAL PLANE */
	returnValue = unpopulatedPlane();
	CHECK_ASSERTION;

	returnValue = populatedPlane();
	CHECK_ASSERTION;

	returnValue = checkPlaneContains();
	CHECK_ASSERTION;

	/* ADD GOAL LINE */

	returnValue = unpopulatedLine();
	CHECK_ASSERTION;

	returnValue = populatedLine();
	CHECK_ASSERTION;

	returnValue = checkLineContains();
	CHECK_ASSERTION;

	returnValue = removeCuboid();
	CHECK_ASSERTION;

	cleanup();

	return "No error";
}

JNIEXPORT jstring JNICALL Java_com_mycardboarddreams_jacobsladder_deluxe_drivers_goals_GoalObjectTest_runGoalObjectTests(
		JNIEnv* jenv, jobject obj, jint x_blocks, jint y_blocks, jint z_blocks){

	const char *response = runGoals(x_blocks, y_blocks, z_blocks);
	jstring answerString = (jenv)->NewStringUTF(response);
//	free(response);
	return answerString;
}

