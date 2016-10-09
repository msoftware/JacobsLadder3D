#ifndef CUBEPOS_H_
#define CUBEPOS_H_

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>

#include "Wallpaper/logger.h"
#include "Utils/iterativecollectionmember.h"
#include "Drivers/IdentityGrid/identity.h"

class CubePos : public IterativeCollectionMember<CubePos>{


public:

	CubePos();
	~CubePos();

	GLuint stepMovedFragment;
	GLubyte moveDirection;

	GLuint frustration;
	bool wasTouched;

	GLubyte _shouldMoveToBitsInContact;
	GLubyte _canMoveToBits;

	GLubyte lastMovedDirection;
	GLubyte priorMovedDirection;

	GLint 	diff_destx,
			diff_desty,
			diff_destz;


	/* STATE METHODS */

	void placeCubeAt(Identity *identity);
	Identity *updateVariablesAfterMoving();

	bool incrementMovement(GLuint increment);

	bool shouldMoveInThisDirection(GLubyte dir);
	void setNewDirectionVariables(GLubyte dir);
	void stopMoving();

	void giveTargetIdentity(Identity *target, GLuint MAX_Z);

	Identity *getFailedTarget();


	/* GET PROPERTIES */

	Identity *getGridElement();

	GLboolean isMoving();

	bool shouldMoveToInContact(GLubyte dir);
	bool canMoveInThisDirectionInAPinch(GLubyte dir);

	bool isJustStartingToMove();

	void isUndermined();

	Identity *getNextIdentity();

	static void sortDistancesHighestToLowest(CubePos *c, GLubyte (&sorted)[3]);

	bool isMoveStateValid();

	bool isInGoal();

private:

	void resetVariables();

	GLuint currentJourneyLength;

	bool isEstablished;

	Identity *nextIdentity;
	Identity *targetIdentity;

	Identity *currIdentity;

};



#endif /* CUBEPOS_H_ */
