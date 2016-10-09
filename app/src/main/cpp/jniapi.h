//
// Copyright 2011 Tero Saarni
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef JNIAPI_H
#define JNIAPI_H

#include <jni.h>

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>

extern "C" {

    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_00024WhiteWallpaperEngine_nativeSetupBlocks(JNIEnv* jenv, jobject obj, jint id, jint x, jint y, jint z, jint total);
    JNIEXPORT jintArray JNICALL Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_00024WhiteWallpaperEngine_nativeSetupCustomColors(JNIEnv* jenv, jobject obj, jint id, jstring setName, jintArray colors);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_00024WhiteWallpaperEngine_nativeSetupSpeed(JNIEnv* jenv, jobject obj, jint id, jint speed);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_00024WhiteWallpaperEngine_nativeSetupAngle(JNIEnv* jenv, jobject obj, jint id, jfloat angle);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_00024WhiteWallpaperEngine_nativeTouchEvent(JNIEnv* jenv, jobject obj, jint id, jfloat x, jfloat y, jboolean isSingleTap = JNI_TRUE);

    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_JacobsLadderService_nativeIsInDebugMode(JNIEnv* jenv, jobject obj, jboolean isDebug);

    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeSetupBlocks(JNIEnv* jenv, jobject obj, jint id, jint x, jint y, jint z, jint total);
	JNIEXPORT jintArray JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeSetupCustomColors(JNIEnv* jenv, jobject obj, jint id, jstring setName, jintArray colors);
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeSetupSpeed(JNIEnv* jenv, jobject obj, jint id, jint speed);

    JNIEXPORT jintArray JNICALL Java_net_margaritov_preference_colorpicker_ColorPickerPreference_nativeGetApparentCustomColors(JNIEnv* jenv, jobject obj);

};

#endif // JNIAPI_H
