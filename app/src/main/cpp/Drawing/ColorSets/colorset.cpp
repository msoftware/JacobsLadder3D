#include <Wallpaper/wallpaperutils.h>
#include "Drawing/ColorSets/colorset.h"

ColorSet *ColorSet::currentColorSet = 0;

#define LOG_TAG "ColorSet"

ColorSet::ColorSet()
{
	distancePaints = 0;

	apparentColors = 0;

	X_BLOCKS_PLUS_1 = 0;
	Y_BLOCKS_PLUS_1 = 0;
	Z_BLOCKS_PLUS_1 = 0;
};

ColorSet::~ColorSet(){
	deleteAllArrays();
};

bool ColorSet::currentSetHasBackground()
{
	return colorSetStruct.hasBackground;
}

GLuint* ColorSet::updateCurrentColorSet(const GLint newColors[LENGTH_OF_CUSTOM_ARRAY])
{
	if(newColors != 0){

		GLuint unsignedColors[LENGTH_OF_CUSTOM_ARRAY];

		for(unsigned int i = 0; i < LENGTH_OF_CUSTOM_ARRAY; i++){
			unsignedColors[i] = static_cast<GLuint>(newColors[i]);
		}

		colorHasRecentlyChanged |= checkRecentChanges(unsignedColors);

		for (unsigned int i = 0; i < LENGTH_OF_CUSTOM_ARRAY; i++) {
			priorColors[i] = unsignedColors[i];
		}

		return setColorArrays(unsignedColors);
	}

	return NULL;
}

bool ColorSet::checkRecentChanges(const GLuint newColors[LENGTH_OF_CUSTOM_ARRAY])
{
	for(unsigned int i = 0; i < LENGTH_OF_CUSTOM_ARRAY; i++){
		if(newColors[i] != priorColors[i])
			return true;
	}
	return false;
}

void ColorSet::clearUpdateFlag() {
	colorHasRecentlyChanged = false;
}



ColorSet *ColorSet::getCurrentColorSet()
{
	if(!currentColorSet){
		LOG_INFO_DEBUG("    Creating new color set.");
		currentColorSet = new ColorSet();
	}

	return currentColorSet;
}

GLuint* ColorSet::setColorArrays(const GLuint newColors[LENGTH_OF_CUSTOM_ARRAY])
{
	setCustomLightList(newColors);

	updateBackground(newColors);

	return setCustomCornerList(newColors);
}

void ColorSet::updateBackground(const GLuint newColors[LENGTH_OF_CUSTOM_ARRAY])
{
	colorSetStruct.hasBackground = newColors[CP_HAS_BACKGROUND] == 1;

	getColorBytesFromInt(newColors[CP_BACKGROUND], colorSetStruct.backgroundColor);

	LOG_INFO_DEBUG("Background: %d, %d, %d, 255  visible %s",
			colorSetStruct.backgroundColor[0],
			colorSetStruct.backgroundColor[1],
			colorSetStruct.backgroundColor[2],
	colorSetStruct.hasBackground ? "true" : "false");
}

GLuint* ColorSet::setCustomCornerList(const GLuint newColors[LENGTH_OF_CUSTOM_ARRAY])
{

	getColorByteArrayFromInt(newColors[CP_LEFT_FACE_TL], true, colorSetStruct.cornerColors[0][0]);
	getColorByteArrayFromInt(newColors[CP_RIGHT_FACE_TL], true, colorSetStruct.cornerColors[0][1]);
	getColorByteArrayFromInt(newColors[CP_TOP_FACE_TL], true, colorSetStruct.cornerColors[0][2]);
	getColorByteArrayFromInt(newColors[CP_BOTTOM_FACE_TL], true, colorSetStruct.cornerColors[0][3]);
	getColorByteArrayFromInt(newColors[CP_NEAR_FACE_TL], true, colorSetStruct.cornerColors[0][4]);

	getColorByteArrayFromInt(newColors[CP_LEFT_FACE_TR], true, colorSetStruct.cornerColors[1][0]);
	getColorByteArrayFromInt(newColors[CP_RIGHT_FACE_TR], true, colorSetStruct.cornerColors[1][1]);
	getColorByteArrayFromInt(newColors[CP_TOP_FACE_TR], true, colorSetStruct.cornerColors[1][2]);
	getColorByteArrayFromInt(newColors[CP_BOTTOM_FACE_TR], true, colorSetStruct.cornerColors[1][3]);
	getColorByteArrayFromInt(newColors[CP_NEAR_FACE_TR], true, colorSetStruct.cornerColors[1][4]);

	getColorByteArrayFromInt(newColors[CP_LEFT_FACE_BL], true, colorSetStruct.cornerColors[2][0]);
	getColorByteArrayFromInt(newColors[CP_RIGHT_FACE_BL], true, colorSetStruct.cornerColors[2][1]);
	getColorByteArrayFromInt(newColors[CP_TOP_FACE_BL], true, colorSetStruct.cornerColors[2][2]);
	getColorByteArrayFromInt(newColors[CP_BOTTOM_FACE_BL], true, colorSetStruct.cornerColors[2][3]);
	getColorByteArrayFromInt(newColors[CP_NEAR_FACE_BL], true, colorSetStruct.cornerColors[2][4]);

	getColorByteArrayFromInt(newColors[CP_LEFT_FACE_BR], true, colorSetStruct.cornerColors[3][0]);
	getColorByteArrayFromInt(newColors[CP_RIGHT_FACE_BR], true, colorSetStruct.cornerColors[3][1]);
	getColorByteArrayFromInt(newColors[CP_TOP_FACE_BR], true, colorSetStruct.cornerColors[3][2]);
	getColorByteArrayFromInt(newColors[CP_BOTTOM_FACE_BR], true, colorSetStruct.cornerColors[3][3]);
	getColorByteArrayFromInt(newColors[CP_NEAR_FACE_BR], true, colorSetStruct.cornerColors[3][4]);

	return applyLightsToFaces(newColors);
}

GLuint* ColorSet::applyLightsToFaces(const GLuint colors[LENGTH_OF_CUSTOM_ARRAY]){
	apparentColors = new GLuint[LENGTH_OF_CUSTOM_ARRAY];

	apparentColors[CP_BACKGROUND] = colors[CP_BACKGROUND];

	apparentColors[CP_LEFT_FACE_TL] = applyLightsToFace(0, 1.0, 0, colors[CP_LEFT_FACE_TL], LEFT_FACE);
	apparentColors[CP_RIGHT_FACE_TL] = applyLightsToFace(0, 1.0, 0, colors[CP_RIGHT_FACE_TL], RIGHT_FACE);
	apparentColors[CP_TOP_FACE_TL] = applyLightsToFace(0, 1.0, 0, colors[CP_TOP_FACE_TL], TOP_FACE);
	apparentColors[CP_BOTTOM_FACE_TL] = applyLightsToFace(0, 1.0, 0, colors[CP_BOTTOM_FACE_TL], BOTTOM_FACE);
	apparentColors[CP_NEAR_FACE_TL] = applyLightsToFace(0, 1.0, 0, colors[CP_NEAR_FACE_TL], NEAR_FACE);

	apparentColors[CP_LEFT_FACE_TR] = applyLightsToFace(1.0, 1.0, 0, colors[CP_LEFT_FACE_TR], LEFT_FACE);
	apparentColors[CP_RIGHT_FACE_TR] = applyLightsToFace(1.0, 1.0, 0, colors[CP_RIGHT_FACE_TR], RIGHT_FACE);
	apparentColors[CP_TOP_FACE_TR] = applyLightsToFace(1.0, 1.0, 0, colors[CP_TOP_FACE_TR], TOP_FACE);
	apparentColors[CP_BOTTOM_FACE_TR] = applyLightsToFace(1.0, 1.0, 0, colors[CP_BOTTOM_FACE_TR], BOTTOM_FACE);
	apparentColors[CP_NEAR_FACE_TR] = applyLightsToFace(1.0, 1.0, 0, colors[CP_NEAR_FACE_TR], NEAR_FACE);

	apparentColors[CP_LEFT_FACE_BL] = applyLightsToFace(0, 0, 0, colors[CP_LEFT_FACE_BL], LEFT_FACE);
	apparentColors[CP_RIGHT_FACE_BL] = applyLightsToFace(0, 0, 0, colors[CP_RIGHT_FACE_BL], RIGHT_FACE);
	apparentColors[CP_TOP_FACE_BL] = applyLightsToFace(0, 0, 0, colors[CP_TOP_FACE_BL], TOP_FACE);
	apparentColors[CP_BOTTOM_FACE_BL] = applyLightsToFace(0, 0, 0, colors[CP_BOTTOM_FACE_BL], BOTTOM_FACE);
	apparentColors[CP_NEAR_FACE_BL] = applyLightsToFace(0, 0, 0, colors[CP_NEAR_FACE_BL], NEAR_FACE);

	apparentColors[CP_LEFT_FACE_BR] = applyLightsToFace(1.0, 0, 0, colors[CP_LEFT_FACE_BR], LEFT_FACE);
	apparentColors[CP_RIGHT_FACE_BR] = applyLightsToFace(1.0, 0, 0, colors[CP_RIGHT_FACE_BR], RIGHT_FACE);
	apparentColors[CP_TOP_FACE_BR] = applyLightsToFace(1.0, 0, 0, colors[CP_TOP_FACE_BR], TOP_FACE);
	apparentColors[CP_BOTTOM_FACE_BR] = applyLightsToFace(1.0, 0, 0, colors[CP_BOTTOM_FACE_BR], BOTTOM_FACE);
	apparentColors[CP_NEAR_FACE_BR] = applyLightsToFace(1.0, 0, 0, colors[CP_NEAR_FACE_BR], NEAR_FACE);

	apparentColors[CP_BOTTOM_LEFT] = colors[CP_BOTTOM_LEFT];
	apparentColors[CP_TOP_RIGHT] = colors[CP_TOP_RIGHT];
	apparentColors[CP_TOP_LEFT] = colors[CP_TOP_LEFT];
	apparentColors[CP_BOTTOM_RIGHT] = colors[CP_BOTTOM_RIGHT];

	apparentColors[CP_PRIMARY_LIGHT] = colors[CP_PRIMARY_LIGHT];
	apparentColors[CP_DISTANT_LIGHT] = colors[CP_DISTANT_LIGHT];
	apparentColors[CP_MID_LIGHT] = colors[CP_MID_LIGHT];

	apparentColors[CP_HAS_BACKGROUND] = colors[CP_HAS_BACKGROUND];

	return apparentColors;
}


void ColorSet::setCustomLightList(const GLuint newColors[LENGTH_OF_CUSTOM_ARRAY])
{
	LOG_INFO_DEBUG("   Retrieving custom color set...");
	Light *lights = ColorSet::colorSetStruct.lights;

	lights[0].assignFromColor(newColors[CP_BOTTOM_LEFT]);
	lights[1].assignFromColor(newColors[CP_TOP_RIGHT]);
	lights[2].assignFromColor(newColors[CP_TOP_LEFT]);
	lights[3].assignFromColor(newColors[CP_BOTTOM_RIGHT]);
	lights[4].assignFromColor(newColors[CP_PRIMARY_LIGHT]);
	lights[5].assignFromColor(newColors[CP_DISTANT_LIGHT]);
	lights[6].assignFromColor(newColors[CP_MID_LIGHT]);

	LOG_INFO_DEBUG("   Custom color set retrieved.");

}

GLuint *ColorSet::getApparentColors() {
	return apparentColors;
}



