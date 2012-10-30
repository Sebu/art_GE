#ifndef __INPUT_H__
#define __INPUT_H__

class GInput
{
	bool bKeys[256];
public:
	// set a key status
	void KeySet(unsigned int iKey, bool bState);
	// get a key status
	bool KeyGet(unsigned int iKey);
};

#endif __INPUT_H__