
#ifndef GOALSTATS_H_
#define GOALSTATS_H_

#include <stdio.h>

#define MAX_GOAL_POINTS 14

enum ReasonForDestruction{
	notDestroyed = 0,
	flattenedOut,
	tooFewTargets,
	frustration,
	tooManyRequests,
	goalIsViolated,
};

struct GoalStats{

public:
	unsigned int id;
	unsigned int filled;
	unsigned int capacity;
	unsigned int numRequests;
	unsigned int numFailed;
	unsigned int filledMovingAverage;
	unsigned int cumulativeFrustration;
	unsigned int expectedTargetCountInCube;
	ReasonForDestruction reasonForDestruction;

	GoalStats() : id(0),
				  filled(0),
				  capacity(0),
				  numRequests(0),
				  numFailed(0),
				  filledMovingAverage(0),
				  cumulativeFrustration(0),
				  expectedTargetCountInCube(0),
				  reasonForDestruction(notDestroyed){

	}

	GoalStats(unsigned int _id, unsigned int _filled, unsigned int _capacity,
			  unsigned int _numRequests, unsigned int _numFailed, unsigned int _filledMovingAverage,
			  unsigned int _cumulativeFrustration, unsigned int _expectedTargetCountInCube,
			  ReasonForDestruction _reasonForDestruction) :
			id(_id),
			filled(_filled),
			capacity(_capacity),
			numRequests(_numRequests),
			numFailed(_numFailed),
			filledMovingAverage(_filledMovingAverage),
			cumulativeFrustration(_cumulativeFrustration),
			expectedTargetCountInCube(_expectedTargetCountInCube),
			reasonForDestruction(_reasonForDestruction){
	}

	void writeToJson(FILE* file){
		char buff[250];
		sprintf(buff, "{id:\"%d\",filled:\"%d\",capacity:\"%d\",numRequests:\"%d\",numFailed:\"%d\","
					"filledMovingAverage:\"%d\",cumulativeFrustration:\"%d\",expectedTargetCountInCube:\"%d\","
					"reasonForDestruction:\"%d\"},",
						id,
						filled,
						capacity,
						numRequests,
						numFailed,
						filledMovingAverage,
						cumulativeFrustration,
						expectedTargetCountInCube,
						reasonForDestruction);

		char *buffAsStdStr = buff;
		fputs(buffAsStdStr, file);
	}
};

struct GoalFrameStats{

	unsigned int claimedAsOccupied;
	unsigned int claimedAsFailed;
	unsigned int noNearbyGoals;
	unsigned int noTargetsForNearbyGoal;
	unsigned int numDeletedGoals;
	unsigned int numCreatedGoals;
	unsigned int numTotalGoals;
	unsigned int goalsMovingAverage;


	void writeToJson(FILE* file){

		char buffGoalValues[240];
		sprintf(buffGoalValues, "frameStats:{"
				"claimedAsOccupied:\"%d\",claimedAsFailed:\"%d\",noNearbyGoals:\"%d\",noTargetsForNearbyGoal:\"%d\",numDeletedGoals:\"%d\",numCreatedGoals:\"%d\","
				"numTotalGoals:\"%d\",goalsMovingAverage:\"%d\""
				"}",
				claimedAsOccupied,
				claimedAsFailed,
				noNearbyGoals,
				noTargetsForNearbyGoal,
				numDeletedGoals,
				numCreatedGoals,
				numTotalGoals,
				goalsMovingAverage);

		char *buffAsStdStr = buffGoalValues;
		fputs(buffAsStdStr, file);
	}

};

struct GoalAggregateStats{

public:
	GoalStats goalStats[MAX_GOAL_POINTS];

	GoalFrameStats frameStats;

	void writeToJson(FILE* file){

		frameStats.writeToJson(file);


		char buffSeparator1[50];
		sprintf(buffSeparator1, ",goalStats:[");
		char *buffAsStdStr = buffSeparator1;
		fputs(buffAsStdStr, file);

		for(int i = 0; i < MAX_GOAL_POINTS; i++){
			goalStats[i].writeToJson(file);
		}

		char buff[5];
		sprintf(buff, "]");
		buffAsStdStr = buff;
		fputs(buffAsStdStr, file);
	}
};

#endif /* GOALSTATS_H_ */
