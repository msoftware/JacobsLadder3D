#include "Drivers/cubedriver.h"

#define LOG_TAG "Driver"



//========================================================================================
//the primary call point from the engine to set the next driver properties----------------
void CubeDriver::iterateOneFrame(GLuint fractionOfPreferedFrame) {
	assert(_identityGrid != 0);

	if(!isInitialized)
		return;

	static long long nextMoveUnsupported = 0;
	static long long nextMoveSelected = 0;

	iterateMovingCubes(fractionOfPreferedFrame);

	if(++nextMoveUnsupported % CHECK_UNSUPPORTED_INCREMENT == 0)
		allArrayCollections->moveUnsupportedToPriority();

	bool needMoreGoals = !startPriorityCubesMoving();

	goalSet->recycleViolatedGoalObjects(	_identityGrid->_identityArray,
											allArrayCollections,
											allArrayCollections->selectGroup->getSize(),
											needMoreGoals);

	if(++nextMoveSelected % MOVE_SELECTED_INCREMENT == 0)
		moveSelectionToPriority();

	if(allArrayCollections->movingElements->getSize() < MAX_MOVING_AT_A_TIME)
		moveNewCandidatesToSelected();
}


void CubeDriver::iterateMovingCubes(GLuint moveDistance) {
	CubePos *c;

	IterativeCollection<CubePos> *movingCollection = allArrayCollections->movingElements;

	movingCollection->lock();

	movingCollection->resetIterator();

	while(movingCollection->hasNext())
	{
		c = movingCollection->getNext();

		assert(c);

		if(!c->isMoving()){
			allArrayCollections->addCubeToList(c, SELECTED_TO_MOVE_LIST);
		}

		//if finished moving------------------------------
		else if(c->incrementMovement(moveDistance))
		{
			c->updateVariablesAfterMoving();

			if(!tryToGetNextMove(c))
				stopCubeMoving(c);
		}
	}

	movingCollection->unlock();
}


bool CubeDriver::tryToGetNextMove(CubePos *c){
	Identity *ident = c->getGridElement();

	if(_identityGrid->shouldKeepMoving(ident))
	{
		_identityGrid->setAbleToMoveToBits(c);

		if(!c->_canMoveToBits)
			allArrayCollections->addCubeToList(c, SELECTED_TO_MOVE_LIST);
		else if(_identityGrid->findAndSetNewSuggestedDirection(goalSet, c, true)){
			return true;
		}
		else {
			allArrayCollections->addCubeToList(c, PRIORITY_LIST);
		}
	}
	// can stop-----------------------------
	else{
		allArrayCollections->addCubeToList(c, CANDIDATE_LIST);
	}

	return false;
}

void CubeDriver::stopCubeMoving(CubePos *c){

	Identity *landed = c->getGridElement();
	Identity *failedTarget = c->getFailedTarget();

	if(failedTarget)
	{
		goalSet->markTargetAsFailed(failedTarget, c->frustration);
	}

	goalSet->claimTargetAsOccupied(landed);

	c->stopMoving();

	_identityGrid->cubeHasStoppedMoving(landed);

}


void CubeDriver::moveSelectionToPriority(){

	IterativeCollection<CubePos> *select = allArrayCollections->selectGroup;

	GLuint numberOfCubesToMove = (GLuint)((float) select->getSize()*FRACTION_OF_SELECTION_TO_PRIORITY);
	int maxSpacesLeft = MAX_MOVING_AT_A_TIME - allArrayCollections->movingElements->getSize();

	if(maxSpacesLeft < 0)
		return;

	if(maxSpacesLeft < (GLint)numberOfCubesToMove)
		numberOfCubesToMove = (GLuint)maxSpacesLeft;

	CubePos *c;

	select->lock();

	select->resetIterator();

	GLuint i = 0;
	while(select->hasNext() && i < numberOfCubesToMove)
	{
		c = select->getNext();

		assert(c);

		if(_identityGrid->isInGoals(c->getGridElement())){
#ifdef COLLECT_STATS
			selectionWasInGoals++;
#endif
			if(c->getGridElement()->isSupportedFromWall()){
				allArrayCollections->addCubeToList(c, CANDIDATE_LIST);
				continue;
			}
		}

		c = getCubePosFurthestFromWall(c);

		if(c) {
			allArrayCollections->addCubeToList(c, PRIORITY_LIST);
			i++;
		}

	}

	select->unlock();
}

bool CubeDriver::startPriorityCubesMoving(){

	IterativeCollection<CubePos> *priority = allArrayCollections->priorityGroup;
	IterativeCollection<CubePos> *moving = allArrayCollections->movingElements;

	moving->lock();

	CubePos *c;

	if(!moving->hasRoom()){
		LOG_ERROR_DEBUG("Unable to hit priority queue.");
		failedToGetToPriority = true;
		moving->unlock();
		return false;
	}

	//push whole priority cue--------------------------------------------------
	priority->lock();

	priority->resetIterator();

	while(priority->hasNext() && moving->hasRoom())
	{
		c = priority->getNext();

		_identityGrid->setAbleToMoveToBits(c);

		if(c->isInGoal()){
			goalSet->moveOutOfGoal(c);
		}

		if(!c->_canMoveToBits) {
			allArrayCollections->addCubeToList(c, SELECTED_TO_MOVE_LIST);
			c->wasTouched = false;
		} else if(_identityGrid->findAndSetNewSuggestedDirection(goalSet, c, false)) {
			allArrayCollections->addCubeToList(c, MOVING_LIST);
		}
		else{
#ifdef COLLECT_STATS
			priorityCubeCouldNotMove++;
#endif
		}
	}

	priority->unlock();
	moving->unlock();

	return true;
}

void CubeDriver::moveNewCandidatesToSelected()
{
	IterativeCollection<CubePos> *candidate = allArrayCollections->candidateGroup;

	candidate->lock();

	CubePos *c;

	for(GLuint i = 0; i < MAX_RANDOM_MOVE_PER_FRAME; i++)
	{
		c = candidate->getRandomEntry();
		if(c) {
			c = getCubePosFurthestFromWall(c);
		}

		if(c){
			allArrayCollections->addCubeToList(c, SELECTED_TO_MOVE_LIST);
#ifdef COLLECT_STATS
			numCubesMovedToSelection++;
#endif
		}
		else{
#ifdef COLLECT_STATS
			unableToGetCandidate++;
#endif
		}
	}
	candidate->unlock();
}

CubePos *CubeDriver::getCubePosFurthestFromWall(CubePos *c){
	assert(c);

	if(!c|| c->isMoving()){
		return 0;
	}

	Identity *chosenIdent = c->getGridElement();

	if(!chosenIdent->isSupportedFromWall())
		return c;

	GLubyte awayFromWall = chosenIdent->pullAgainstWallsDirection;

	if(!_identityGrid->isInGoals(chosenIdent)){
		Identity *testIdent = chosenIdent->findIdentityRefInDirection(awayFromWall);

		do{
			if(testIdent && testIdent->state == STATE_STATIC)
				c = testIdent->cube;
			else
				break;

			testIdent = testIdent->findIdentityRefInDirection(awayFromWall);

		}
		while(!_identityGrid->isInGoals(testIdent) && awayFromWall == testIdent->pullAgainstWallsDirection);

		return c;
	}

	return 0;
}


bool CubeDriver::tryToPlaceCubeInGrid(CubePos *c, Position *pos)
{
	assert(c);

	if(_identityGrid->placeACube(c, pos)){
		allArrayCollections->addCubeToList(c, CANDIDATE_LIST);
		return true;
	}

	return false;
}

void CubeDriver::deleteACube(CubePos *c)
{
	assert(c);

	if(c && _identityGrid->deleteACube(c)) {
		allArrayCollections->addCubeToList(c, NO_LIST);

		delete c;
	}
}

AggregateStats CubeDriver::getAggregateStats(){
	AggregateStats stats;

	getDriverStats(stats.driverStats);

	goalSet->getGoalStats(stats.goalAggregateStats);

	allArrayCollections->getIterativeCollectionStats(stats.arrayStats);

	_identityGrid->getIdentityGridStats(stats.identityGridStats);

	return stats;
}


void CubeDriver::getDriverStats(DriverStats &stats){
	stats.failedToGetToPriority = failedToGetToPriority;
	stats.numCubesMovedToSelect = numCubesMovedToSelection;
	stats.unableToGetCandidate = unableToGetCandidate;
	stats.priorityCubeCouldNotMove = priorityCubeCouldNotMove;
	stats.selectionWasInGoals = selectionWasInGoals;

	numCubesMovedToSelection = 0;
	unableToGetCandidate = 0;
	priorityCubeCouldNotMove = 0;
	selectionWasInGoals = 0;
	failedToGetToPriority = false;
}

