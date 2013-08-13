/*****************************************************
*
*    Anderain Lovelace Just Port BCE for fx-9860
*
******************************************************/

#include <fxlib.h>
#include <string.h>
#include "dconsole.h"
#include "include/bce.h"

typedef unsigned int uint;
typedef unsigned char uchar;


int AddIn_main(int isAppli, unsigned short OptionNum)
{
	char * buf[512];
	char * p;
	puts("Balance CE-fx\n       port:Anderain");
	puts("--------------------");
	for (;;)
	{
		gets(buf,512);
		if (!strchr(buf,'='))
			continue;
		p = automatic_balance_ce(buf);
		if (p==NULL)
			puts("Error");
		else
		{
			puts(p);
			free(p);
		}
	}
	return 0;
}



#pragma section _BR_Size
unsigned long BR_Size;
#pragma section


#pragma section _TOP

int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}

#pragma section

