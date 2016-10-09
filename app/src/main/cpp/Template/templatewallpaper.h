#ifndef TEMPLATEWALLPAPER_H_
#define TEMPLATEWALLPAPER_H_

#include "Wallpaper/basewallpaper.h"

class TemplateWallpaper : public BaseWallpaper {

public:
	TemplateWallpaper(GLuint ident) : BaseWallpaper(ident){}

protected:
    void setupCustomOpenGLInThread(GLuint width, GLuint height);

    void renderFrameInThread(GLfloat fractionOfPreferedFrame);
    void iterateFrameInThread(GLfloat fractionOfPreferedFrame);

    void destroyingCustomGLContextInThread();
};


#endif /* TEMPLATEWALLPAPER_H_ */
