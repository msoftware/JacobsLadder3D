
#ifndef WALLPAPERGENERATOR_H_
#define WALLPAPERGENERATOR_H_

#include "Wallpaper/basewallpaper.h"


class WallpaperHandler
{
public:
	static BaseWallpaper * getWallpaperForHandle(jint id, jobject obj, JNIEnv* jenv);

	static BaseWallpaper * getNewWallpaper(GLuint id);

};



#endif /* WALLPAPERGENERATOR_H_ */
