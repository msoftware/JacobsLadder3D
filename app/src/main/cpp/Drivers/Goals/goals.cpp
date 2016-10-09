#include "Drivers/Goals/goals.h"
#include "Drivers/Goals/goalcuboid.h"
#include "Drivers/Goals/goalline.h"
#include "Wallpaper/wallpaperutils.h"

#define LOG_TAG "Goal Set"

#define RAND_GOAL_DIMENS 2u
#define RAND_GOAL_DIMENS_X 3u
#define RAND_GOAL_DIMENS_Y 4u
#define RAND_GOAL_DIMENS_Z 3u

#define BOTTOM_LEFT_CORNER 0
#define TOP_LEFT_CORNER 1
#define BOTTOM_RIGHT_CORNER 2
#define TOP_RIGHT_CORNER 3

Goals::Goals() : Dimension(){

	goalList = 0;

	currentGoalType = GOALS_NONE;

	resetStatsValues();
}

void Goals::resetStatsValues(){
	claimedAsOccupied = 0;
	claimedAsFailed = 0;
	noNearbyGoals = 0;
	noTargetsForNearbyGoal = 0;
	numDeletedGoals = 0;
	numCreatedGoals = 0;

	for(int i = 0; i < MAX_GOAL_POINTS; i++)
		historyOfStats[i] = GoalStats();

}

Goals::~Goals(){
	deleteGoalGrids();
}


void Goals::createNewGoalsGrid(GoalType type, Dimension *dimens){

	deleteGoalGrids();

	currentGoalType = type;

	setDimension(dimens);

	LOG_INFO_DEBUG("  Creating goals list...");

	goalList = new IterativeCollection<GoalObject>(MAX_GOAL_POINTS + 20, GOAL_LIST);

	LOG_INFO_DEBUG("  Created goals list.");
}

void Goals::createNewGoalObjects(Identity ****identity){

	if(!goalList)
		return;

	GoalObject *centerLine = new GoalLine(
					X_BLOCKS/2u, Y_BLOCKS/2u, 3, 3,
					X_BLOCKS, Y_BLOCKS, Z_BLOCKS,
					Z_DIR, REPEL, 0);

	centerLine->populateIdentityGrid(identity);
	goalList->addItem(centerLine);

	GoalObject *g;

	int perSide = MAX_GOAL_POINTS/2 - 1;
	int z_span = Z_BLOCKS/perSide;

	for(unsigned int i = 0; i < perSide; i++){
		g = new GoalLine(
							0, i*z_span,
							3, 2,
							X_BLOCKS, Y_BLOCKS, Z_BLOCKS,
							Y_DIR, ATTRACT, LEFT_SIDE);

		g->populateIdentityGrid(identity);
		goalList->addItem(g);
	}
	for(unsigned int i = 0; i < perSide; i++){
		g = new GoalLine(
							X_BLOCKS, i*z_span,
							3, 2,
							X_BLOCKS, Y_BLOCKS, Z_BLOCKS,
							Y_DIR, ATTRACT, RIGHT_SIDE);

		g->populateIdentityGrid(identity);
		goalList->addItem(g);
	}
}

void Goals::tryToViolateGoalToTouch(Identity *pos){
	goalList->resetIterator();

	while(goalList->hasNext()){
		GoalObject *goalObject = goalList->getNext();

		if(goalObject->getGoalType() == ATTRACT &&
			!(goalObject->isViolated) &&
			goalObject->containsPoint(pos) &&
			goalObject->isUpForDestruction()){

			LOG_INFO_DEBUG("Found goal object to violate.");
			goalObject->isViolated = true;
		}
	}
}

int Goals::getNumAvailableTargets(){
	goalList->resetIterator();
	int numAvailable = 0;

	while(goalList->hasNext()){
		numAvailable += goalList->getNext()->getNumAvailable();
	}

	return numAvailable;

}

void Goals::recycleViolatedGoalObjects(Identity ****grid, AllIterativeCollections *allCollections,
									   GLuint numSelected, bool needMoreGoals){

	goalList->resetIterator();

	int i = 0;

	while(goalList->hasNext()){

		GoalObject *g = goalList->getNext();

		ReasonForDestruction reasonForDestr = g->getReasonForDestruction(numSelected);

		if(reasonForDestr != notDestroyed){

#ifdef COLLECT_STATS
			numDeletedGoals++;

			g->getGoalStats(historyOfStats[i]);
			historyOfStats[i].reasonForDestruction = reasonForDestr;
#endif

			g->removeFromIdentityGrid(allCollections, reasonForDestr);

			goalList->removeItem(g);

			delete g;

			int goalsDeficit = MAX_GOAL_POINTS - goalList->getSize();

			if(goalsDeficit <= 0 && goalsDeficit > -5 && needMoreGoals){
				createRandomGoal(grid, allCollections);
			}

			for(int j = 0; j < goalsDeficit; j++){
				createRandomGoal(grid, allCollections);
			}

			break;
		}

		i++;
	}
}

GoalObject *Goals::createRandomGoal(Identity ****grid, AllIterativeCollections *allCollections){

	GoalObject *g;

	unsigned short x_pos = 0, y_pos = 0;
	unsigned short x_diff = 0, y_diff = 0;
	GLubyte pullToWalls = 0;

	switch((GLubyte)(RANDOM(4))){
		case BOTTOM_LEFT_CORNER:
			x_pos = x_diff = (GLubyte)(RANDOM(X_BLOCKS/2));
			y_pos = y_diff = (GLubyte)(RANDOM(Y_BLOCKS/3));
			pullToWalls = BOTTOM_SIDE;
			break;
		case TOP_LEFT_CORNER:
			x_pos = x_diff = (GLubyte)(RANDOM(X_BLOCKS/2));
			y_diff = (GLubyte)(RANDOM(Y_BLOCKS/3));
			y_pos = Y_BLOCKS - y_diff;
			pullToWalls = TOP_SIDE;
			break;
		case BOTTOM_RIGHT_CORNER:
			x_diff = (GLubyte)(RANDOM(X_BLOCKS/2));
			x_pos = X_BLOCKS - x_diff;
			y_pos = y_diff = (GLubyte)(RANDOM(Y_BLOCKS/3));
			pullToWalls = BOTTOM_SIDE;
			break;
		case TOP_RIGHT_CORNER:
			x_diff = (GLubyte)(RANDOM(X_BLOCKS/2));
			x_pos = X_BLOCKS - x_diff;
			y_diff = (GLubyte)(RANDOM(Y_BLOCKS/3));
			y_pos = Y_BLOCKS - y_diff;
			pullToWalls = TOP_SIDE;
			break;
		default:
			//TODO ?
			break;
	}

	unsigned short z_pos = RANDOM(Z_BLOCKS);

	g = new GoalCuboid(
			x_pos, y_pos, z_pos,
			(GLuint)(RANDOM(RAND_GOAL_DIMENS)) + RAND_GOAL_DIMENS_X,
			(GLuint)(RANDOM(RAND_GOAL_DIMENS)) + RAND_GOAL_DIMENS_Y,
			(GLuint)(RANDOM(RAND_GOAL_DIMENS)) + RAND_GOAL_DIMENS_Z,
			X_BLOCKS, Y_BLOCKS, Z_BLOCKS,
			ATTRACT, pullToWalls);

	bool hasEnoughTargets = g->populateIdentityGrid(grid);

	if(hasEnoughTargets){
		goalList->addItem(g);

#ifdef COLLECT_STATS
		numCreatedGoals++;
#endif

		return g;
	}
	else{
		g->removeFromIdentityGrid(allCollections, tooFewTargets);
		delete g;
		g = 0;
	}

	return 0;
}

void Goals::claimTargetAsOccupied(Identity *ident){
	if(!ident)
		return;

#ifdef COLLECT_STATS
	claimedAsOccupied++;
#endif

	goalList->resetIterator();

	while(goalList->hasNext()){
		if(goalList->getNext()->targetWasOccupied(ident))
			break;
	}
}

void Goals::markTargetAsFailed(Identity *failedTarget, GLuint frustration){
	if(!failedTarget)
		return;

#ifdef COLLECT_STATS
	claimedAsFailed++;
#endif

	goalList->resetIterator();

	while(goalList->hasNext()){
		if(goalList->getNext()->targetWasFailed(failedTarget, frustration))
			break;
	}
}

void Goals::moveOutOfGoal(CubePos *cube) {
	assert(cube);

	goalList->resetIterator();

	while(goalList->hasNext()){
		GoalObject *g = goalList->getNext();
		g->allowAsTarget(cube->getGridElement());
	}

}

Identity *Goals::getRandomTarget(Identity *pos){
	GoalObject *nearestGoal = getClosestGoal(pos);
	if(!nearestGoal){
#ifdef COLLECT_STATS
		noNearbyGoals++;
#endif
		return 0;
	}

	Identity *target = nearestGoal->getRandomTarget();
	if(!target){
#ifdef COLLECT_STATS
		noTargetsForNearbyGoal++;
#endif
		return 0;
	}

	Identity *testToWalls = target;

	testToWalls = testToWalls->getIdentityInWallsDirection();
	if(testToWalls && testToWalls->goalNum > 0 && testToWalls->state != STATE_NONE){
		return testToWalls;
	}

	return target;
}

GoalObject *Goals::getClosestGoal(Identity *pos){
	GLint closestDistance = INT_MAX;
	GoalObject *nearestGoal = 0;

	goalList->resetIterator();

	while(goalList->hasNext()){

		GoalObject *g = goalList->getNext();
		GLint thisDist = g->distanceToCenterPoint(pos);
		if(thisDist < closestDistance){
			closestDistance = thisDist;
			nearestGoal = g;
		}
	}

	return nearestGoal;
}

void Goals::deleteGoalGrids(){

	if(goalList){
		LOG_INFO_DEBUG("   Deleting Goal arrays...");

		goalList->resetIterator();

		while(goalList->hasNext()){
			GoalObject *g = goalList->getNext();
			delete g;
		}

		delete goalList;

		goalList = 0;

		LOG_INFO_DEBUG("   Deleted goal arrays.");
	}

	currentGoalType = GOALS_NONE;

}


void Goals::getGoalStats(GoalAggregateStats &aggrStats){
	static GLfloat movingFilledAverage = 0;

	aggrStats.frameStats.claimedAsFailed = claimedAsFailed;
	aggrStats.frameStats.claimedAsOccupied = claimedAsOccupied;
	aggrStats.frameStats.noNearbyGoals = noNearbyGoals;
	aggrStats.frameStats.noTargetsForNearbyGoal = noTargetsForNearbyGoal;
	aggrStats.frameStats.numCreatedGoals = numCreatedGoals;
	aggrStats.frameStats.numDeletedGoals = numDeletedGoals;
	aggrStats.frameStats.numTotalGoals = goalList->getSize();

	GLuint i = 0;

	goalList->resetIterator();

	while(goalList->hasNext()){
		GoalObject *g = goalList->getNext();
		g->getGoalStats(aggrStats.goalStats[i]);

		if(historyOfStats[i].id > 0){
			aggrStats.goalStats[i] = historyOfStats[i];
			movingFilledAverage = movingFilledAverage*0.9f + historyOfStats[i].filled*0.1f;
		}

		i++;
	}
	while(i < MAX_GOAL_POINTS){
		aggrStats.goalStats[i] = GoalStats();
		i++;
	}

	aggrStats.frameStats.goalsMovingAverage = static_cast<unsigned int>(movingFilledAverage);

	resetStatsValues();
}


