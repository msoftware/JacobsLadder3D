#include "Stats/statssnapshot.h"

#define LOG_TAG "StatsSnapshot"

bool StatsSnapshot::startRecording(const char* path, CubeDriver *driver, GLuint iterations, GLuint iterationSpan, GLfloat speed){

	for(uint i = 0; i < iterations; i++){

		for(uint j = 0; j < iterationSpan; j++){
			driver->iterateOneFrame(static_cast<GLuint>(fabsf(speed*FLOATING_POINT_PRECISION)));
		}

		if(!writeToFile(path, driver->getAggregateStats(), i == 0))
			return false;
	}
	return true;
}

bool StatsSnapshot::writeToFile(const char* path, AggregateStats stats, bool restart){
	FILE* file;

	if(restart)
		file = fopen(path,"w+");
	else
		file = fopen(path,"a");

	if (file != NULL)
	{
		LOG_INFO_DEBUG("Writing stats file...");
		stats.writeToJson(file);

		fflush(file);
		fclose(file);
		LOG_INFO_DEBUG("Finished writing stats file.");
		return true;
	} else {
		LOG_ERROR_DEBUG("Failed to open stats.txt");
		return false;
	}
}
