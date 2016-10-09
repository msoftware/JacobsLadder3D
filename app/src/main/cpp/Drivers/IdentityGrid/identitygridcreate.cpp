#include "Drivers/IdentityGrid/identitygrid.h"

#define LOG_TAG "Identity Grid"

GLubyte IdentityGrid::X_NEXT[] = {NEAR_SIDE, FAR_SIDE, BOTTOM_SIDE, TOP_SIDE};
GLubyte IdentityGrid::Y_NEXT[] = {LEFT_SIDE, RIGHT_SIDE, NEAR_SIDE, FAR_SIDE};
GLubyte IdentityGrid::Z_NEXT[] = {LEFT_SIDE, RIGHT_SIDE, BOTTOM_SIDE, TOP_SIDE};

IdentityGrid::IdentityGrid() : Dimension()
{
	_identityArray = 0;

	_sliceIsDirty = 0;

	resetStatsVariables();

	resetIdentityAndGoalArrays();

	LOG_INFO_DEBUG("      Created IdentityGrid.");

}

void IdentityGrid::resetSlicesToDirty(){
	for(GLuint i = 0; i < Z_BLOCKS; i++)
		_sliceIsDirty[i] = true;
}

bool IdentityGrid::isReady(){
	return _identityArray != 0;
}


IdentityGrid::~IdentityGrid()
{
	resetIdentityAndGoalArrays();

}

void IdentityGrid::resetIdentityAndGoalArrays()
{
	deleteIdentityGridArray();

	if(_sliceIsDirty){
		delete[] _sliceIsDirty;
		_sliceIsDirty = 0;
	}

	X_BLOCKS = 0;
	Y_BLOCKS = 0;
	Z_BLOCKS = 0;

	X_BLOCKS_MINUS_1 = 0;
	Y_BLOCKS_MINUS_1 = 0;
	Z_BLOCKS_MINUS_1 = 0;
}

void IdentityGrid::deleteIdentityGridArray()
{
	if(_identityArray)
	{
		LOG_INFO_DEBUG("      ---Deleting Identity Grid...");

		for(GLuint x = 0; x < X_BLOCKS; x++){

			for(GLuint y = 0; y < Y_BLOCKS; y++){

				for(GLuint z = 0; z < Z_BLOCKS; z++){
					delete _identityArray[x][y][z];
				}
				delete[] _identityArray[x][y];
			}
			delete[] _identityArray[x];
		}
		delete[] _identityArray;
		_identityArray = 0;

		LOG_INFO_DEBUG("      Finished deleting Identity Grid...");
	}
}


void IdentityGrid::setIdentityToDimensions(Dimension *dimens)
{
	LOG_INFO_DEBUG("   Setting Identity Grid to dimensions (%d, %d, %d)...", dimens->X_BLOCKS, dimens->Y_BLOCKS, dimens->Z_BLOCKS);

	resetIdentityAndGoalArrays();

	X_BLOCKS = dimens->X_BLOCKS;
	Y_BLOCKS = dimens->Y_BLOCKS;
	Z_BLOCKS = dimens->Z_BLOCKS;

	X_BLOCKS_MINUS_1 = X_BLOCKS-1;
	Y_BLOCKS_MINUS_1 = Y_BLOCKS-1;
	Z_BLOCKS_MINUS_1 = Z_BLOCKS-1;

	X_BLOCKS_MID = ((GLfloat)X_BLOCKS)/2.0f;
	Y_BLOCKS_MID = ((GLfloat)Y_BLOCKS)/2.0f;

	_sliceIsDirty = new bool[Z_BLOCKS];

	for(GLuint i = 0; i < Z_BLOCKS; i++)
		_sliceIsDirty[i] = true;

	createIdentityGridArray();

}


void IdentityGrid::createIdentityGridArray()
{
	LOG_INFO_DEBUG("      Creating Identity Grid array...");

	if(_identityArray){
		LOG_ERROR_DEBUG("        Identity array was already defined right before its creation");
		return;
	}

	// define identifiers array-----------------------------
	_identityArray = new Identity***[X_BLOCKS];

	for (GLuint x = 0; x < X_BLOCKS; ++x) {
		_identityArray[x] = new Identity**[Y_BLOCKS];

		for (GLuint y = 0; y < Y_BLOCKS; ++y){
			_identityArray[x][y] = new Identity*[Z_BLOCKS];

			for (GLuint z = 0; z < Z_BLOCKS; ++z){
				_identityArray[x][y][z] = new Identity(0, 0, STATE_NONE, x, y, z);
				if(x == 0)
					_identityArray[x][y][z]->sidesTouchingStaticCubes |= LEFT_SIDE;
				if(x == X_BLOCKS-1)
					_identityArray[x][y][z]->sidesTouchingStaticCubes |= RIGHT_SIDE;
				if(y == 0)
					_identityArray[x][y][z]->sidesTouchingStaticCubes |= BOTTOM_SIDE;
				if(y == Y_BLOCKS-1)
					_identityArray[x][y][z]->sidesTouchingStaticCubes |= TOP_SIDE;
			}
		}
	}

	setupAllIdentityLinks();

	LOG_INFO_DEBUG("      Created Identity Grid array. Size: %5.0f Kb", (X_BLOCKS*Y_BLOCKS*Z_BLOCKS*sizeof(Identity))/1000.0f);

}

void IdentityGrid::setupAllIdentityLinks(){

	Identity *ident;

	for (GLuint i = 0; i < X_BLOCKS; ++i) {

		for (GLuint j = 0; j < Y_BLOCKS; ++j){

			for (GLuint k = 0; k < Z_BLOCKS; ++k){
				ident = _identityArray[i][j][k];
				setAdjacentIdentityRefs(ident, i, j, k);
			}
		}
	}
}


void IdentityGrid::setAdjacentIdentityRefs(Identity *ident, GLuint x, GLuint y, GLuint z){

	if(x > 0)
		ident->leftIdent = getGridElement(x-1, y, z);

	if(x < X_BLOCKS_MINUS_1)
		ident->rightIdent = getGridElement(x+1, y, z);

	if(y > 0)
		ident->downIdent = getGridElement(x, y-1, z);

	if(y < Y_BLOCKS_MINUS_1)
		ident->upIdent = getGridElement(x, y+1, z);

	ident->farIdent = getGridElement(x, y, (z+1) % Z_BLOCKS);

	ident->nearIdent = getGridElement(x, y, (z + Z_BLOCKS_MINUS_1) % Z_BLOCKS);
}


void IdentityGrid::setGoalsAtEdges()
{
	LOG_INFO_DEBUG("     Setting goal grid as *edges*...");

	for(GLuint x = 0; x < X_BLOCKS; x++){

		for(GLuint y = 0; y < Y_BLOCKS; y++){

			//if edge has block, it is candidate, otherwise destination==========
			for(GLuint k = 0; k < Z_BLOCKS; k++)
			{
				Identity *ident = getGridElement(x, y, k);
				if(x == 0 || x == X_BLOCKS_MINUS_1 || y == 0 || y == Y_BLOCKS_MINUS_1)
				{
					if(ident->state == STATE_NONE)
					{
						ident->isBasicGoalPosition = true;
					}
				}

				if(y > x){
					if(x < (Y_BLOCKS - y)){
						ident->pullToWallsDirection = LEFT_SIDE;
						ident->pullAgainstWallsDirection = RIGHT_SIDE;
					}
					else{
						ident->pullToWallsDirection = TOP_SIDE;
						ident->pullAgainstWallsDirection = BOTTOM_SIDE;
					}
				}
				else{
					if((X_BLOCKS - x) <= y){
						ident->pullToWallsDirection = RIGHT_SIDE;
						ident->pullAgainstWallsDirection = LEFT_SIDE;
					}
					else{
						ident->pullToWallsDirection = BOTTOM_SIDE;
						ident->pullAgainstWallsDirection = TOP_SIDE;
					}
				}

				ident->setPullToWallsIdentity();
			}
		}
	}

	LOG_INFO_DEBUG("     Set goal grid as *edges*.");
}


Identity *IdentityGrid::getGridElement(GLuint x, GLuint y, GLuint z)
{
	assert(_identityArray);

	return _identityArray[x][y][z];
}

