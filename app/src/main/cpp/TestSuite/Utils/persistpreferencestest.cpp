#include "TestSuite/Utils/persistpreferencestest.h"
#include "Drawing/drawing.h"

Drawing *prefs = 0;

bool resetPrefs()
{
	if(prefs)
	{
		delete prefs;
		prefs = 0;
	}
	prefs = new Drawing();
	return true;
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_PersistPreferencesTests_setUpPersistPreferences(JNIEnv* jenv, jobject obj)
{
	resetPrefs();
	Dimension *dimens = new Dimension((GLuint)0, (GLuint)0, (GLuint)0, (GLuint)0);
	return prefs != 0;
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_PersistPreferencesTests_checkSimilarDimensionsAndColor(JNIEnv* jenv, jobject obj)
{
	resetPrefs();
	if(!prefs)
		return false;

	return true;
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_PersistPreferencesTests_changedPreferences(JNIEnv* jenv, jobject obj)
{
	resetPrefs();
	if(!prefs)
		return false;
	Dimension *dimens = new Dimension((GLuint)1, (GLuint)3, (GLuint)5, (GLuint)11);

	return true;
}
