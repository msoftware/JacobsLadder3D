
#ifndef AGGREGATESTATS_H_
#define AGGREGATESTATS_H_

#include "Stats/iterativecollectionsstats.h"
#include "Stats/goalstats.h"
#include "Stats/driverstats.h"
#include "Stats/identitystats.h"
#include <stdio.h>


struct AggregateStats {

public:
	DriverStats driverStats;

	GoalAggregateStats goalAggregateStats;

	IterativeCollectionStats arrayStats[4];

	IdentityStats identityGridStats;

	void writeToJson(FILE *file){

		char buffStart[50];
		sprintf(buffStart, "{arrayStats:[");
		fputs(buffStart, file);

		for(int i = 0; i < 4; i++){
			arrayStats[i].writeToJson(file);
		}

		char buffSeparator[50];
		sprintf(buffSeparator, "],goalAggregateStats:{");
		fputs(buffSeparator, file);


		goalAggregateStats.writeToJson(file);

		char buffIdentity[30];
		sprintf(buffIdentity, "},identityGridStats:");
		fputs(buffIdentity, file);

		identityGridStats.writeToJson(file);

		char buffDriver[30];
		sprintf(buffDriver, ",driverStats:");
		fputs(buffDriver, file);

		driverStats.writeToJson(file);

		char buff[10];
		sprintf(buff, "}\r\n");
		fputs(buff, file);
	}

};



#endif /* AGGREGATESTATS_H_ */
