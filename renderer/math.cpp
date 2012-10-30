/******************************************************************************************

    >>Picasso MathLib<<
	>>MathLib

    Project: Picasso

	Lead Programmer: TalisA (Sebastian Szczepanski)
	Programmmers: Sebastian Szczepanski

	Date: 02.11.2002

*******************************************************************************************/

/*
History:

	-> started programming  - TalisA, 02.11.2002
*/

#include "global.h"


inline CPlane::CPlane( const D3DXVECTOR3& a, const D3DXVECTOR3& b, const D3DXVECTOR3& c)
{
	D3DXVec3Cross(&vNormal,&(b-a),&(c-a));
	D3DXVec3Normalize(&vNormal,&vNormal);
	fDistance = -D3DXVec3Dot(&vNormal,&a);
};

void CTimer::TimeSet(int iTime)
{
	iTimeKey=iTime;
};


void CTimer::Update()
{
	fTimeElapsed=((float)iTimeKey-fTimeStart)*1;
	fTimeStart=(float)iTimeKey;
};