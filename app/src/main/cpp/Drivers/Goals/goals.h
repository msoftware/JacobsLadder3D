
#ifndef GOALS_H_
#define GOALS_H_

#include "Drivers/Goals/goalobject.h"
#include "Utils/iterativecollection.h"
#include "Drivers/IdentityGrid/identity.h"
#include "Geometry/dimension.h"
#include "Stats/goalstats.h"

enum GoalType{
	GOALS_NONE = 0,
	GOALS_EDGE_WALK = 1,
};

class GoalsTest;

class Goals : public Dimension{

friend class GoalsTest;

public:

	GoalType currentGoalType;

	IterativeCollection<GoalObject> *goalList;

	Goals();
	~Goals();

	void createNewGoalObjects(Identity ****identity);

	void markTargetAsFailed(Identity *failedTarget, GLuint frustration);

	void deleteGoalGrids();
	void createNewGoalsGrid(GoalType type, Dimension *dimens);

	void recycleViolatedGoalObjects(Identity ****grid, AllIterativeCollections *allCollections, GLuint numSelected, bool needMoreGoals);
	void tryToViolateGoalToTouch(Identity *pos);

	void claimTargetAsOccupied(Identity *ident);

	Identity *getRandomTarget(Identity *pos);

	void getGoalStats(GoalAggregateStats &aggrStats);

	void moveOutOfGoal(CubePos *cube);

	int getNumAvailableTargets();

private:

	/*** Goal stats**/
	GLuint claimedAsOccupied;
	GLuint claimedAsFailed;
	GLuint noNearbyGoals;
	GLuint noTargetsForNearbyGoal;
	GLuint numDeletedGoals;
	GLuint numCreatedGoals;

	GoalStats historyOfStats[MAX_GOAL_POINTS];

	void resetStatsValues();

	GoalObject *getClosestGoal(Identity *pos);

	GoalObject *createRandomGoal(Identity ****grid, AllIterativeCollections *allCollections);

};




#endif /* GOALS_H_ */
