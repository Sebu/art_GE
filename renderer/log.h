/******************************************************************************************

    >>Picasso LogFile<<
	>>LogFile Header

    Project: Picasso

	Lead Programmer: TalisA (Sebastian Szczepanski)
	Programmmers: Sebastian Szczepanski

	Date: 03.11.2002

*******************************************************************************************/

/*
History:

	-> started programming  - TalisA, 03.11.2002
*/

#ifndef __LOG_H__
#define __LOG_H__

#include "global.h"

#ifdef RENDERER_EXPORTS
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif

//class API_EXPORT ofstream;

class API_EXPORT CLog
{

	ofstream xlog;

	
public:
	CLog(void);
	~CLog(void);
    void Write(const char *fmt, ...);

};


#endif __LOG_H__