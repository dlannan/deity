// String_types

#ifndef _STRING_TYPES_H_
#define _STRING_TYPES_H_

// MS SHITTY TEMPLATE SUPPORT!
#pragma warning(disable:4786)

#include <vector>
#include <string.h>

using namespace std;

namespace Data
{
	// Should be a better way to do this - static arrays are most efficient, but not flexible.
	#define		MAX_STRING_SIZE		255

	enum {
			STR_FORMAT_HEX	=	10,
			STR_FORMAT_FLOAT =	11,
			STR_FORMAT_DOUBLE =	12
		};

	class StringClass 
	{
	private:
		unsigned int	formatting;

		char			text[MAX_STRING_SIZE];
	public:

		StringClass() 
		{
			Clear();
			formatting = 0;
		}

		~StringClass() 
		{
		}

		const char *String(void) 
		{
			if(text[0])
				return text;
			else
				return NULL;
		}

		void Lower(void)
		{
			if(text[0])
			{
				_strlwr(text);			
			}
		}

		int MaxSize(void)
		{
			return MAX_STRING_SIZE;
		}

		void Clear(void)
		{
			char *t = &text[0];
			int i = MAX_STRING_SIZE;
			while(i--)
				*t=0;
		}

		int Empty(void)
		{
			return (text[0] == 0);
		}

		// Removes the CR or LF from a string
		void NoEndLine()
		{
			*this - '\n';
			*this - '\r';
		}

		void NoWhiteSpace()
		{
			char *t = &text[0];
			char *s = t;
			while(*t)
			{
				switch(*t)
				{
				case ' ':	//space
				case '\n':	//newline
				case '\r':	//carriage return
				case '\t':	//tab
					if(*(t+1) == 0 ) *s = 0;
					else *s = *t;
					break;
				default:
					*s = *t;
					s++;
					break;
				}
				t++;
			}
		}

		void operator << (const char *news)
		{
			if(news)StringClass::operator + (news);
		}

		void operator << (StringClass *text)
		{
			if(text)StringClass::operator << (text->String());
		}

		void operator << (int varint)
		{
			int	rad = 10;
			if(formatting & STR_FORMAT_HEX) rad = 16;
			char strint[255];
			itoa(varint, strint, rad);
			StringClass::operator << (strint);
		}

		void operator << (float varfl)
		{
			char strdbl[255];
			_gcvt((double)varfl, 7, strdbl);
			StringClass::operator << (strdbl);
		}

		void operator << (double vardbl)
		{
			char strdbl[255];
			_gcvt(vardbl, 7, strdbl);
			StringClass::operator << (strdbl);
		}

	//	void operator << (char varch)
	//	{
	//		char strch[255];
	//		itoa((int)varch, strch, 10);
	//		StringClass::operator << (strch);
	//	}

		void operator << (long varlong)
		{
			int	rad = 10;
			if(formatting & STR_FORMAT_HEX) rad = 16;
			char strlong[255];
			ltoa(varlong, strlong, rad);
			StringClass::operator << (strlong);
		}

		void operator << (unsigned int varuint)
		{
			int	rad = 10;
			if(formatting & STR_FORMAT_HEX) rad = 16;
			char struint[255];
			itoa(varuint, struint, rad);
			StringClass::operator << (struint);
		}

		void operator << (unsigned char varuch)
		{
			int	rad = 10;
			if(formatting & STR_FORMAT_HEX) rad = 16;
			char strch[255];
			itoa((int)varuch, strch, rad);
			StringClass::operator << (strch);
		}

		void operator << (unsigned long varulong)
		{
			int	rad = 10;
			if(formatting & STR_FORMAT_HEX) rad = 16;
			char strlong[255];
			ltoa((long)varulong, strlong, rad);
			StringClass::operator << (strlong);
		}

		// Just add a char
		void operator << (char news)
		{
			char temp[2];
			temp[0] = news; temp[1]='\0';
			if(!news) return;
			StringClass::operator + (temp);
		}

		StringClass operator + (const char *news)
		{
			if(strlen(text) + strlen(news) > MAX_STRING_SIZE ) return *this;
			strcat(text,news);

			if( (strlen(news) == 2) && (strcmp(news,"0x")==0) )
			{
				formatting |= STR_FORMAT_HEX;
			}
			else
			{
				formatting = 0;
			}

			return *this;
		}

		// Allows you to remove all the particular chars out of the string
		void operator - (char ch)
		{
			if(Empty()) return;
			
			char *t = &text[0];
			char *s = t;
			while(*t)
			{
				if(*t == ch)
				{
					if(*(t+1) == 0 ) *s = 0;
					else *s = *t;
				}
				else
				{
					*s = *t;
					s++;
				}
				t++;
			}
		}

		void SetData(const char *news)
		{
			if(news) strcpy(text, news);
		}

		void SetData(char *news)
		{
			if(news) strcpy(text, news);
		}

		void SetData(char *news, int size)
		{
			if(news) strncpy(text, news, size);
		}

		char Char(int ch)
		{
			if(Empty())
				return 0;
			else
				return text[ch];
		}

		int Size(void)
		{
			return strlen(text);
		}

		void operator = (const char *news)
		{
			if(news) SetData(news);
		}

		void operator = (char *news)
		{
			if(news) SetData(news);
		}
	
		void operator = (StringClass news)
		{
			if(!news.Empty()) SetData(news.String());
		}

	};

	// Used for making textual pages..
	typedef vector<StringClass>	PageType;
}

#endif