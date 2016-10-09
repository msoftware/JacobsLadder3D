
#ifndef ALLITERATIVECOLLECTIONS_H_
#define ALLITERATIVECOLLECTIONS_H_

#include "Geometry/cubepos.h"
#include "Utils/iterativecollection.h"
#include "Stats/iterativecollectionsstats.h"

class AllIterativeCollections {

public:
	IterativeCollection<CubePos> *selectGroup;
	IterativeCollection<CubePos> *priorityGroup;
	IterativeCollection<CubePos> *movingElements;
	IterativeCollection<CubePos> *candidateGroup;

	AllIterativeCollections();
	~AllIterativeCollections();

	void initializeArrays();
	void deleteArrays();
	void revertArrays();

	bool addCubeToList(CubePos *cube, const AddedToListType type);

	void getIterativeCollectionStats(IterativeCollectionStats (&stats)[4]);

	GLuint moveUnsupportedToPriority();

	GLuint moveUnsupportedToPriorityList(IterativeCollection<CubePos> *listToCheck);

private:

	bool doesListHaveRoom(const AddedToListType type);
};


#endif /* ALLITERATIVECOLLECTIONS_H_ */
