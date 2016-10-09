#include "TestSuite/IdentityGrid/identitygridtest.h"
#include "Wallpaper/logger.h"
#include "Drivers/IdentityGrid/identitygrid.h"


#define LOG_TAG "JniApi"

IdentityGrid *grid = 0;

void resetGrid()
{
	if(grid){
		delete grid;
		grid = 0;
	}
	grid = new IdentityGrid();
}

bool identityIsAtEdge(Identity *ident, GLuint x, GLuint y, GLuint z)
{
	return ident->x == 0 || ident->x == x-1 || ident->y == 0 || ident->y == y-1 || ident->z == 0 || ident->z == z-1;
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityGridTests_setUpIdentityGrid(JNIEnv* jenv, jobject obj)
{
	if(grid){
		return true;
	}

	grid = new IdentityGrid();
	return (grid != 0);
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityGridTests_tearDownIdentityGrid(JNIEnv* jenv, jobject obj)
{
	if(grid){
		delete grid;
		grid = 0;
	}

	return (grid == 0);
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IdentityGridTests_resizeIdentityGrid(JNIEnv* jenv, jobject obj, jint x, jint y, jint z)
{
	if(!grid)
		return false;

	grid->setIdentityToDimensions(new Dimension((GLuint)x, (GLuint)y, (GLuint)z, (GLuint)0));

	return (grid->getGridElement(x-1, y-1, z-1) != 0);
}
