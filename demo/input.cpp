#include "global.h"

void GInput::KeySet(unsigned int iKey, bool bState)
{ 
	if (iKey < 256) bKeys[iKey] = bState;
}

// get a key status
bool GInput::KeyGet(unsigned int iKey)
{
	return bKeys[iKey];
};
