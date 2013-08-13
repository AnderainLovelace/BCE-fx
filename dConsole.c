#include <fxlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "dconsole.h"

#define LINE_ROW_MAX	8
#define LINE_COL_MAX	21

typedef unsigned int 	uint;
typedef unsigned char	uchar;

char	line_buf[LINE_ROW_MAX][LINE_COL_MAX+1];
int		line_index	= 0;
int		line_x		= 0;
int		line_start	= 0;
int		line_count	= 0;

void dAreaClear (int left,int top,int right,int bottom,int sel) 
{ 
	DISPBOX box; 
	box.left = left;box.top = top;box.right = right;box.bottom = bottom; 
 	if (sel != 1) 
		Bdisp_AreaClr_VRAM (&box); 
	if (sel != 0){ 
 		Bdisp_DrawLineVRAM (box.left,box.top,box.right,box.top); 
		Bdisp_DrawLineVRAM (box.left,box.bottom,box.right,box.bottom); 
		Bdisp_DrawLineVRAM (box.left,box.top,box.left,box.bottom); 
		Bdisp_DrawLineVRAM (box.right,box.top,box.right,box.bottom); 
    } 
}

uint WaitKey ()
{
	uint key;GetKey(&key);return key;
}

char dGetKeyChar (uint key,int downcase)
{
	if (key>=KEY_CHAR_A && key<=KEY_CHAR_Z)
		return downcase ? key+32 : key;

	else if (key>=KEY_CHAR_0 && key<= KEY_CHAR_9)
		return key;
	else if (key>=KEY_CHAR_0 && key<= KEY_CHAR_9)
		return key;

	else if (key==KEY_CHAR_EQUAL)
		return '=';
	else if (key==KEY_CHAR_PLUS)
		return '+';
	else if (key==KEY_CHAR_LPAR)
		return '(';
	else if (key==KEY_CHAR_RPAR)
		return ')';
	return 0;
}
int dGetLine (char * s,int max)	// This function is depended on dConsole
								// And this function is not allowed to abolish
{
	int		pos = strlen(s);
	int		refresh = 1;
	int		x,y,l,width;
	uint	key;
	char	c;
	int		downcase = 0;

	l = strlen (line_buf[line_index]);
	
	if (l>=LINE_COL_MAX)
	{
		dConsolePut("\n");
		l = 0;
	}
	else
		dConsoleRedraw();

	x = l + 1;
	y = line_count;
	width = LINE_COL_MAX - 2;

	while (1)
	{
		if (refresh)
		{
			int i;
			locate(x,y);PrintRev((uchar*)(downcase ? "\xE6\x9D" : "\xE6\x9C"));
			for (i=x+1;i<=LINE_COL_MAX;++i)
			{
				locate(i,y);Print((uchar*)" ");
			}
			if (pos<width-1)
			{
				locate(x+1,y);		Print((uchar*)s);
				locate(x+pos+1,y);	Print((uchar*)"_");
			}
			else
			{
				locate(x+1,y);			Print((uchar*)s+pos-width+1);
				locate(x+width,y);	Print((uchar*)"_");
			}
			refresh = 0;
		}
		GetKey(&key);
		if ((c=dGetKeyChar(key,downcase))!=0)
		{
			if (pos>=max) continue;
			s[pos++] = c;s[pos] = '\0';
			refresh = 1;
		}
		else
		{
			if (key==KEY_CTRL_DEL)
			{
				if (pos<=0) continue;
				s[--pos] = '\0';
				refresh  = 1;
			}
			else if (key==KEY_CTRL_AC)
			{
				*s		= 0;
				pos		= 0;
				refresh	= 1;
			}
			else if (key==KEY_CTRL_OPTN)
			{
				downcase = !downcase;
				refresh = 1;
			}
			else if (key==KEY_CTRL_EXE) return 1;
			
		}
	}
	return 0;
}
void dConsoleRedraw ()
{
	int i,j;
	Bdisp_AllClr_VRAM();
	for(i=0,j=line_start;i<line_count;++i)
	{
		locate(1,i+1);Print((uchar*)line_buf[j]);
		if (++j>=LINE_ROW_MAX) j = 0;
	}

}
void dConsolePut(const char * str)
{
	if (line_count == 0) line_count = 1;
	for (;*str;++str)
	{
		if (*str != '\n')
			line_buf[line_index][line_x++] = *str;
		else
		{
			line_buf[line_index][line_x] = '\0';
			line_x = LINE_COL_MAX;
		}
		if (line_x>=LINE_COL_MAX)
		{
			line_buf[line_index][line_x] = '\0';

			line_x = 0;
			if (line_count<LINE_ROW_MAX)
			{
				++line_count;
			}
			else
			{
				line_start++;
				if (line_start>=LINE_ROW_MAX) line_start = 0;
			}
			line_index++;
			if (line_index>=LINE_ROW_MAX) line_index = 0;
		}
	}
	line_buf[line_index][line_x] = '\0';
	dConsoleRedraw();
}
void dPuts(const char * str)
{
	dConsolePut(str);
	dConsolePut("\n");
}

int dPrintf (const char * format,...)
{
	char	buf[256];
	int		length;
	va_list	arg_list;

	va_start(arg_list,format);
	length = vsprintf(buf,format,arg_list);
	va_end(arg_list);

	dConsolePut (buf);

	return length;
}

