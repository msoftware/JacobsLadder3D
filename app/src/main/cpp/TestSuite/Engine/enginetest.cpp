#include "TestSuite/Engine/enginetest.h"
#include "Drawing/drawing.h"
#include "Drawing/textures.h"

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_EngineTests_generateTexture(JNIEnv* jenv, jobject obj)
{
	Textures *textures = new Textures();

	unsigned char** textureData = textures->getTextureData();
	return textureData != 0;
}
