/******************************************************************************************

    >>Global Header<<

    Project: Picasso 

	Lead Programmer: TalisA (Sebastian Szczepanski)
	Programmmers: Sebastian Szczepanski

	Date: 02.11.2002

*******************************************************************************************/

/*
History:

	-> started programming  - TalisA, 02.11.2002
*/


#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#ifdef RENDERER_EXPORTS
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif

//BASIS HEADERs
#include <windows.h>
#include <windowsx.h>
#include <basetsd.h>
#include <d3dx9.h> 
#include <math.h>
#include <stdio.h>	
#include <mmsystem.h>
#include <fstream.h>
//#include <tchar.h>
#include <assert.h>
#include <d3dfont.h>

#include <vector>


using namespace std;


//ENGINE HEADERs
//#include "DynamicIB.h" //by nVidia
//#include "DynamicVB.h" //by nVidia

#include "log.h"
#include "math.h"
#include "camera.h"
#include "CgFXmanager.h"
#include "frustrum.h"
#include "core.h"



//ADDITIONAL HEADERs





#endif __GLOBAL_H__