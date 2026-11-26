#include "../engine/include/engine.h"
#include ".\map.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <iostream>

using namespace std;

#if 0
namespace oldmap{
  
static bool LoadMap(const unsigned char* pBuf,MapTile* pMapTiles);
#define OPEN_FILE(f,file_name) (f.Create(file_name,GENERIC_READ,FILE_SHARE_READ,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL))
// ◊∞‘ÿµÿÕº
// µ±pMapTiles Œ™ø’ ±∑µªÿµÿÕºµƒ¥Û–°
// 
bool LoadMap(const char* szFile ,MapTile* pMapTiles,unsigned int * piTileWidth,unsigned int * piTileHeight)
{
	CAtlFile file;
	OPEN_FILE(file,szFile);
	if(pMapTiles == NULL)
	{
		MapHeader hdr;
		file.Read(&hdr,sizeof(MapHeader));
		*piTileWidth = hdr.dwTileWidth;
		*piTileHeight = hdr.dwTileHeight;
		return true;
	}
	else
	{
		unsigned char* pBuf ;
		ULONGLONG ulLen;
		file.GetSize(ulLen);
		int len = ulLen;
		pBuf = (unsigned char*)malloc(len);
		file.Read(pBuf,len);
		bool b = LoadMap(pBuf,pMapTiles);
		free(pBuf);
		return b;
	}
	return true;
}

static bool LoadMap(const unsigned char* pBuf,MapTile* pMapTiles)
{

	MapHeader *hdr;
	hdr = (MapHeader*)pBuf;
	pBuf += sizeof(MapHeader);

	unsigned int i,j;
	unsigned char btemp;
	switch(hdr->dwVersion)
	{
	case 1:
		for(i=0;i<hdr->dwTileHeight;++i)
		{
			for(j=0;j<hdr->dwTileWidth;++j,++pMapTiles)
			{
				pMapTiles->smTile = *(unsigned short*)pBuf;		pBuf += 2;
				pMapTiles->tile   = *(unsigned short*)pBuf;		pBuf += 2;
				pMapTiles->objectPic = *(unsigned short*)pBuf;	pBuf += 2;
				pMapTiles->objectPicLib=*(unsigned short*)pBuf;	pBuf += 2;
				pMapTiles->tileLib = 0xffff;
				pMapTiles->smTileLib = 0xffff;
				btemp = *pBuf;  pBuf +=1;
				
				//pMapTiles->bUsed = ( (btemp & 0x01) > 0 );
				pMapTiles->bBlock = ( (btemp & 0x02) > 0 );
				
				pMapTiles->music = *pBuf; pBuf += 3;
			}
		}
		break;
	case 2:
		for(i=0;i<hdr->dwTileHeight;++i)
		{
			for(j=0;j<hdr->dwTileWidth;++j,++pMapTiles)
			{
				pMapTiles->tileLib = 0xffff;
				pMapTiles->smTileLib = 0xffff;
				btemp = *pBuf; pBuf++;
				//pMapTiles->bUsed = true;
				pMapTiles->bBlock = ( (btemp & 0x01) > 0 );
				if( btemp & 0x02 )
				{
					pMapTiles->smTile = *(unsigned short*)pBuf;
					pBuf += 2;
				}
                else
                {
                	pMapTiles->smTile = 0;
                }

				if( btemp & 0x04 )
				{
					pMapTiles->tile = *(unsigned short*)pBuf;
					pBuf += 2;
				}
                else
                {
                	pMapTiles->tile = 0;
                }

				if( btemp & 0x08 )
				{
					pMapTiles->objectPic = *(unsigned short*)pBuf;
					pBuf += 2;
					pMapTiles->objectPicLib = *(unsigned short*)pBuf;
					pBuf += 2;
				}
                else
                {
                	pMapTiles->objectPic = 0;
                    pMapTiles->objectPicLib = 0;
                }
			}
		}
		break;
	case 3:
		for(i=0;i<hdr->dwTileHeight;++i)
		{
			for(j=0;j<hdr->dwTileWidth;++j,++pMapTiles)
			{
				pMapTiles->tileLib = 0xffff;
				pMapTiles->smTileLib = 0xffff;
				btemp = *pBuf; pBuf++;
				//pMapTiles->bUsed = true;
				pMapTiles->bBlock = ( (btemp & 0x01) > 0 );

				if( btemp & 0x02 )
				{
					pMapTiles->smTile = *(unsigned short*)pBuf;
					pBuf += 2;
				}
                else
                {
                	pMapTiles->smTile = 0;
                }

				if( btemp & 0x04 )
				{
					pMapTiles->tile = *(unsigned short*)pBuf;
					pBuf += 2;
				}
                else
                {
                	pMapTiles->tile = 0;
                }

				if( btemp & 0x08 )
				{
					pMapTiles->objectPic = *(unsigned short*)pBuf;
					pBuf += 2;
					pMapTiles->objectPicLib = *(unsigned short*)pBuf;
					pBuf += 2;
				}
                else
                {
                	pMapTiles->objectPic = 0;
                    pMapTiles->objectPicLib = 0;
                }

				if( btemp & 0x10 )
				{
					pMapTiles->music = *pBuf;
					pBuf ++;
				}
                else
                {
                	pMapTiles->music = 0;
                }
                
				if( btemp & 0x20 )
				{
					pBuf ++;
				}
				if( btemp & 0x40 )
				{
					pBuf ++;
				}
				if( btemp & 0x80 )
				{
					pBuf ++;
				}
			}
		}
		break;
	default:
		assert(0);
		return false;
	}
	return true;
}

int SaveMap(unsigned char* pBuf,MapTile* pMapTiles,int MapWidth,int MapHeight,int iWidth,int iHeight,int ver)
{
	int len = 0;
	int i,j;
    unsigned char *btemp;
    MapTile* p;
	switch(ver)
    {
    case 1:
    	for(i=0;i<iHeight;i++)
        {
        	p = pMapTiles;
        	for(j=0;j<iWidth;j++,p++)
            {
            	if(j >= MapWidth || i >= MapHeight)
                {
                	memset(pBuf,0,12);
                    pBuf+=8;
                    *pBuf = 0x2;
                    pBuf += 4;
                    len+=12;
                }
                else
                {
                	*(unsigned short*)pBuf = p->smTile;			pBuf+=2;
                    *(unsigned short*)pBuf = p->tile;       	pBuf+=2;
                    *(unsigned short*)pBuf = p->objectPic;  	pBuf+=2;
                    *(unsigned short*)pBuf = p->objectPicLib;	pBuf+=2;

                    *pBuf = 0;

                    if(p->bBlock) *pBuf |= 0x2;
                    /*if(p->bUsed)*/  *pBuf |= 0x1;

                    pBuf++;

                    *pBuf = p->music;
                    pBuf+=3; 
                    len+=12;
                    
                }
            }
            pMapTiles += MapWidth;
        }
	    break;
    case 2:
    	for(i=0;i<iHeight;i++)
        {
        	p = pMapTiles;
        	for(j=0;j<iWidth;j++,p++)
            {
            	if(j >= MapWidth || i >= MapHeight)
                {
                	*pBuf++ = 0x1;
                    len++;
                }
                else
                {
                    btemp = pBuf++;
                    len++;
                    *btemp = 0;
                    if(p->bBlock)
                    {
                        *btemp |= 0x1;
                    }
                    if(p->smTile)
                    {
                        *btemp |= 0x2;
                        *(unsigned short*)pBuf = p->smTile;
                        pBuf+=2;
                        len+=2;
                    }
                    if(p->tile)
                    {
                        *btemp |= 0x4;
                        *(unsigned short*)pBuf = p->tile;
                        pBuf += 2;
                        len+=2;
                    }
                    if(p->objectPicLib &&  p->objectPic)
                    {
                        *btemp |= 0x8;

                        *(unsigned short*)pBuf = p->objectPic;
                        pBuf += 2;
                        *(unsigned short*)pBuf = p->objectPicLib;
                        pBuf += 2;

                        len+=4;
                    }
                }
            }
            pMapTiles += MapWidth;
        }
    	break;
    case 3:
        for(i=0;i<iHeight;i++)
        {
        	p = pMapTiles;
        	for(j=0;j<iWidth;j++,p++)
            {
            	if(j >= MapWidth || i >= MapHeight)
                {
                	*pBuf++ = 0x1;
                    len++;
                }
                else
                {
                    btemp = pBuf++;
                    len++;
                    *btemp = 0;
                    if(p->bBlock)
                    {
                        *btemp |= 0x1;
                    }
                    if(p->smTile)
                    {
                        *btemp |= 0x2;
                        *(unsigned short*)pBuf = p->smTile;
                        pBuf+=2;
                        len+=2;
                    }
                    if(p->tile)
                    {
                        *btemp |= 0x4;
                        *(unsigned short*)pBuf = p->tile;
                        pBuf += 2;
                        len+=2;
                    }
                    if(p->objectPicLib &&  p->objectPic)
                    {
                        *btemp |= 0x8;

                        *(unsigned short*)pBuf = p->objectPic;
                        pBuf += 2;
                        *(unsigned short*)pBuf = p->objectPicLib;
                        pBuf += 2;

                        len+=4;
                    }
                    if(p->music)
                    {
                        *btemp |= 0x10;
                        *pBuf = p->music;
                        pBuf ++;
                        len+=1;
                    }
                }
            }
            pMapTiles += MapWidth;
        }
    	break;
    }
    return len;
}

};
#endif
#define MAP_NODE_TERRAIN	MAKEFOURCC('T','E','R','R')
#define MAP_NODE_SMTERRAIN	MAKEFOURCC('S','T','E','R')
#define MAP_NODE_TILEITEM	MAKEFOURCC('T','I','L','E')
#define MAP_NODE_OTHER		MAKEFOURCC('O','T','H','E')
#define MAP_NODE_ADDIN		MAKEFOURCC('A','D','D','I')
#define MAP_NODE_END		MAKEFOURCC(' ','E','N','D')

#define MAP_MAGIC (MAKEFOURCC('X','M','A','P'))

struct stMapNodeHeader{
	DWORD type;
	DWORD size;
};

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param szFileName : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
bool stMap::LoadFromFile(const char* szFileName)
{
	FileStream str(szFileName,FileStream::Read);
	return LoadFromStream(&str);
}


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param p1 : √Ë ˆ
 * \param p2 : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
int __cdecl compareInt(const void * p1, const void * p2) 
{
	DWORD dw1 = ((stMapAddinData*)p1)->nodeNum;
	DWORD dw2 = ((stMapAddinData*)p2)->nodeNum;
	if(dw1 < dw2) return -1;
	if(dw1 == dw2) return 0;
	return 1;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param p1 : √Ë ˆ
 * \param p2 : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
int  __cdecl compareSInt ( const void * p1, const void * p2) 
{
	DWORD dw1 = *((DWORD*)p1);
	DWORD dw2 = ((stMapAddinData*)p2)->nodeNum;
	if(dw1 < dw2) return -1;
	if(dw1 == dw2) return 0;
	return 1;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pt : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
stMapAddinData* stMap::GetAddinData( POINT pt )
{
	DWORD dwNodeNum = ( (pt.y << 16) | pt.x );
	stMapAddinData* p = (stMapAddinData*)bsearch(&dwNodeNum,&m_addins[0],m_addins.size(),sizeof(stMapAddinData),compareSInt);
	return p;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pStream : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
bool stMap::LoadFromStream(Stream* pStream)
{
	stMapFileHeader fhdr;
	
	if(!pStream->read(&fhdr,sizeof(fhdr)))
		return false;
	
	if(fhdr.magic != MAP_MAGIC)
	{
		AssertFatal(0,"µÿÕºŒƒº˛∏Ò Ω≤ª’˝»∑£°");
		return false;
	}
	
	if(fhdr.ver != MAP_VERSION && fhdr.ver != MAP_VERSION_OLD && fhdr.ver != MAP_VERSION_OLD2 && fhdr.ver != MAP_VERSION_OLD3)
	{
		AssertFatal(0,"µÿÕºŒƒº˛∞Ê±æ≤ª’˝»∑£°");
		return false;
	}

	Reset(fhdr.width,fhdr.height);

	for( ; ; )
	{
		stMapNodeHeader hdr;
		if(!pStream->read(&hdr,sizeof(hdr)))
			return false;
		switch(hdr.type)
		{
		case MAP_NODE_ADDIN:	// ≥°æ∞∏Ωº”µƒ ˝æ›
			m_addins.resize( (hdr.size + sizeof(stMapAddinData)-1) / sizeof(stMapAddinData) ) ;
			pStream->read(&m_addins[0],hdr.size);
			qsort(&m_addins[0],m_addins.size(),sizeof(stMapAddinData),compareInt);
			for(size_t i=0;i<m_addins.size();++i)
			{
				//TRACE("-------- %d,%d   music :%d\n",m_addins[i].x,m_addins[i].y,m_addins[i].music);
			}
			break;
		case MAP_NODE_OTHER:
			m_others.resize( (hdr.size + sizeof(stMapTileOther)-1) / sizeof(stMapTileOther) ) ;
			pStream->read(&m_others[0],hdr.size);
			break;
		case MAP_NODE_TERRAIN:
			if(fhdr.ver == MAP_VERSION_OLD)
			{
				int num = CalculateTerrainCount(m_width,m_height);
				if(hdr.size != num * sizeof(stMapTileTerrainOld))
					return false;
				for( size_t i=0; i< num; ++i )
				{
					stMapTileTerrainOld oldst;
					pStream->read(&oldst,sizeof(oldst));
					m_pTerrain[i].tile = oldst.tile;
				}
			}
			else
			{
				if(hdr.size != CalculateTerrainCount(m_width,m_height) * sizeof(stMapTileTerrain))
					return false;
				pStream->read(m_pTerrain,hdr.size);
			}
			break;
		case MAP_NODE_SMTERRAIN:
			if(fhdr.ver == MAP_VERSION){
				if(hdr.size != CalculateSmTerrainCount(m_width,m_height) * sizeof(stMapTileSmTerrain))
					return false;
				pStream->read(m_pSmTerrain,hdr.size);
			}else{
				SetFrameAllocator fa;
				std::vector<stMapTileSmTerrainOld,std::stack_allocator<stMapTileSmTerrainOld> > aTerrainOld;
				aTerrainOld.resize(CalculateSmTerrainCount(m_width,m_height));
				if(hdr.size != aTerrainOld.size() * sizeof(stMapTileSmTerrainOld))
					return false;
				pStream->read(&aTerrainOld[0],hdr.size);
				for( size_t i=0; i< aTerrainOld.size(); ++i )
				{
					if(NULL_MAPIMGID == aTerrainOld[i].smTile)
						m_pSmTerrain[i].smTile = NULL_SMTERRID;
					else
						m_pSmTerrain[i].smTile = aTerrainOld[i].smTile;
				}
			}
			break;
		case MAP_NODE_TILEITEM:
			if(fhdr.ver == MAP_VERSION_OLD)
			{
				int num = CalculateTileCount(m_width,m_height);
				if(hdr.size != num)
					return false;
				for(size_t i=0;i<num;++i)
				{
					stMapTileItemOld oldst;
					pStream->read(&oldst,sizeof(oldst));
					m_pTiles[i].flags = oldst.flags;
					m_pTiles[i].type = oldst.type;
					m_pTiles[i].object = oldst.object;
					m_pTiles[i].objectLib = (BYTE)oldst.objectLib;
					m_pTiles[i].byBlock = 0;
					m_pTiles[i].pOther = NULL;
					m_pTiles[i].zone = -1;
				}
			}
			else if (fhdr.ver == MAP_VERSION_OLD2)
			{
				size_t num =  CalculateTileCount(m_width,m_height);
				if(hdr.size != num * sizeof(stMapTileItemOld2))
					return false;
				for(size_t i=0;i<num;++i)
				{
					stMapTileItemOld2 oldst;
					pStream->read(&oldst,sizeof(oldst));
					memcpy(&m_pTiles[i],&oldst,sizeof(stMapTileItemOld2));
					m_pTiles[i].pOther = NULL;
					m_pTiles[i].byBlock = 0;
					m_pTiles[i].zone = -1;
				}
			}
			else
			{
				size_t count = CalculateTileCount(m_width,m_height);
				if(hdr.size != count * sizeof(stMapTileItem))
					return false;
				for(size_t i=0; i<count ; ++i)
				{
					pStream->read(&m_pTiles[i],sizeof(stMapTileItem));
					m_pTiles[i].byBlock = 0;
					m_pTiles[i].pOther = NULL;
				}
			}
			break;
		case MAP_NODE_END:
			goto _out;
			break;
		default:
			pStream->setPosition(pStream->getPosition() + hdr.size);
			break;
		}
	}
_out:
	for( size_t i=0; i< m_others.size(); ++i )
	{
		stMapTileItemEx* p = GetTile(m_others[i].x,m_others[i].y);
		if(p) p->pOther = &m_others[i];
	}
	return true;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param szFileName : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
bool stMap::SaveToFile(const char* szFileName)
{
	FileStream str(szFileName,FileStream::Write);
	return SaveToStream(&str);
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pStream : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
bool stMap::SaveToStream(Stream* pStream)
{
	Assert(0);
	return false;
}
