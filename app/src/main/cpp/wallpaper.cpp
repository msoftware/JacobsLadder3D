#include <pthread.h>
#include <android/native_window.h> // requires ndk r5 or newer
#include <Stats/statssnapshot.h>

#include "wallpaper.h"


//#define TAKE_LIVE_STATS false

#define LOG_TAG "Wallpaper"

Wallpaper::Wallpaper(GLuint id) : BaseWallpaper(id)
{
	cubeDriver = new CubeDriver();
	drawing = new Drawing();
	angleOfRotation = 0.0f;
	cubeMoveFraction = 0.0f;

	objectBufferVersion = 0;
}

Wallpaper::~Wallpaper()
{
	delete cubeDriver;
	delete drawing;
}

bool Wallpaper::isInitialized()
{
	return ObjectBuffers::getObjectBuffers()->isBufferReady() && cubeDriver->isDriverInitialized();
}

bool Wallpaper::resetSize(Dimension *dimens)
{
	bool driverWasInit = cubeDriver->initializeDimensions(dimens);
	bool engineWasInit = ObjectBuffers::getObjectBuffers()->tryRederiveVertices(dimens);

	return driverWasInit || engineWasInit;
}

GLuint* Wallpaper::updateCurrentColorSet(GLint customColors[LENGTH_OF_CUSTOM_ARRAY])
{
	GLuint* adjustedArray = ColorSet::getCurrentColorSet()->updateCurrentColorSet(customColors);

	cubeDriver->shouldRefresh = true;

	return adjustedArray;
}

//END LIFE CYCLE======================================
void Wallpaper::setBlocks(Dimension *dimens)
{
	pthread_mutex_lock(&_mutex_rendering);

	LOG_INFO_DEBUG("  Setting blocks to (%d, %d, %d).... (%d)", dimens->X_BLOCKS, dimens->Y_BLOCKS, dimens->Z_BLOCKS, wallpaper_id);

	resetSize(dimens);
	int bufferVersion = ObjectBuffers::getObjectBuffers()->getObjectBufferVersion();

	if(bufferVersion != objectBufferVersion) {
		_msg_renderer = MSG_RESET_SCENE_VBOS;
		objectBufferVersion = bufferVersion;
	}

	pthread_mutex_unlock(&_mutex_rendering);
}


void Wallpaper::setSpeed(GLint speed)
{
	pthread_mutex_lock(&_mutex_rendering);

	LOG_INFO_DEBUG("  Changing speed to %d.... (%d)", speed, wallpaper_id);

	static GLuint cubeMoveIterations = 8;

	cubeMoveIterations = 11u-speed;

	cubeMoveFraction = 1.0f/(cubeMoveIterations);

	LOG_INFO_DEBUG("  Finished changing speed to %d. (%d)", speed, wallpaper_id);

	pthread_mutex_unlock(&_mutex_rendering);
}

void Wallpaper::setAngle(GLfloat angle)
{
	pthread_mutex_lock(&_mutex_rendering);

	angleOfRotation = angle;

	pthread_mutex_unlock(&_mutex_rendering);
}

GLuint* Wallpaper::setCustomColorScheme(GLint *colorSet)
{
	START_TIMER

	pthread_mutex_lock(&_mutex_rendering);

	LOG_INFO_DEBUG("  Setting colors in engine to set: Custom Colors (%d)", wallpaper_id);

	GLuint* adjustedArray = updateCurrentColorSet(colorSet);

	pthread_cond_signal(&_condition_renderer);

	pthread_mutex_unlock(&_mutex_rendering);

	END_TIMER(Timer::SET_COLORS)

	return adjustedArray;
}

void Wallpaper::touchEvent(GLfloat x, GLfloat y, GLboolean isSingleTap)
{
	GLfloat xConst = ((x - _screenWidthHalf)/(_screenWidthHalf / _screenRatio))/NEAR_FIELD;
	GLfloat yConst = ((-y + _screenHeightHalf)/(_screenHeightHalf / ZOOM_FACTOR))/NEAR_FIELD;

	pthread_mutex_lock(&_mutex_driver);

	cubeDriver->onTouchEvent(static_cast<GLint>(x), static_cast<GLint>(y), xConst, yConst, drawing->cameraMovementShift, isSingleTap);

	pthread_mutex_unlock(&_mutex_driver);
}

void Wallpaper::setupCustomOpenGLInThread(GLuint width, GLuint height)
{
	if(!Constants::RENDER_OPENGL)
		return;

	if(width == 0 || height == 0) return;

	Utils::checkGlError("Setting up drawing... (%d)", wallpaper_id);


    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);


	Utils::checkGlError("after setups", wallpaper_id);


    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustumf(-_screenRatio, _screenRatio, -ZOOM_FACTOR, ZOOM_FACTOR, NEAR_FIELD, 2*(cubeDriver->Z_BLOCKS));
    Utils::checkGlError("after projection", wallpaper_id);

	//SURFACING----------------------------
	glFrontFace(GL_CCW);
	glShadeModel(GL_FLAT);
    glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	Utils::checkGlError("after surfacing", wallpaper_id);


	//DEPTH--------------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glDepthRangef(0.0f, 1.0f);
	Utils::checkGlError("after depth", wallpaper_id);


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	//set up rendering-----------------------------
	glMatrixMode(GL_MODELVIEW);

	LOG_INFO_DEBUG("    GL set up : height - %d width - %d Zoom factor %f (%d)", height, width, ZOOM_FACTOR, wallpaper_id);

	ObjectBuffers::getObjectBuffers()->bindVerticesToBuffersInThread();
	LOG_INFO_DEBUG("   Finished creating custom GL context (%d)", wallpaper_id);
}


void Wallpaper::renderFrameInThread(GLfloat fractionOfPreferredFrame)
{
	if(isInitialized())
	{
		cubeDriver->resetIfNecessary();

		drawing->drawCubes(fractionOfPreferredFrame,
						   angleOfRotation,
						   cubeDriver->_identityGrid,
						   cubeDriver->allArrayCollections->movingElements,
						   cubeDriver->goalSet);
	}
}

void Wallpaper::iterateFrameInThread(GLfloat fractionOfPreferredFrame)
{
	if(isInitialized())
	{

#ifdef TAKE_LIVE_STATS
		if(TAKE_LIVE_STATS) {
			static int iteration = 0;
			//TODO the path should use getExternalStorageDirectory()
			if ((iteration % 30) == 0)
				StatsSnapshot::writeToFile("/storage/emulated/0/stats.txt",
										   cubeDriver->getAggregateStats(), iteration == 0);
			iteration++;
		}
#endif

		GLuint moveDistance = static_cast<GLuint>(cubeMoveFraction*fractionOfPreferredFrame*FLOATING_POINT_PRECISION);
		cubeDriver->iterateOneFrame(moveDistance);
	}
}


void Wallpaper::destroyingCustomGLContextInThread()
{
	LOG_INFO_DEBUG(" Destroying custom GL context (%d)", wallpaper_id);
	//TODO this should unbind if there are no wallpapers
	//ObjectBuffers::getObjectBuffers()->unbindVertexBuffers();
}

