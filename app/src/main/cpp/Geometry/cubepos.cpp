#include "cubepos.h"
#include "Wallpaper/wallpaperutils.h"
#include "Utils/utils.h"
#include <stdlib.h>

#define LOG_TAG "CubePos"


CubePos::CubePos() : IterativeCollectionMember()
{

	resetVariables();
}

CubePos::~CubePos(){

}

void CubePos::resetVariables(){

	wasTouched = false;
	moveDirection = 0;
	lastMovedDirection = 0;
	_shouldMoveToBitsInContact = 0;
	_canMoveToBits = 0;
	priorMovedDirection = 0;
	currentJourneyLength = 0;

	diff_destx =
	diff_desty =
	diff_destz = 0;

	frustration = 0;

	isEstablished = true;

	stepMovedFragment = 0;

	currIdentity = 0;
	nextIdentity = 0;
}


void CubePos::setNewDirectionVariables(GLubyte dir)
{
	moveDirection = dir;

	stepMovedFragment = 0;

	nextIdentity = currIdentity->findIdentityRefInDirection(dir);

	nextIdentity->setProperties(0, STATE_MOVING, dir);
	currIdentity->setProperties(this, STATE_MOVING, dir);

	isEstablished = false;
}

bool CubePos::incrementMovement(GLuint increment)
{
	stepMovedFragment += increment;

	//if finished moving------------------------------
	return (stepMovedFragment >= FLOATING_POINT_PRECISION);
}


void CubePos::placeCubeAt(Identity *ident)
{
	resetVariables();

	ident->setProperties(this, STATE_STATIC, NO_SIDE);

	x = ident->x;
	y = ident->y;
	z = ident->z;

	currIdentity = ident;
}

Identity *CubePos::getFailedTarget(){
	if(currIdentity != targetIdentity)
		return targetIdentity;

	return 0;
}

Identity *CubePos::updateVariablesAfterMoving()
{
	lastMovedDirection = moveDirection;
	priorMovedDirection = moveDirection;
	wasTouched = false;

	switch (lastMovedDirection) {
		case RIGHT_SIDE:
			diff_destx++;
			break;
		case LEFT_SIDE:
			diff_destx--;
			break;
		case TOP_SIDE:
			diff_desty++;
			break;
		case BOTTOM_SIDE:
			diff_desty--;
			break;
		case FAR_SIDE:
			diff_destz++;
			break;
		case NEAR_SIDE:
			diff_destz--;
			break;
		default:
			break;
	}

	currentJourneyLength++;

	currIdentity->setProperties(0, STATE_NONE, NO_SIDE);

	currIdentity = currIdentity->findIdentityRefInDirection(lastMovedDirection);

	x = currIdentity->x;
	y = currIdentity->y;
	z = currIdentity->z;

	currIdentity->setProperties(this, STATE_PAUSED, NO_SIDE);

	moveDirection = 0;
	stepMovedFragment = 0;

	return 0;
}


void CubePos::stopMoving(){

	currIdentity->setProperties(this, STATE_STATIC, NO_SIDE);

	isEstablished = true;
	lastMovedDirection = 0;
	wasTouched = false;

	nextIdentity = 0;

	currentJourneyLength = 0;
	frustration = 0;

	diff_destx = 0;
	diff_desty = 0;
	diff_destz = 0;
}

bool CubePos::shouldMoveInThisDirection(GLubyte dir){
	return shouldMoveToInContact(dir) && !Utils::areOppositeDirections(lastMovedDirection, dir);
}


bool CubePos::shouldMoveToInContact(GLubyte dir)
{
	return canMoveInThisDirectionInAPinch(dir) != 0
		   && (_shouldMoveToBitsInContact & dir) != 0
		   && !Utils::areOppositeDirections((_canMoveToBits^ALL_DIR), dir);
}

bool CubePos::canMoveInThisDirectionInAPinch(GLubyte dir)
{
	return (_canMoveToBits & dir) != 0;
}

GLboolean CubePos::isMoving()
{
	return moveDirection;
}

bool CubePos::isJustStartingToMove()
{
	return isEstablished;
}

Identity *CubePos::getGridElement(){
	return currIdentity;
}

void CubePos::isUndermined(){

}

void CubePos::giveTargetIdentity(Identity *target, GLuint MAX_Z){
	nextIdentity = target;
	targetIdentity = target;

	if(target){
		diff_destx = x - target->x;
		diff_desty = y - target->y;
		int d = abs(z - target->z);
		diff_destz = (d < MAX_Z/2) ? (z - target->z) : (z - (MAX_Z + target->z));
	}
	else{
		diff_destx = 0;
		diff_desty = 0;
		diff_destz = 0;
	}

	currentJourneyLength = 0;
	frustration = 0;

}

Identity *CubePos::getNextIdentity(){
	return nextIdentity;
}

bool CubePos::isMoveStateValid(){
	return isMoving() || (
			(((_canMoveToBits & LEFT_SIDE) == 0) ^ (currIdentity->leftIdent && currIdentity->leftIdent->state == STATE_NONE)) &&
			(((_canMoveToBits & RIGHT_SIDE) == 0) ^ (currIdentity->rightIdent && currIdentity->rightIdent->state == STATE_NONE)) &&
			(((_canMoveToBits & TOP_SIDE) == 0) ^ (currIdentity->upIdent && currIdentity->upIdent->state == STATE_NONE)) &&
			(((_canMoveToBits & BOTTOM_SIDE) == 0) ^ (currIdentity->downIdent && currIdentity->downIdent->state == STATE_NONE)) &&
			(((_canMoveToBits & NEAR_SIDE) == 0) ^ (currIdentity->nearIdent && currIdentity->nearIdent->state == STATE_NONE)) &&
			(((_canMoveToBits & FAR_SIDE) == 0) ^ (currIdentity->farIdent && currIdentity->farIdent->state == STATE_NONE))
			);
}


void CubePos::sortDistancesHighestToLowest(CubePos *c, GLubyte (&sorted)[3])
{
	int xAbsoluteLength = abs(c->diff_destx);
	int yAbsoluteLength = abs(c->diff_desty);
	int zAbsoluteLength = abs(c->diff_destz);

	if(xAbsoluteLength < yAbsoluteLength){
		if(xAbsoluteLength < zAbsoluteLength){
			if(yAbsoluteLength < zAbsoluteLength){
				sorted[0]= Z_DIR;
				sorted[1]= Y_DIR;
				sorted[2]= X_DIR;
			}
			else{
				sorted[0]= Y_DIR;
				sorted[1]= Z_DIR;
				sorted[2]= X_DIR;
			}
		}
		else{
			sorted[0]= Y_DIR;
			sorted[1]= X_DIR;
			sorted[2]= Z_DIR;
		}
	}
	else {
		if(yAbsoluteLength < zAbsoluteLength){
			if(xAbsoluteLength < zAbsoluteLength){
				sorted[0]= Z_DIR;
				sorted[1]= X_DIR;
				sorted[2]= Y_DIR;
			}
			else{
				sorted[0]= X_DIR;
				sorted[1]= Z_DIR;
				sorted[2]= Y_DIR;
			}
		}
		else{
			sorted[0]= X_DIR;
			sorted[1]= Y_DIR;
			sorted[2]= Z_DIR;
		}
	}

}

bool CubePos::isInGoal() {
	return currIdentity->goalNum > 0;
}



