
#ifndef DRIVERSTATS_H_
#define DRIVERSTATS_H_

#include <stdio.h>

struct DriverStats {
public:
	bool failedToGetToPriority;
	unsigned int numCubesMovedToSelect;
	unsigned int unableToGetCandidate;
	unsigned int priorityCubeCouldNotMove;
	unsigned int selectionWasInGoals;


	void writeToJson(FILE *file){
		char buffStart[250];
		sprintf(buffStart, "{failedToGetToPriority:\"%s\",numCubesMovedToSelect:\"%d\",unableToGetCandidate:\"%d\","
				"priorityCubeCouldNotMove:\"%d\",selectionWasInGoals:\"%d\"}",
				failedToGetToPriority ? "true" : "false",
				numCubesMovedToSelect,
				unableToGetCandidate,
				priorityCubeCouldNotMove,
				selectionWasInGoals);
		fputs(buffStart, file);
	}

};



#endif /* DRIVERSTATS_H_ */
