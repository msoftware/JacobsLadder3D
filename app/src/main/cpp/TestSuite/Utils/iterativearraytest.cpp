#include "TestSuite/Utils/iterativearraytest.h"
#include "Utils/iterativecollection.h"

IterativeCollection<CubePos> *array = 0;
jint arraySize = 0;

#define LOG_TAG "IterativeArrayTest"

bool deleteArray()
{
	if(!array)
		return true;

	delete array;
	array = 0;

	return true;
}

void resetArray(int size)
{
	if(!array)
		array = new IterativeCollection<CubePos>(size, MOVING_LIST);
	else{
		array->resetArray(size);
	}
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_setUpIterativeArray(JNIEnv* jenv, jobject obj, jint size)
{
	if(array)
		return true;

	arraySize = size;
	array = new IterativeCollection<CubePos>(arraySize, MOVING_LIST);

	return (array != 0) && (array->getSize() == 0) && (array->hasRoom());
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_tearDownIterativeArray(JNIEnv* jenv, jobject obj)
{
	return deleteArray() && (array == 0);
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_addItemToArray(JNIEnv* jenv, jobject obj)
{
	resetArray(arraySize);

	CubePos * c = new CubePos();

	array->addItem(c);

	bool isFirstItem = array->getFirst() == c;
	bool isOnlyItem = array->getRandomEntry() == c;

	return isFirstItem && isOnlyItem;
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_removeItemFromArray(JNIEnv* jenv, jobject obj)
{
	resetArray(arraySize);

	CubePos * c = new CubePos();

	array->addItem(c);

	bool hasRoom = array->hasRoom();

	CubePos *remCube = array->getRandomEntry();
	bool isRemovedItem = remCube == c;
	array->removeItem(c);

	bool wasRemoved = array->hasRoom() && (array->getSize() == 0);

//	LOG_INFO(">>>>>>>> %d   %p  %p", array->getSize(), remCube, c);

	return hasRoom && isRemovedItem && wasRemoved;
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_overflowArray(JNIEnv* jenv, jobject obj)
{
	resetArray(arraySize);

	for(int i = 0; i < arraySize; i++)
	{
		CubePos *c = new CubePos();
		array->addItem(c);
	}

	bool isFull = !array->hasRoom();

	CubePos *c = new CubePos();
	uint position = array->addItem(c);

	LOG_INFO(">>>>>>>> %d   %d", array->getSize(), arraySize);

	return isFull && (array->getSize() == arraySize) && (position == -1);
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_dontAddDuplicates(JNIEnv* jenv, jobject obj)
{
	resetArray(arraySize);
	CubePos *c = new CubePos();

	array->addItem(c);

	bool wasntAdded = array->addItem(c) == -1;

	return wasntAdded && array->getSize() == 1;

}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_resetArray(JNIEnv* jenv, jobject obj)
{
	resetArray(arraySize);

	return array->hasRoom() && array->getSize() == 0;
}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_getRandomElement(JNIEnv* jenv, jobject obj)
{
	resetArray(arraySize);

	CubePos *c_array[arraySize];
	int numToAdd = 8;

	for(int i = 0; i < numToAdd; i++)
	{
		CubePos *c = new CubePos();
		c_array[i] = c;
		array->addItem(c);
	}

	CubePos *c_random = array->getRandomEntry();

	for(int i = 0; i < numToAdd; i++)
	{
		CubePos *c_check = c_array[i];
		if(c_check == c_random)
			return true;
	}
	return false;

}

JNIEXPORT jboolean JNICALL Java_com_mycardboarddreams_jacobsladder_test_IterativeArrayTests_getRandomElementAndRemove(JNIEnv* jenv, jobject obj)
{
	resetArray(arraySize);

	int numToAdd = 8;

	for(int i = 0; i < numToAdd; i++)
	{
		CubePos *c = new CubePos();
		array->addItem(c);
	}

	CubePos *c_random = array->getRandomEntry();
	array->removeItem(c_random);

	array->resetIterator();

	while(array->hasNext())
	{
		CubePos *c_check = array->getNext();
		if(c_check == c_random)
			return (array->getSize() == numToAdd-1);
	}
	return true;

}
