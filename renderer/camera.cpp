/******************************************************************************************

    >>Input System<<
	>>Keyboard Input Source

    Project: g3DREDUX 

	Lead Programmer: Sebu (Sebastian Szczepanski)
	Programmmers: Sebastian Szczepanski

	Date: 26.04.2002

*******************************************************************************************/

/*
History:

	-> started programming  - Sebu, 26.04.2002
*/


#include "global.h"





void Camera::ModeSet(int mode)
{
	iMode=mode;
}
	

void Camera::LookAtSet(D3DXVECTOR3 *pvLookAt)
{
	vLookAt=*pvLookAt;
}
void Camera::Rotate(float fRotX, float fRotY, float fRotZ) {
   fPitch += fRotX;
   fYaw   += fRotY;
   fRoll  += fRotZ;
   }

void Camera::Translate(float fX, float fY, float fZ) {
   // Entlang der x Achse (rechts vs links)
   vPos.x += vRight.x * fX;
   vPos.y += vRight.y * fX;
   vPos.z += vRight.z * fX;

   // Entlang der y Achse (hoch vs runter)
   vPos.x += vUp.x * fY;
   vPos.y += vUp.y * fY;
   vPos.z += vUp.z * fY;

   // Entlang der z Achse (vor vs zurück)
   vPos.x += vDir.x * fZ;
   vPos.y += vDir.y * fZ;
   vPos.z += vDir.z * fZ;
   } // xKamera_verschieben



void Camera::Create(float fPosX, float fPosY, float  fPosZ,char *Name) {
   fPitch = 0.0f;
   fRoll  = 0.0f;
   fYaw   = 0.0f;
   vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
   vUp    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
   vDir   = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
   vPos   = D3DXVECTOR3(fPosX, fPosY, fPosZ);
   pcName=Name;
   } 



void Camera::Update(void) {
 

	// Werte nur einmal berechnen!
		float fSinYaw   = sinf(fYaw), 
	          fCosYaw   = cosf(fYaw);
		float fSinPitch = sinf(fPitch),
			  fCosPitch = cosf(fPitch);
		float fSinRoll  = sinf(fRoll),
			  fCosRoll  = cosf(fRoll);

		// Igitt: Rotation von Hand:
		vRight.x = fCosYaw*fCosRoll + fSinYaw*fSinPitch*fSinRoll;
		vRight.y = fSinRoll*fCosPitch;
		vRight.z = fCosYaw*fSinPitch*fSinRoll - fSinYaw*fCosRoll;

		vUp.x = fSinYaw*fSinPitch*fCosRoll - fCosYaw*fSinRoll;
		vUp.y = fCosRoll*fCosPitch;
		vUp.z = fSinRoll*fSinYaw + fCosRoll*fCosYaw*fSinPitch;

		vDir.x = fCosPitch*fSinYaw;
		vDir.y = -fSinPitch;
		vDir.z = fCosPitch*fCosYaw;

	   // Erzeuge die View Matrix für Direct3D
		matView._11 = vRight.x;
		matView._21 = vRight.y;
		matView._31 = vRight.z;
		matView._41 = -D3DXVec3Dot(&vPos, &vRight);
		matView._12 = vUp.x;
		matView._22 = vUp.y;
		matView._32 = vUp.z;
		matView._42 = -D3DXVec3Dot(&vPos, &vUp);
		matView._13 = vDir.x;
		matView._23 = vDir.y;
		matView._33 = vDir.z;
		matView._43 = -D3DXVec3Dot(&vPos, &vDir);
		matView._14 = 0.0f;
		matView._24 = 0.0f;
		matView._34 = 0.0f;
		matView._44 = 1.0f; 

   
   
   
} // xKamera_Update

