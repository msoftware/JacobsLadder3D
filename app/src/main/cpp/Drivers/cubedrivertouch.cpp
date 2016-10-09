#include "Drivers/cubedriver.h"
#include "Drawing/drawing.h"

#define LOG_TAG "Cube Driver Touch"

void CubeDriver::onTouchEvent(GLint x, GLint y, GLfloat xConst, GLfloat yConst, GLuint cameraMovementShift, GLboolean isSingleTap)
{
	GLuint _x = 0, _y = 0;
	Identity *ident;

	if(	!allArrayCollections ||
		!allArrayCollections->movingElements ||
		!allArrayCollections->movingElements->hasRoom())
		return;

	if(!_identityGrid || !_identityGrid->isReady() || !allArrayCollections)
		return;

	for(GLuint z = 0; z < Z_BLOCKS; z++){

		_x = (GLint)((xConst)*(CUBE_SIDE*(GLfloat)z - CAMERA_BACK_OFFSET)) + X_BLOCKS_MID;
		_y = (GLint)((yConst)*(CUBE_SIDE*(GLfloat)z - CAMERA_BACK_OFFSET)) + Y_BLOCKS_MID;

		if(_x < 0 || _y < 0 || _x >= X_BLOCKS || _y >= Y_BLOCKS)
			break;

		ident = _identityGrid->getGridElement(_x, _y, ((z + cameraMovementShift) % Z_BLOCKS));

		CubePos *cube = ident->cube;
		if(cube){
			cube->wasTouched = true;

			if(isSingleTap){
				goalSet->tryToViolateGoalToTouch(ident);
			}
			else if(!cube->isMoving()) {
				allArrayCollections->addCubeToList(cube, PRIORITY_LIST);
				setSurroundFromTouch(ident);
			}
			break;

		}
	}
}

bool CubeDriver::setSurroundFromTouch(Identity *thisIdent)
{
	GLuint 	x = thisIdent->x,
			y = thisIdent->y;
	CubePos *cube;

	if(x > 0){
		cube = thisIdent->leftIdent->cube;
		if(cube && (thisIdent->leftIdent->state == STATE_STATIC)) {
			allArrayCollections->addCubeToList(cube, PRIORITY_LIST);
			cube->wasTouched = true;
		}
	}

	if(x < X_BLOCKS-1){
		cube = thisIdent->rightIdent->cube;
		if(cube && (thisIdent->rightIdent->state == STATE_STATIC)) {
			allArrayCollections->addCubeToList(cube, PRIORITY_LIST);
			cube->wasTouched = true;
		}
	}

	if(y > 0){
		cube = thisIdent->downIdent->cube;
		if(cube && (thisIdent->downIdent->state == STATE_STATIC)) {
			allArrayCollections->addCubeToList(cube, PRIORITY_LIST);
			cube->wasTouched = true;
		}
	}

	if(y < Y_BLOCKS-1){
		cube = thisIdent->upIdent->cube;
		if(cube && (thisIdent->upIdent->state == STATE_STATIC)) {
			allArrayCollections->addCubeToList(cube, PRIORITY_LIST);
			cube->wasTouched = true;
		}
	}

	cube = thisIdent->nearIdent->cube;
	if(cube && (thisIdent->nearIdent->state == STATE_STATIC)) {
		allArrayCollections->addCubeToList(cube, PRIORITY_LIST);
		cube->wasTouched = true;
	}

	cube = thisIdent->farIdent->cube;
	if(cube && (!thisIdent->farIdent->state == STATE_STATIC)) {
		allArrayCollections->addCubeToList(cube, PRIORITY_LIST);
		cube->wasTouched = true;
	}

	return true;
}

