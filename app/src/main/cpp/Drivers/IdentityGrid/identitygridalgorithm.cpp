#include "Drivers/IdentityGrid/identitygrid.h"

#define LOG_TAG "Identity Grid Algorithm"


GLubyte IdentityGrid::findAndSetNewSuggestedDirection(Goals *const goals, CubePos *const c, bool isContinuing)
{
#ifdef COLLECT_STATS
	totalMoveAttempts++;
#endif

	if(!tryToSetTarget(goals, c)){
#ifdef COLLECT_STATS
		unableToGiveTarget++;
#endif
		return 0;
	}

	GLubyte dir = getBestDirectionForCube(c, isContinuing);

	if(dir){

#ifdef COLLECT_STATS

		switch(dir){
		case LEFT_SIDE:
			newDirectionLeft++;
			break;
		case RIGHT_SIDE:
			newDirectionRight++;
			break;
		case TOP_SIDE:
			newDirectionTop++;
			break;
		case BOTTOM_SIDE:
			newDirectionBottom++;
			break;
		case NEAR_SIDE:
			newDirectionNear++;
			break;
		case FAR_SIDE:
			newDirectionFar++;
			break;
		}
#endif
		setACubeAsMoving(c, dir);
	}
	else{
#ifdef COLLECT_STATS

		numUnableToMove++;
		if(!isContinuing && c->getGridElement()->isInUntenablePosition()){
			if(c->getGridElement()->goalNum < 0)
				numFloatingUnableToMoveInRepel++;
			else
				numFloatingUnableToMove++;
		}

#endif
	}

	return dir;
}

bool IdentityGrid::tryToSetTarget(Goals *const  goals, CubePos *const c){

	if(!c->getNextIdentity()){
		Identity *ident = c->getGridElement();

		Identity *suggestTarget = findAdjacentTarget(ident);

		if(!suggestTarget) {
			suggestTarget = goals->getRandomTarget(ident);
			if(!suggestTarget || !isAcceptableTarget(ident, suggestTarget))
				return false;
		}
#ifdef COLLECT_STATS
		else{
			foundAdjacentTarget++;
		}
#endif

		c->giveTargetIdentity(suggestTarget, Z_BLOCKS);
#ifdef COLLECT_STATS
		settingNewTarget++;
#endif
	}
	return true;
}

bool IdentityGrid::isAcceptableTarget(Identity *ident, Identity *target){

	return !Utils::areOppositeDirections(target->pullToWallsDirection, ident->pullToWallsDirection);
}

GLubyte IdentityGrid::getBestDirectionForCube(CubePos *const c, bool isContinuing){

	Identity *thisIdent = c->getGridElement();
	GLubyte ptwDir = thisIdent->pullToWallsDirection;
	GLubyte targetPtwDir = c->getNextIdentity()->pullToWallsDirection;

	if(!isContinuing){
		if(thisIdent->isInUntenablePosition() && (c->_canMoveToBits & ptwDir) != 0){
			return ptwDir;
		}
	}

	if(ptwDir != targetPtwDir && (c->_canMoveToBits & targetPtwDir) != 0){
		return targetPtwDir;
	}

	GLubyte sorted[3] = {0, 0, 0};
	CubePos::sortDistancesHighestToLowest(c, sorted);

	for(GLuint i = 0; i < 3; i++){
		GLubyte checkdirTwo = sorted[i];
		GLubyte checkdirS = 0;

		if(!checkdirTwo)
			break;

		if(checkdirTwo == X_DIR){
			if(c->diff_destx != 0)
			checkdirS = c->diff_destx > 0 ? LEFT_SIDE : RIGHT_SIDE;
		}
		else if(checkdirTwo == Y_DIR){
			if(c->diff_desty != 0)
			checkdirS = c->diff_desty > 0 ? BOTTOM_SIDE : TOP_SIDE;
		}
		else if(checkdirTwo == Z_DIR){
			if(c->diff_destz != 0)
			checkdirS = c->diff_destz > 0 ? NEAR_SIDE : FAR_SIDE;
		}

		if(!checkdirS)
			break;

		if(c->shouldMoveInThisDirection(checkdirS)){
			return checkdirS;
		}
	}

	c->frustration++;
#ifdef COLLECT_STATS
	amountOfFrustration++;
#endif


	if(c->frustration > 5){

#ifdef COLLECT_STATS
		if(!c->_canMoveToBits)
			cantMoveInAnyDirection++;
		if(!c->_shouldMoveToBitsInContact)
			shouldntMoveInAnyDirection++;
#endif
		return 0;
	}

	GLubyte checkdir = FAR_SIDE;
	GLubyte bestdir = sorted[0];

	if(!bestdir)
		return 0;

	for(GLubyte tries = 0; tries < MAX_MOVE_TRIES; tries++)
	{
		if(bestdir & X_DIR){
			checkdir = X_NEXT[(GLubyte)(RANDOM(4))];
		}
		else if(bestdir & Y_DIR){
			checkdir = Y_NEXT[(GLubyte)(RANDOM(4))];
		}
		else if(bestdir & Z_DIR){
			checkdir = Z_NEXT[(GLubyte)(RANDOM(4))];
		}

		if(c->shouldMoveInThisDirection(checkdir)){
			return checkdir;
		}
	}

#ifdef COLLECT_STATS
	numRandomizerFailed++;
#endif

	return 0;
}


void IdentityGrid::cubeHasStoppedMoving(Identity *landedPosition)
{
	setDrawableSidesForSelfAndAdjescentLanded(landedPosition);

	_sliceIsDirty[landedPosition->z] = true;
}


void IdentityGrid::setACubeAsMoving(CubePos *c, GLubyte dir) {

	assert(c);

	if (c->isJustStartingToMove()){
#ifdef COLLECT_STATS
		newlyStartedToMove++;
#endif

		Identity *thisIdent = c->getGridElement();

		setAdjacentCubesDrawableBitsOnEmpty(thisIdent);

		_sliceIsDirty[c->z] = true;
		if(thisIdent->farIdent->state == STATE_STATIC)
			_sliceIsDirty[(c->z + 1) % Z_BLOCKS] = true;
	}

#ifdef COLLECT_STATS
	if(dir == c->priorMovedDirection)
		cubeReversedOnItself++;
#endif

	c->setNewDirectionVariables(dir);
}


bool IdentityGrid::shouldKeepMoving(Identity *ident)
{
	assert(ident);
	return ident->isInUntenablePosition() || !(isInGoals(ident));
}

bool IdentityGrid::isInGoals(Identity *ident){
	return ident && ident->goalNum > 0;
}


bool IdentityGrid::isGoodCandidateToMove(CubePos *c){
	if(!c || c->isMoving()) return false;

	Identity *identity = c->getGridElement();

	return identity->isInUntenablePosition() ||
			!(isInGoals(identity)) ||
			findAdjacentTarget(identity);

}

Identity *IdentityGrid::findAdjacentTarget(Identity *ident){
	if(!ident)
		return 0;

	Identity *toWalls = ident->getIdentityInWallsDirection();
	if(toWalls && toWalls->isEmptyTarget())
		return toWalls;

	if(ident->leftIdent && ident->leftIdent->isEmptyTarget())
		return ident->leftIdent;

	if(ident->rightIdent && ident->rightIdent->isEmptyTarget())
		return ident->rightIdent;

	if(ident->upIdent && ident->upIdent->isEmptyTarget())
		return ident->upIdent;

	if(ident->downIdent && ident->downIdent->isEmptyTarget())
		return ident->downIdent;

	if(ident->nearIdent && ident->nearIdent->isEmptyTarget())
		return ident->nearIdent;

	if(ident->farIdent && ident->farIdent->isEmptyTarget())
		return ident->farIdent;

	return 0;
}
