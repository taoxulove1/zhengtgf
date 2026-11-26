///********************************************************************
//	created:	2003/06/5
//	created:	5:6:2003   9:30
//	filename: 	c:\ÓÎÏ·¿Í»§¶Ë\Ô´´úÂë\client\client source\stringex.cpp
//	file path:	c:\ÓÎÏ·¿Í»§¶Ë\Ô´´úÂë\client\client source
//	file base:	stringex
//	file ext:	cpp
//	author:		xuzhao
//	
//	purpose:	string
//*********************************************************************/
#include "stdafx.h"

#ifdef MAGIC_EDITOR

#include <cstring>  // [UPGRADED] string.h -> cstring
#include <ctype.h>
#include "stringex.h"


/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param pszSrc : ÃèÊö
 * \param pszDst : ÃèÊö
 * \param pszOld : ÃèÊö
 * \param pszNew : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
//-------------------------------------------------------------------------
// ¹¦ÄÜ: ×Ö·û´®Ìæ»»
//-------------------------------------------------------------------------
void str_replace(const char* pszSrc,char* pszDst,const char* pszOld,const char* pszNew)
{
	const char* p,*q;
	char *q1;

	int len_old = (int)strlen(pszOld);
	int len_new = (int)strlen(pszNew);

	int l;
	q = pszSrc;
	q1 = pszDst;
	for(;;)
	{
		p = strstr(q,pszOld);
		if(p == NULL)
		{
			strcpy(q1,q);
			break;
		}
		else
		{

			l = (int)(p-q);
			memcpy(q1,q,l);
			q1 += l;
			memcpy(q1,pszNew,len_new);
			q1 += len_new;
			q = p + len_old;
		}
	}
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param pszSrc : ÃèÊö
 * \param pszOld : ÃèÊö
 * \param pszNew : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
//-------------------------------------------------------------------------
// ¹¦ÄÜ: ×Ö·û´®Ìæ»»
//-------------------------------------------------------------------------
void str_replace(char* pszSrc,const char* pszOld,const char* pszNew)
{
	int len = (int)strlen(pszSrc);
	char * psz = new char[len*2];

	str_replace(pszSrc,psz,pszOld,pszNew);

	strcpy(pszSrc,psz);

	delete [] psz;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param pszSrc : ÃèÊö
 * \param cOld : ÃèÊö
 * \param cNew : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
//-------------------------------------------------------------------------
// ¹¦ÄÜ: ×Ö·û´®Ìæ»»
//-------------------------------------------------------------------------
void str_replace(char* pszSrc,char cOld,char cNew)
{
	while(*pszSrc)
	{
		if((*pszSrc) & 0x80)
		{
			// ºº×Ö
			pszSrc+=2;
		}
		else
		{
			if(*pszSrc == cOld)
				*pszSrc = cNew;
			pszSrc++;
		}
	}
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param pszSrc : ÃèÊö
 * \param cOld : ÃèÊö
 * \param cNew : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
//-------------------------------------------------------------------------
// ¹¦ÄÜ: ×Ö·û´®Ìæ»»
//-------------------------------------------------------------------------
void str_replace(char* pszSrc,wchar_t cOld,wchar_t cNew)
{
	while(*pszSrc)
	{
		if((*pszSrc) & 0x80)
		{
			// ºº×Ö
			if((*(wchar_t*)pszSrc) == cOld)
			{
				(*(wchar_t*)pszSrc) = cNew;
			}
			pszSrc+=2;
		}
		else
		{
			pszSrc++;
		}
	}
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param pszSrc : ÃèÊö
 * \param c : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
//-------------------------------------------------------------------------
// ¹¦ÄÜ: É¾³ı×Ö·û
//-------------------------------------------------------------------------
void str_del(char* pszSrc,char c)
{
	while(*pszSrc)
	{
		if((*pszSrc) & 0x80)
		{
			// ºº×Ö
			pszSrc+=2;
		}
		else
		{
			if((*pszSrc) == c)
			{
				strcpy(pszSrc,pszSrc+1);
			}
			else
			{
				pszSrc++;
			}
		}
	}
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param pszSrc : ÃèÊö
 * \param c : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
//-------------------------------------------------------------------------
// ¹¦ÄÜ: É¾³ı×Ö·û
//-------------------------------------------------------------------------
void str_del(char* pszSrc,wchar_t c)
{
	while(*pszSrc)
	{
		if((*pszSrc) & 0x80)
		{
			// ºº×Ö
			if((*(wchar_t*)pszSrc) == c)
			{
				strcpy(pszSrc,pszSrc+2);
			}
			else
			{
				pszSrc+=2;
			}
		}
		else
		{
			pszSrc++;
		}
	}
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param psz : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
//-------------------------------------------------------------------------
// ¹¦ÄÜ: ÅĞ¶ÏÊÇ·ñÊÇÒ»¸ö¿Õ°××Ö·û´®
//-------------------------------------------------------------------------
BOOL str_isempty(const char* psz)
{
	if(psz == NULL) return TRUE;
	while(*psz)
	{
		if((*psz) != 20) break;
		psz++;
	}
	return (*psz == 0);
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \return ·µ»ØÖµµÄÃèÊö
 */
//-------------------------------------------------------------------------
// ¹¦ÄÜ: ×Ö·û´®¿½±´, ·ÀÖ¹Ô½½ç, ·ÀÖ¹°ë¸öºº×Ö¿½±´
//-------------------------------------------------------------------------
char* str_cpy(char* pszDest,const char* pszSrc,int len/* Ä¿µÄ³¤¶È(°üÀ¨×îºóµÄ'\0')*/)
{
	int i = 0;
	char* p = pszDest;
	while(*pszSrc)
	{

		if((*pszSrc) & 0x80)
		{
			// ºº×Ö
			if(*(pszSrc+1) == 0) break;
			if(len-i <= 2) break;
			*(unsigned short*)pszDest = *(const unsigned short*)pszSrc;
			pszDest += 2;
			pszSrc  += 2;
			i+=2;		
		}
		else
		{
			if(len-i <= 1) break;
			*pszDest = *pszSrc;
			++pszDest;
			++pszSrc;
			++i;
		}
	}
	*pszDest = 0;
	return p;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param psz : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void str_trim_right(char* psz)
{
	
	char* p = psz;
	char* p1 = NULL;
	while(*p)
	{
		if((*p) & 0x80)
		{
			if(iswspace(*((WORD*)p)))
			{
				if(p1 == NULL)
					p1 = p;
			}
			else
			{
				if(p1)p1 = NULL;
			}
			p+=2;
		}
		else
		{
			if(isspace(*p))
			{
				if(p1 == NULL)
					p1 = p;
			}
			else
			{
				if(p1)p1 = NULL;
			}
			p++;
		}
	}
	if(p1) *p1 = 0;
}

/**
 * \brief ¼ò¶ÌÃèÊö
 * 
 * ÏêÏ¸ÃèÊö
 * 
 * \param psz : ÃèÊö
 * \return ·µ»ØÖµµÄÃèÊö
 */
void str_trim_left(char* psz)
{
//	char* pszNew = new char[strlen(psz) + 1];
	char* p = psz;
	while(*p)
	{
		if((*p) & 0x80)
		{
			if(iswspace(*((WORD*)p)))
			{
			}
			else
			{
				break;
			}
			p+=2;
		}
		else
		{
			if(isspace(*p))
			{
			}
			else
			{
				break;
			}
			p++;
		}
	}
	strcpy(psz,p);
}

#endif
