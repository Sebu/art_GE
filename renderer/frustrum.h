/******************************************************************************************

    >>Picasso ViewFrustrum<<
	>>ViewFrustrum Header

    Project: Picasso

	Lead Programmer: TalisA (Sebastian Szczepanski)
	Programmmers: Sebastian Szczepanski

	Date: 03.11.2002

*******************************************************************************************/

/*
History:

	-> started programming  - TalisA, 09.11.2002
*/

#ifndef __FRUSTRUM_H__
#define __FRUSTRUM_H__

#include "global.h"

#ifdef RENDERER_EXPORTS
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif


class API_EXPORT CFrustrum
{
public:
	CPlane pFrustrum[64];
	int	   iPlanes;
	
	// creates ViewFrustum from Viewport
	Frustrum(D3DXMATRIX *matView,D3DXMATRIX *matProj);
	// creates ViewFrustum Position and Portal
	Frustrum(D3DXVECTOR3 vPos);

};



#endif __FRUSTRUM_H__