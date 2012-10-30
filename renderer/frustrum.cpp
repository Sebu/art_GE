/******************************************************************************************

    >>Picasso ViewFrustrum<<
	>>ViewFrustrum 

    Project: Picasso

	Lead Programmer: TalisA (Sebastian Szczepanski)
	Programmmers: Sebastian Szczepanski

	Date: 03.11.2002

*******************************************************************************************/

/*
History:

	-> started programming  - TalisA, 09.11.2002
*/

#include "global.h"


CFrustrum::Frustrum(D3DXMATRIX *matView,D3DXMATRIX *matProj)
{
   D3DXMATRIX matViewProj;

   D3DXMatrixMultiply(&matViewProj,matView,matProj);
   

   // Linke Clipping Plane
   pFrustrum[0].vNormal.x = -(matViewProj._14 + matViewProj._11);
   pFrustrum[0].vNormal.y = -(matViewProj._24 + matViewProj._21);
   pFrustrum[0].vNormal.z = -(matViewProj._34 + matViewProj._31);
   pFrustrum[0].fDistance = -(matViewProj._44 + matViewProj._41);
   
   // Rechte Clipping Plane
   pFrustrum[1].vNormal.x = -(matViewProj._14 - matViewProj._11);
   pFrustrum[1].vNormal.y = -(matViewProj._24 - matViewProj._21);
   pFrustrum[1].vNormal.z = -(matViewProj._34 - matViewProj._31);
   pFrustrum[1].fDistance = -(matViewProj._44 - matViewProj._41);
   
   // Obere Clipping Plane
   pFrustrum[2].vNormal.x = -(matViewProj._14-matViewProj._12);
   pFrustrum[2].vNormal.y = -(matViewProj._24-matViewProj._22);
   pFrustrum[2].vNormal.z = -(matViewProj._34-matViewProj._32);
   pFrustrum[2].fDistance  = -(matViewProj._44-matViewProj._42);
   
   // Untere Clipping Plane
   pFrustrum[3].vNormal.x = -(matViewProj._14+matViewProj._12);
   pFrustrum[3].vNormal.y = -(matViewProj._24+matViewProj._22);
   pFrustrum[3].vNormal.z = -(matViewProj._34+matViewProj._32);
   pFrustrum[3].fDistance  = -(matViewProj._44+matViewProj._42);
   
   
   // Normalenvektoren der Ebenen normalisieren
   for (int i=0; i<4; i++) {
      float fBetrag = 1.0f / 
                      sqrtf(D3DXVec3Dot(&pFrustrum[i].vNormal,
                                               &pFrustrum[i].vNormal));
      pFrustrum[i].vNormal.x *= fBetrag;
      pFrustrum[i].vNormal.y *= fBetrag;
      pFrustrum[i].vNormal.z *= fBetrag;
      pFrustrum[i].fDistance  *= fBetrag;
      } 
};