#include "Drivers/IdentityGrid/identitygrid.h"

bool IdentityGrid::deleteACube(CubePos *c){
	bool ret = false;

	Identity *thisIdent = c->getGridElement();
	setAdjacentCubesDrawableBitsOnEmpty(thisIdent);

	if(c->isMoving()){
		ret = true;
	}
	else
		_sliceIsDirty[c->z] = true;

	c->getGridElement()->setProperties(c, STATE_NONE, NO_SIDE);

	return ret;
}


bool IdentityGrid::placeACube(CubePos *c, Position *pos){

	Identity *ident = getGridElement(pos->x, pos->y, pos->z);

	if(!ident->cube)
	{
		c->placeCubeAt(ident);

		setDrawableSidesForSelfAndAdjescentLanded(ident);

		return true;
	}
	return false;

}

