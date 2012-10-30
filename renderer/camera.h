/******************************************************************************************

    >>Picasso Camera<<
	>>Camera Header

    Project: Picasso

	Lead Programmer: TalisA (Sebastian Szczepanski)
	Programmmers: Sebastian Szczepanski

	Date: 03.11.2002

*******************************************************************************************/

/*
History:

	-> started programming  - TalisA, 04.11.2002
*/

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "global.h"

#define CULL_FRUSTRUM	0x00000000L
#define CULL_OCCLUSION	0x00000001L

#ifdef RENDERER_EXPORTS
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif


class API_EXPORT Camera
{
	float	fYaw,
			fPitch,
			fRoll,
			fDistance;

	int iMode;
	
public:
	D3DXVECTOR3 vPos,    //Position
				vUp,     //UP Vector
				vRight,  //RIGHT Vector
				vDir,    //DIRECTION Vector
				vLookAt; //LookAt Vector
	D3DXMATRIX	matView;
	char	*pcName;
	//
	void Create(float fPosX, float fPosY, float  fPosZ,char *Name);
	//
	void ModeSet(int mode);
	//
	void Update(void);
	//um Ursprung drehen
	void Rotate(float fRotX, float fRotY, float fRotZ);
	//entlang der Richtungsvectoren bewegen 
	void Translate(float fX, float fY, float fZ);
	//setzt LookAt Vector
	void LookAtSet(D3DXVECTOR3 *pvLookAt);
	//setzt Distance zu LookAt für Follow Cam
	void DistanceSet(float fDistance);
	//
	void MatrixGet();
};

#endif __CAMERA_H__