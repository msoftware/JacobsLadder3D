#include "TestSuite/CubeDriver/cubedrivertest.h"
#include "Drivers/cubedriver.h"
#include "Stats/statssnapshot.h"

#define LOG_TAG "tags"

static CubeDriver *driver;



JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_deluxe_drivers_CubeDriverTest_createDriverInstance(
		JNIEnv* jenv, jobject obj,
		jstring path, jint xBlocks, jint yBlocks, jint zBlocks, jint iterations, jint iterationSpan, jfloat speed){

	const char* pathAsChars = jenv->GetStringUTFChars(path,0);

	driver = new CubeDriver();
	Dimension *dimens = new Dimension(xBlocks, yBlocks, zBlocks, 4000);
	driver->initializeDimensions(dimens);

	bool completed = StatsSnapshot::startRecording(pathAsChars, driver, iterations, iterationSpan, speed);

	jenv->ReleaseStringUTFChars(path, pathAsChars);

	return static_cast<unsigned char>(completed ? 1U : 0U);
}

