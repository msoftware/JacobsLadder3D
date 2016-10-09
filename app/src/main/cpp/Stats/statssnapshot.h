
#ifndef STATSSNAPSHOT_H_
#define STATSSNAPSHOT_H_

#include "Stats/aggregatestats.h"
#include "Drivers/cubedriver.h"

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>


class StatsSnapshot {

public:
	static bool startRecording(const char* path, CubeDriver *driver, GLuint iterations, GLuint iterationSpan, GLfloat speed);

	static bool writeToFile(const char* path, AggregateStats stats, bool restart);

};


#endif
