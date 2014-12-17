#include "XUtilsMath.h"

#include <stdlib.h>

int XUtilsMath::getRand(int randMin, int randMax)
{
	int u = (int)rand() / (RAND_MAX + 1) * (randMax - randMin) + randMin;
	return u;
}



XUtilsMath::XUtilsMath()
{
}


XUtilsMath::~XUtilsMath()
{
}
