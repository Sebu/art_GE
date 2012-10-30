/******************************************************************************************

    >>Picasso CgFX Manager<<
	>>CgFX Manager Header

    Project: Picasso

	Lead Programmer: TalisA (Sebastian Szczepanski)
	Programmmers: Sebastian Szczepanski

	Date: 03.11.2002

*******************************************************************************************/

/*
History:

	-> started programming  - TalisA, 09.11.2002
*/

#ifndef __CGFXMANAGER_H__
#define __CGFXMANAGER_H__

#include "global.h"


class API_EXPORT CFX
{
public:
	int						iFrames;
	char				    cName[128];
	LPD3DXEFFECT			pEffect;
	LPDIRECT3DTEXTURE9		*aTextur;
	
};




#endif __CGFXMANAGER_H__