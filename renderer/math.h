/******************************************************************************************

    >>Picasso MathLib<<
	>>MathLib Header

    Project: Picasso

	Lead Programmer: TalisA (Sebastian Szczepanski)
	Programmmers: Sebastian Szczepanski

	Date: 02.11.2002

*******************************************************************************************/

/*
History:

	-> started programming  - TalisA, 02.11.2002
*/

#ifndef __MATH_H__
#define __MATH_H__

#include "global.h"

#ifdef RENDERER_EXPORTS
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif

struct API_EXPORT D3DXVECTOR3;
struct API_EXPORT D3DXMATRIX;


struct API_EXPORT CaVertex
{
	float x,y,z, w;
	float fU1,fV1, fU2, fV2;
};


class API_EXPORT CVertex
{
public:
	D3DXVECTOR3 vPos;
	D3DXVECTOR3	vNormal;
	float		fU,fV;
};

class API_EXPORT CPlane
{
public:
	D3DXVECTOR3 vNormal;
	float		fDistance;
	CPlane( const D3DXVECTOR3& a, const D3DXVECTOR3& b, const D3DXVECTOR3& c);
};


class API_EXPORT CPortal
{
public:
	D3DXVECTOR3 p_Points[4];
	int			iToSector;
	int			iCgFXIndex;
};

class API_EXPORT CMesh
{
public:
	int		iNumVertices;
	int		iTriangles;
	int		iNumIndices;
	int		iDrawMode;
	int		iCgFXIndex;
	char    cName[128];

	CVertex *p_Vertices;
	WORD	*p_Indices;
};

class API_EXPORT CEntity
{
public:
	D3DXMATRIX	matWorld;
	D3DXVECTOR3	vPos;

	CMesh		Mesh;

};

class API_EXPORT CSector
{
public:
	int		iPortals;
	int		iEntities;
	int		iAlphaEntities;

	CPortal	*p_Portals;
	CEntity *p_Entities;
	CEntity *p_AlphaEntities;
};

class API_EXPORT CMap
{
public:
	int		iSectors;
	char	cName[128];

	CSector	*p_Sectors;
	
};


class API_EXPORT CTimer
{
	//LARGE_INTEGER	qwTime;

	float		fTimeStart;
	int			iTimeKey;
public:

	float		fTimeElapsed;
	//
	void Update(void);
	//
	void TimeSet(int iTime);
	

};

#endif __MATH_H__