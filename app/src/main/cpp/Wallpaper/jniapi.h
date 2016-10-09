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
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnCreate(JNIEnv* jenv, jobject obj, jint id);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnStart(JNIEnv* jenv, jobject obj, jint id);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnPause(JNIEnv* jenv, jobject obj, jint id);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnResume(JNIEnv* jenv, jobject obj, jint id);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnStop(JNIEnv* jenv, jobject obj, jint id);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeOnFinish(JNIEnv* jenv, jobject obj, jint id);
    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_baselib_NativeGLWallpaperService_00024NativeGLWallpaperEngine_nativeSetSurface(JNIEnv* jenv, jobject obj, jint id, jobject surface);


    JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnCreate(JNIEnv* jenv, jobject obj, jint id);
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnStart(JNIEnv* jenv, jobject obj, jint id);
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnPause(JNIEnv* jenv, jobject obj, jint id);
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnResume(JNIEnv* jenv, jobject obj, jint id);
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnStop(JNIEnv* jenv, jobject obj, jint id);
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeOnFinish(JNIEnv* jenv, jobject obj, jint id);
	JNIEXPORT void JNICALL Java_com_mycardboarddreams_jacobsladder_view_WallpaperSurface_nativeSetSurface(JNIEnv* jenv, jobject obj, jint id, jobject surface);

};

#endif // JNIAPI_H
