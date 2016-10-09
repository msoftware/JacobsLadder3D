#include "Wallpaper/wallpaperhandler.h"
#include "wallpaper.h"

BaseWallpaper * WallpaperHandler::getNewWallpaper(GLuint ident){
	return new Wallpaper(ident);
}
