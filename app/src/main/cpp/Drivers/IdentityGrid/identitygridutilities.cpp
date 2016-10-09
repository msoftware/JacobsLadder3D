#include "Drivers/IdentityGrid/identitygrid.h"

#define LOG_TAG "Grid Utilities"


GLubyte IdentityGrid::setAbleToMoveToBits(CubePos *c)
{
	GLuint _x = c->x;
	GLuint _y = c->y;

	Identity *adjescentIdent;
	Identity *thisIdent = c->getGridElement();
	GLubyte shouldMoveToInContact = 0x00;
	GLubyte canMoveTo = 0x00;
	GLubyte sidesCurrentlyInContact = thisIdent->sidesInStaticContact();

	if(_x > 0){
		adjescentIdent = thisIdent->leftIdent;
		if(adjescentIdent->isNotBlockedInThisDirection(LEFT_SIDE)){
			canMoveTo |= LEFT_SIDE;
			if ((sidesCurrentlyInContact || adjescentIdent->sidesInStaticContact())
					&& !Utils::areOppositeDirections(sidesCurrentlyInContact, LEFT_SIDE))
				shouldMoveToInContact |= LEFT_SIDE;
		}
	}

	if(_x < X_BLOCKS_MINUS_1){
		adjescentIdent = thisIdent->rightIdent;
		if(adjescentIdent->isNotBlockedInThisDirection(RIGHT_SIDE)){
			canMoveTo |= RIGHT_SIDE;
			if ((sidesCurrentlyInContact || adjescentIdent->sidesInStaticContact())
				&& !Utils::areOppositeDirections(sidesCurrentlyInContact, RIGHT_SIDE))
				shouldMoveToInContact |= RIGHT_SIDE;
		}
	}

	if(_y > 0){
		adjescentIdent = thisIdent->downIdent;
		if(adjescentIdent->isNotBlockedInThisDirection(BOTTOM_SIDE)){
			canMoveTo |= BOTTOM_SIDE;
			if ((sidesCurrentlyInContact || adjescentIdent->sidesInStaticContact())
				&& !Utils::areOppositeDirections(sidesCurrentlyInContact, BOTTOM_SIDE))
				shouldMoveToInContact |= BOTTOM_SIDE;
		}
	}

	if(_y < Y_BLOCKS_MINUS_1){
		adjescentIdent = thisIdent->upIdent;
		if(adjescentIdent->isNotBlockedInThisDirection(TOP_SIDE)){
			canMoveTo |= TOP_SIDE;
			if ((sidesCurrentlyInContact || adjescentIdent->sidesInStaticContact())
				&& !Utils::areOppositeDirections(sidesCurrentlyInContact, TOP_SIDE))
				shouldMoveToInContact |= TOP_SIDE;
		}
	}

	adjescentIdent = thisIdent->nearIdent;
	if(adjescentIdent->isNotBlockedInThisDirection(NEAR_SIDE)){
		canMoveTo |= NEAR_SIDE;
		if ((sidesCurrentlyInContact || adjescentIdent->sidesInStaticContact())
			&& !Utils::areOppositeDirections(sidesCurrentlyInContact, NEAR_SIDE))
			shouldMoveToInContact |= NEAR_SIDE;
	}

	adjescentIdent = thisIdent->farIdent;
	if(adjescentIdent->isNotBlockedInThisDirection(FAR_SIDE)){
		canMoveTo |= FAR_SIDE;
		if ((sidesCurrentlyInContact || adjescentIdent->sidesInStaticContact())
			&& !Utils::areOppositeDirections(sidesCurrentlyInContact, FAR_SIDE))
			shouldMoveToInContact |= FAR_SIDE;
	}

	c->_shouldMoveToBitsInContact = shouldMoveToInContact;
	c->_canMoveToBits = canMoveTo;

	return shouldMoveToInContact;
}


void IdentityGrid::setAdjacentCubesDrawableBitsOnEmpty(Identity *thisIdent)
{
	GLuint _x = thisIdent->x;
	GLuint _y = thisIdent->y;

	if(_x > 0){
		thisIdent->leftIdent->sidesTouchingStaticCubes &= ~RIGHT_SIDE;
		if(_x < X_BLOCKS_MID)
			thisIdent->leftIdent->_drawableStaticBits |= RIGHT_SIDE;
	}

	if(_x < X_BLOCKS_MINUS_1){
		thisIdent->rightIdent->sidesTouchingStaticCubes &= ~LEFT_SIDE;
		if(_x >= X_BLOCKS_MID)
			thisIdent->rightIdent->_drawableStaticBits |= LEFT_SIDE;
	}

	if(_y > 0){
		thisIdent->downIdent->sidesTouchingStaticCubes &= ~TOP_SIDE;
		if(_y < Y_BLOCKS_MID)
			thisIdent->downIdent->_drawableStaticBits |= TOP_SIDE;
	}

	if(_y < Y_BLOCKS_MINUS_1){
		thisIdent->upIdent->sidesTouchingStaticCubes &= ~BOTTOM_SIDE;
		if(_y >= Y_BLOCKS_MID)
			thisIdent->upIdent->_drawableStaticBits |= BOTTOM_SIDE;
	}

	thisIdent->farIdent->_drawableStaticBits |= NEAR_SIDE;
	thisIdent->farIdent->sidesTouchingStaticCubes &= ~NEAR_SIDE;

	thisIdent->nearIdent->sidesTouchingStaticCubes &= ~FAR_SIDE;
}



void IdentityGrid::setDrawableSidesForSelfAndAdjescentLanded(Identity *thisIdent)
{
	GLuint x = thisIdent->x;
	GLuint y = thisIdent->y;

	Identity *adjIdent;

	GLubyte drawOwnSides = 0x00;

	if(x > 0){
		adjIdent = thisIdent->leftIdent;
		adjIdent->sidesTouchingStaticCubes |= RIGHT_SIDE;

		if(x < X_BLOCKS_MID){
			adjIdent->_drawableStaticBits &= ~RIGHT_SIDE;
		}
		else if(adjIdent->isEmptyOrMovingSpot()){
			drawOwnSides |= LEFT_SIDE;
		}
	}
	else
		thisIdent->sidesTouchingStaticCubes |= LEFT_SIDE;

	if(x < X_BLOCKS_MINUS_1){
		adjIdent = thisIdent->rightIdent;
		adjIdent->sidesTouchingStaticCubes |= LEFT_SIDE;

		if(x >= X_BLOCKS_MID){
			adjIdent->_drawableStaticBits &= ~LEFT_SIDE;
		}
		else if(adjIdent->isEmptyOrMovingSpot()){
			drawOwnSides |= RIGHT_SIDE;
		}
	}
	else
		thisIdent->sidesTouchingStaticCubes |= RIGHT_SIDE;

	if(y > 0){
		adjIdent = thisIdent->downIdent;
		adjIdent->sidesTouchingStaticCubes |= TOP_SIDE;

		if(y < Y_BLOCKS_MID){
			adjIdent->_drawableStaticBits &= ~TOP_SIDE;
		}
		else if(adjIdent->isEmptyOrMovingSpot()){
			drawOwnSides |= BOTTOM_SIDE;
		}
	}
	else
		thisIdent->sidesTouchingStaticCubes |= BOTTOM_SIDE;

	if(y < Y_BLOCKS_MINUS_1){
		adjIdent = thisIdent->upIdent;
		adjIdent->sidesTouchingStaticCubes |= BOTTOM_SIDE;

		if(y >= Y_BLOCKS_MID){
			adjIdent->_drawableStaticBits &= ~BOTTOM_SIDE;
		}
		else if(adjIdent->isEmptyOrMovingSpot()){
			drawOwnSides |= TOP_SIDE;
		}
	}
	else
		thisIdent->sidesTouchingStaticCubes |= TOP_SIDE;

	adjIdent = thisIdent->nearIdent;
	adjIdent->sidesTouchingStaticCubes |= FAR_SIDE;

	if(adjIdent->isEmptyOrMovingSpot()){
		drawOwnSides |= NEAR_SIDE;
	}

	adjIdent = thisIdent->farIdent;
	adjIdent->_drawableStaticBits &= ~NEAR_SIDE;

	adjIdent->sidesTouchingStaticCubes |= NEAR_SIDE;


	thisIdent->_drawableStaticBits = drawOwnSides;
}


void IdentityGrid::resetStatsVariables(){

	totalMoveAttempts = 0;
	amountOfFrustration = 0;
	numUnableToMove = 0;
	unableToGiveTarget = 0;
	numRandomizerFailed = 0;
	newlyStartedToMove = 0;
	numFloatingUnableToMove = 0;
	cubeReversedOnItself = 0;
	numFloatingUnableToMoveInRepel = 0;
	settingNewTarget = 0;
	foundAdjacentTarget = 0;
	cantMoveInAnyDirection = 0;
	shouldntMoveInAnyDirection = 0;

	newDirectionLeft = 0;
	newDirectionRight = 0;
	newDirectionTop = 0;
	newDirectionBottom = 0;
	newDirectionNear = 0;
	newDirectionFar = 0;
}


void IdentityGrid::getIdentityGridStats(IdentityStats &stats){
	stats.moveAttempts = totalMoveAttempts;
	stats.frustration = amountOfFrustration;
	stats.numUnableToMove = numUnableToMove;
	stats.unableToGiveTarget = unableToGiveTarget;
	stats.numRandomizerFailed = numRandomizerFailed;
	stats.newlyStartedToMove = newlyStartedToMove;
	stats.numFloatingUnableToMove = numFloatingUnableToMove;
	stats.numFloatingUnableToMoveInRepel = numFloatingUnableToMoveInRepel;
	stats.cubeReversedOnItself = cubeReversedOnItself;
	stats.foundAdjacentTarget = foundAdjacentTarget;
	stats.cantMoveInAnyDirection = cantMoveInAnyDirection;
	stats.shouldntMoveInAnyDirection = shouldntMoveInAnyDirection;

	stats.newDirectionLeft = newDirectionLeft;
	stats.newDirectionRight = newDirectionRight;
	stats.newDirectionTop = newDirectionTop;
	stats.newDirectionBottom = newDirectionBottom;
	stats.newDirectionNear = newDirectionNear;
	stats.newDirectionFar = newDirectionFar;
	stats.settingNewTarget = settingNewTarget;

	unsigned int numAttract = 0;
	unsigned int numStatic = 0;
	unsigned int numCubesInGoal = 0;
	unsigned int numEmptyGoal = 0;
	unsigned int numPaused = 0;
	unsigned int numMoving = 0;
	unsigned int numInvalidState = 0;
	unsigned int numMovingInvalid = 0;

	if(_identityArray)
		for(GLuint x = 0; x < X_BLOCKS; x++)
			for(GLuint y = 0; y < Y_BLOCKS; y++)
				for(GLuint z = 0; z < Z_BLOCKS; z++){
					Identity *ident = _identityArray[x][y][z];
					if(ident->goalNum > 0){
						numAttract++;
						if(ident->cube)
							numCubesInGoal++;
					}
					if(ident->state == STATE_STATIC)
						numStatic++;
					else if(ident->state == STATE_NONE && ident->goalNum > 0){
						numEmptyGoal++;
					}
					else if(ident->state == STATE_MOVING){
						numMoving++;
					}
					else if(ident->state == STATE_PAUSED){
						numPaused++;
					}

					if(!ident->isInValidState()){
						numInvalidState++;
					}

					if(ident->cube && !ident->cube->isMoveStateValid()){
						numMovingInvalid++;
					}
				}

	stats.totalAttractIdentities = numAttract;
	stats.numStaticIdentities = numStatic;
	stats.numCubesInAttractGoalPositions = numCubesInGoal;
	stats.numEmptyGoals = numEmptyGoal;
	stats.numPausedIdentities = numPaused;
	stats.numMovingIdentities = numMoving;
	stats.numInvalidIdents = numInvalidState;
	stats.numMovingInvalid = numMovingInvalid;

	resetStatsVariables();
}


