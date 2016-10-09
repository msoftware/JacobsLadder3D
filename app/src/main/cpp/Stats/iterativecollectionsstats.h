
#ifndef ITERATIVECOLLECTIONSTATS_H_
#define ITERATIVECOLLECTIONSTATS_H_

#include "Utils/iterativelisttypes.h"
#include <stdio.h>

struct IterativeCollectionStats{
public:
	AddedToListType type;
	unsigned int capacity;
	unsigned int filled;
	unsigned int movingAverage;

	void writeToJson(FILE *file){

		char buff[200];
		sprintf(buff, "{filled:\"%d\",capacity:\"%d\",movingAverage:\"%d\"},",
				filled,
				capacity,
				movingAverage);
		char *buffAsStdStr = buff;
		fputs(buffAsStdStr, file);

	}
};


#endif /* ITERATIVECOLLECTIONSTATS_H_ */
