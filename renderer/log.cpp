/******************************************************************************************

    >>Picasso LogFile<<
	>>LogFile

    Project: Picasso

	Lead Programmer: TalisA (Sebastian Szczepanski)
	Programmmers: Sebastian Szczepanski

	Date: 03.11.2002

*******************************************************************************************/

/*
History:

	-> started programming  - TalisA, 03.11.2002
*/

#include "global.h"

CLog::CLog(void)
{
	xlog.open("picasso_log.html", ios::trunc);
	
//	xlog.setbuf(NULL, NULL);

	xlog << "<HTML><style type=text/css>"
		 <<	".base{font-size:9;font-family:Verdana;color:#FFFFFF;background-color:#8AA2AE;border-color:#016599;border-width:0;border-style:solid;}"
		 << "</STYLE><TABLE WIDTH=100% CLASS=base ALIGN=LEFT VALIGN=TOP>";
};

CLog::~CLog(void)
{
	xlog << "</HTML>";
};

void CLog::Write(const char *fmt, ...)	
{
	char		text[1024];									
	va_list		ap;											// Zeiger auf Argumentliste

	if (fmt == NULL)										// wenn kein text 
		return;												// abbrechen
	va_start(ap, fmt);										// sucht nach variablen
	    vsprintf(text, fmt, ap);							// und ersetzt sie
	va_end(ap);												// text

	xlog << "<TR VALIGN=TOP><TD>" << text << "</TD></TR>";


};