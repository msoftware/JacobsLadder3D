#ifndef IDENTITY_CPP_
#define IDENTITY_CPP_

#include "Geometry/cubepos.h"


bool Identity::isEmptyTarget(){
	return (state == STATE_NONE && goalNum > 0);
}


void Identity::setProperties(CubePos *cubeRef, IdentityState identityState, GLubyte movingToDirection)
{
	cube = cubeRef;
	state = identityState;
	movingDirection = movingToDirection;
}

bool Identity::isNotBlockedInThisDirection(GLubyte dir)
{
	return state == STATE_NONE && goalNum >= 0;
}

bool Identity::isEmptyOrMovingSpot()
{
	return state != STATE_STATIC;
}

bool Identity::isInUntenablePosition(){
	return sidesTouchingStaticCubes == 0 || goalNum < 0;
}

bool Identity::isSupportedFromWall(){
	Identity *toWalls = getIdentityInWallsDirection();
	return goalNum >= 0 && (!toWalls || toWalls->state == STATE_STATIC);
}

GLubyte Identity::sidesInStaticContact(){
	return sidesTouchingStaticCubes;
}


GLubyte Identity::getDrawableFaces(){
	return _drawableStaticBits;
}


void Identity::isUndermined(){
	if(!isBasicGoalPosition && goalNum == 0 && cube){
		cube->isUndermined();
		if(leftIdent)
			leftIdent->isUndermined();
		if(rightIdent)
			rightIdent->isUndermined();
		if(upIdent)
			upIdent->isUndermined();
		if(downIdent)
			downIdent->isUndermined();
		if(nearIdent)
			nearIdent->isUndermined();
		if(farIdent)
			farIdent->isUndermined();
	}

}

Identity *Identity::findIdentityRefInDirection(GLubyte dir){
	switch(dir){
		case LEFT_SIDE:
			return leftIdent;
		case RIGHT_SIDE:
			return rightIdent;
		case TOP_SIDE:
			return upIdent;
		case BOTTOM_SIDE:
			return downIdent;
		case FAR_SIDE:
			return farIdent;
		case NEAR_SIDE:
			return nearIdent;
		default:
			return 0;
	}
}

void Identity::setPullToWallsIdentity(){
	pullToWallsIdent = findIdentityRefInDirection(pullToWallsDirection);
}

Identity *Identity::getIdentityInWallsDirection(){
	if(goalNum <= 0)
		return pullToWallsIdent;
	else
		return findIdentityRefInDirection(goalPullToWallsDirection);
}


bool Identity::isInValidState(){
	return (((sidesTouchingStaticCubes & LEFT_SIDE) == 0) ^ (!leftIdent || (leftIdent->cube && leftIdent->state == STATE_STATIC))) &&
			(((sidesTouchingStaticCubes & RIGHT_SIDE) == 0) ^ (!rightIdent || (rightIdent->cube && rightIdent->state == STATE_STATIC))) &&
			(((sidesTouchingStaticCubes & TOP_SIDE) == 0) ^ (!upIdent || (upIdent->cube && upIdent->state == STATE_STATIC))) &&
			(((sidesTouchingStaticCubes & BOTTOM_SIDE) == 0) ^ (!downIdent || (downIdent->cube && downIdent->state == STATE_STATIC))) &&
			(((sidesTouchingStaticCubes & NEAR_SIDE) == 0) ^ (nearIdent->cube && nearIdent->state == STATE_STATIC)) &&
			(((sidesTouchingStaticCubes & FAR_SIDE) == 0) ^ (farIdent->cube && farIdent->state == STATE_STATIC));
}

#endif /* IDENTITY_CPP_ */
