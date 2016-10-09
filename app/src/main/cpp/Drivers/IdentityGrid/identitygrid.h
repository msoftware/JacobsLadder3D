#ifndef IDENTITYGRID_H_
#define IDENTITYGRID_H_

#include "Utils/utils.h"
#include "Wallpaper/wallpaperutils.h"

#include "Geometry/cubepos.h"
#include "Geometry/dimension.h"
#include "Stats/identitystats.h"
#include "Drivers/Goals/goals.h"
#include <math.h>
#include <stdlib.h>

#define MAX_MOVE_TRIES 4


class IdentityGrid : public Dimension
{
private:

	GLuint X_BLOCKS_MINUS_1, Y_BLOCKS_MINUS_1, Z_BLOCKS_MINUS_1;
	GLfloat X_BLOCKS_MID, Y_BLOCKS_MID;

	GLuint numFloatingUnableToMove;
	GLuint numFloatingUnableToMoveInRepel;
	GLuint numUnableToMove;
	GLuint amountOfFrustration;
	GLuint totalMoveAttempts;
	GLuint unableToGiveTarget;
	GLuint newlyStartedToMove;
	GLuint cubeReversedOnItself;
	GLuint settingNewTarget;
	GLuint foundAdjacentTarget;
	GLuint numRandomizerFailed;
	GLuint cantMoveInAnyDirection;
	GLuint shouldntMoveInAnyDirection;

	GLuint newDirectionLeft;
	GLuint newDirectionRight;
	GLuint newDirectionTop;
	GLuint newDirectionBottom;
	GLuint newDirectionNear;
	GLuint newDirectionFar;


	static GLubyte X_NEXT[];
	static GLubyte Y_NEXT[];
	static GLubyte Z_NEXT[];


	/* Instantiation methods */
	void deleteIdentityGridArray();

	void setupAllIdentityLinks();


	/* Utils */
	void setAdjacentCubesDrawableBitsOnEmpty(Identity *thisIdent);

	void setDrawableSidesForSelfAndAdjescentLanded(Identity *thisIdent);

	void setAdjacentIdentityRefs(Identity *ident, GLuint x, GLuint y, GLuint z);

	/* Goal behavior */
	bool tryToSetTarget(Goals * goalSet, CubePos *c);
	bool isAcceptableTarget(Identity *c, Identity *target);

	GLubyte getBestDirectionForCube(CubePos *c, bool isContinuing);

	Identity *findAdjacentTarget(Identity *ident);

	void resetStatsVariables();

public:
	Identity ****_identityArray;

	bool *_sliceIsDirty;

	IdentityGrid();
	~IdentityGrid();

	/* Instantiation methods */
	void createIdentityGridArray();

	void resetIdentityAndGoalArrays();
	void setIdentityToDimensions(Dimension *dimens);

	bool deleteACube(CubePos *c);
	bool placeACube(CubePos *c, Position *pos);

	bool isReady();

	void resetSlicesToDirty();


	/* Cube behavior */
	Identity *getGridElement(GLuint x, GLuint y, GLuint z);

	void cubeHasStoppedMoving(Identity *landedPosition);

	void setACubeAsMoving(CubePos *c, GLubyte dir);

	GLubyte setAbleToMoveToBits(CubePos *c);

	/* Goal behavior */
	void setGoalsAtEdges();

	GLubyte findAndSetNewSuggestedDirection(Goals * goalSet, CubePos *c, bool isContinuing);

	bool shouldKeepMoving(Identity *identity);
	bool isGoodCandidateToMove(CubePos *c);

	bool isInGoals(Identity *ident);

	void getIdentityGridStats(IdentityStats &stats);

};


#endif /* IDENTITYGRID_H_ */
