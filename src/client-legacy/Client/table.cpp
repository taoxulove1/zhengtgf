#include "public.h"
#include "table.h"
#include <algorithm>
#include "command.h"
#include "../xml_parse/XmlParseEx.h"

#ifdef _AUTOPLAY
#include "./AutoGame.h"
#endif

const int c_nGAME_VERSION = 20060216;// ’‚∏ˆ≤Œ ˝◊˜∑œ£¨–¬≤Œ ˝∂®“Â‘⁄Game.cpp÷–
/*
static  stCharTypeInfo c_charTypeInfo[9] =
{
	{PROFESSION_NONE,0,1,1,"Œﬁ“µ",eSex_Male,{0}},

	{PROFESSION_1,0,1,1,"œ¿øÕ",eSex_Male,{0}},
	{PROFESSION_2,1,2,2,"œ¿≈Æ",eSex_Female,{0}},
	{PROFESSION_3,2,3,3,"º˝œ¿",eSex_Male,{0}},
	{PROFESSION_4,3,4,4,"º˝¡È",eSex_Female,{0}},

	{PROFESSION_5,4,5,5,"æ¸ ¶",eSex_Male,{0}},
	{PROFESSION_6,5,6,6,"√¿≈Æ",eSex_Female,{0}},
	{PROFESSION_7,6,7,7,"∑® ¶",eSex_Male,{0}},
	{PROFESSION_8,7,8,8,"œ…≈Æ",eSex_Female,{0}},
};
*/
static  stCharTypeInfo c_charTypeInfo[3] =
{
	{PROFESSION_NONE,0,1,1,"Œﬁ“µ",eSex_Male,{0}},

	{PROFESSION_1,0,1,1,"œ¿øÕ",eSex_Male,{0}},
	{PROFESSION_2,1,2,2,"œ¿≈Æ",eSex_Female,{0}},
};


/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param type : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
const stCharTypeInfo & GetCharTypeInfo(size_t type)
{
	FUNCTION_BEGIN;

	for(size_t i=0;i<count_of(c_charTypeInfo);++i)
	{
		if(type == c_charTypeInfo[i].type)
			return c_charTypeInfo[i];
	}
	return c_charTypeInfo[0];

	FUNCTION_END;
}

tTableObjectBase g_tableObjectBase;
tTableNpcBase	 g_tableNpcBase;
tTableSkillBase	 g_tableSkillBase;
tTableSkillLevelBase	 g_tableSkillLevelBase;
tTableObjectUp  	 g_tableObjectUp;
tTableItemSetBase		g_tableItemSetBase;
tTableCharStateBase	g_tableCharStateBase;
tTableHairStyle		g_tableHairStyle;
tTableHairColor		g_tableHairColor;
tTableKnight        g_tableKnight;
tTableCivilian      g_tableCivilian;
tTableHeadList          g_tableHeadList;
tLevelUpTipMap		g_mapLevelup;
tTableCountryMaterialList g_CountryMatrialList;

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param data1 : √Ë ˆ
 * \param data2 : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
template <class tRecord>
inline bool DataLessById(tRecord& data1,tRecord & data2)
{
	FUNCTION_BEGIN;

	return data1.dwID <= data2.dwID;

	FUNCTION_END;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param data1 : √Ë ˆ
 * \param data2 : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
bool SkillLevelDataLessById(SkillLevelBase_t & data1,SkillLevelBase_t& data2)
{
	FUNCTION_BEGIN;

	if(data1.dwID == data2.dwID)
	{
		return data1.dwLevel < data2.dwLevel;
	}
	return data1.dwID <= data2.dwID;

	FUNCTION_END;
}

char*  GetKnightName(DWORD value)
{
	for (int i=0;i<g_tableKnight.size();i++)
	{
		if (value<g_tableKnight[i].dwValue)
		{
			if (i>0)
				return g_tableKnight[i-1].strName;
			else
				return "";
		}
	}
	return g_tableKnight[g_tableKnight.size()-1].strName;
}

char*  GetCivilianName(DWORD value)
{
	for (int i=0;i<g_tableCivilian.size();i++)
	{
		if (value<g_tableCivilian[i].dwValue)
		{
			if (i>0)
				return g_tableCivilian[i-1].strName;
			else
				return "";
		}
	}
	return g_tableCivilian[g_tableCivilian.size()-1].strName;
}
/**
* \brief ºÚ∂Ã√Ë ˆ
* 
* œÍœ∏√Ë ˆ
* 
* \param data1 : √Ë ˆ
* \param data2 : √Ë ˆ
* \return ∑µªÿ÷µµƒ√Ë ˆ
*/
bool SkillLevelDataLessByIdTree(SkillLevelBase & data1,SkillLevelBase& data2)
{
	FUNCTION_BEGIN;

	if(data1.dwID == data2.dwID)
	{
		return data1.dwLevel < data2.dwLevel;
	}
	return data1.dwID <= data2.dwID;

	FUNCTION_END;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pStream : √Ë ˆ
 * \param vecData : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
template <class tRecord>
bool ReadTableData(Stream* pStream,std::vector<tRecord> & vecData)
{
	FUNCTION_BEGIN;

	DWORD dwCount;
	pStream->read(&dwCount,sizeof(dwCount));
	vecData.resize(dwCount);
	pStream->read(&vecData[0],sizeof(tRecord) * dwCount);
	std::sort(vecData.begin(),vecData.end(),DataLessById<tRecord>);
	return true;

	FUNCTION_END;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pStream : √Ë ˆ
 * \param vecData : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
bool ReadSkillLevelTableData(Stream* pStream,std::vector<SkillLevelBase_t> & vecData)
{
	FUNCTION_BEGIN;

	DWORD dwCount;
	pStream->read(&dwCount,sizeof(dwCount));
	vecData.resize(dwCount);
	pStream->read(&vecData[0],sizeof(SkillLevelBase_t) * dwCount);
	std::sort(vecData.begin(),vecData.end(),SkillLevelDataLessById);
	return true;

	FUNCTION_END;
}

/**
* \brief ºÚ∂Ã√Ë ˆ
* 
* œÍœ∏√Ë ˆ
* 
* \param pStream : √Ë ˆ
* \param vecData : √Ë ˆ
* \return ∑µªÿ÷µµƒ√Ë ˆ
*/
bool ReadSkillLevelTableDataTree(Stream* pStream,std::vector<SkillLevelBase> & vecData)
{
	FUNCTION_BEGIN;

	DWORD dwCount;
	pStream->read(&dwCount,sizeof(dwCount));
	vecData.resize(dwCount);
	pStream->read(&vecData[0],sizeof(SkillLevelBase) * dwCount);
	std::sort(vecData.begin(),vecData.end(),SkillLevelDataLessByIdTree);
	return true;

	FUNCTION_END;
}

bool LoadLevelupTipMap()
{
	FUNCTION_BEGIN;

	TiXmlDocument doc;
	Stream* pStream = OpenPackFileForRead("data\\datas.pak","datas\\leveluptip.xml");
	if(!pStream) return false;	
	if(	true == LoadXMLFromStream(pStream,doc) )
	{
		TiXmlElement* Tips = doc.FirstChildElement("Tips");
		if( Tips )
		{
			TiXmlElement* Tip = Tips->FirstChildElement("Tip");
			while( Tip )
			{
				int level = 0;
				if( Tip->QueryIntAttribute("level",&level) != TIXML_SUCCESS )
				{
					level = 0;
				}
				const char* item = Tip->Attribute("item");
				if( NULL != item && strlen(item) > 0 )
				{
					g_mapLevelup.insert(tLevelUpTipMap::value_type(level,item));
				}				
				Tip = Tip->NextSiblingElement("Tip");
			}
		}
	}
	ClosePackFileForRead(pStream);
	return true;
	
	FUNCTION_END;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
bool LoadTables()
{
	FUNCTION_BEGIN;

	LoadLevelupTipMap();

	Stream* pStream;

	pStream = OpenPackFileForRead("data\\datas.pak","datas\\tables\\ObjectBase.tbl");		//"data\\datas.pak"
	if(!pStream) return false;
	if(!ReadTableData<ObjectBase_t>(pStream,g_tableObjectBase)) 
	{
		ClosePackFileForRead(pStream);
		return false;
	}



	ClosePackFileForRead(pStream);
//#ifdef _DEBUG	
//	for(tTableObjectBase::iterator it = g_tableObjectBase.begin(); it != g_tableObjectBase.end(); ++it)
//	{ 
//		TRACE("object:[%u][%s][%x]\n",(*it).dwID,(*it).strName,(*it).dwType);
//		//TRACE("object:[%u][%s][%x][%u]\n",(*it).dwID,(*it).strName,(*it).dwType,(*it).dwIntervalTime);
//	}
//#endif

	//*
	pStream = OpenPackFileForRead("data\\datas.pak","datas\\tables\\NpcBase.tbl");
	if(!pStream) return false;
	if(!ReadTableData<NpcBase_t>(pStream,g_tableNpcBase))
	{
		ClosePackFileForRead(pStream);
		return false;
	}
	////for(tTableNpcBase::iterator it = g_tableNpcBase.begin(); it != g_tableNpcBase.end(); ++it)
	//{ 
	//	/*TRACE("npc:[%d][%s][%d][%d][%d][%d][%d][%d]\n",
	//		(*it).dwID,
	//		(*it).strName,
	//		(*it).dwType,
	//		(*it).dwLevel,
	//		(*it).dwColor,
	//		(*it).dwFive,
	//		(*it).dwPic,
	//		(*it).dwImage);*/
	//}

#ifdef _AUTOPLAY
	if(gAutoGameConfig.fnLoadNpcTables){
		pStream->setPosition(0);
		std::vector<BYTE> data;
		data.resize(pStream->getSize());
		pStream->read(&data[0],data.size());
		gAutoGameConfig.fnLoadNpcTables(&data[0]);
	}
#endif

	ClosePackFileForRead(pStream);


	//*
	pStream = OpenPackFileForRead("data\\datas.pak","datas\\tables\\ObjectUp.tbl");
	if(!pStream) return false;
	if(!ReadTableData<ObjectUp_t>(pStream,g_tableObjectUp))
	{
		ClosePackFileForRead(pStream);
		return false;
	}
	ClosePackFileForRead(pStream);


	//*/
	pStream = OpenPackFileForRead("data\\datas.pak","datas\\tables\\SkillBase.tbl");		//"data\\datas.pak"
	if(!pStream) return false;
	if(!ReadTableData<SkillBase_t>(pStream,g_tableSkillBase))
	{
		ClosePackFileForRead(pStream);
		return false;
	}

#ifdef _AUTOPLAY
	if(gAutoGameConfig.fnLoadSkillTables){
		pStream->setPosition(0);
		std::vector<BYTE> data;
		data.resize(pStream->getSize());
		pStream->read(&data[0],data.size());
		gAutoGameConfig.fnLoadSkillTables(&data[0]);
	}
#endif

	ClosePackFileForRead(pStream);

	
	for(tTableSkillBase::iterator it = g_tableSkillBase.begin(); it != g_tableSkillBase.end(); ++it)
	{ 
		//if ( (*it).dwID == 377)
		//	TRACE( "skill:%u:%s:%u, target=%u\n",(*it).dwID,(*it).pstrName,(*it).dwSerialType,(*it).dwPic );
	}
	
	pStream = OpenPackFileForRead("data\\datas.pak","datas\\tables\\SkillLevelBase.tbl");		//"data\\datas.pak"
	if(!pStream) return false;
	if(!ReadSkillLevelTableData(pStream,g_tableSkillLevelBase))
	{
		ClosePackFileForRead(pStream);
		return false;
	}

#ifdef _AUTOPLAY
	if(gAutoGameConfig.fnLoadSkillLevelTables){
		pStream->setPosition(0);
		std::vector<BYTE> data;
		data.resize(pStream->getSize());
		pStream->read(&data[0],data.size());
		gAutoGameConfig.fnLoadSkillLevelTables(&data[0]);
	}
#endif

	ClosePackFileForRead(pStream);

	/*for(tTableSkillLevelBase::iterator it = g_tableSkillLevelBase.begin(); it != g_tableSkillLevelBase.end(); ++it)
	{ 
		if ( (*it).dwID == 246)
			TRACE( "skill:%u:%s:%s, MP=%u\n",(*it).dwID,(*it).pstrName,(*it).strDesc,(*it).dwSP );
	}*/
	

	//Read item set table
	pStream = OpenPackFileForRead( "data\\datas.pak", "datas\\tables\\SetObjectBase.tbl" );
	if( !pStream) return false;

	std::vector<ItemSetTable_t> tableItemSetSource;
	//
	if ( !ReadTableData<ItemSetTable_t>( pStream, tableItemSetSource ) )
	{
		ClosePackFileForRead( pStream );
		return false;
	}

	//Get what we want really
	for( std::vector<ItemSetTable_t>::iterator itor=tableItemSetSource.begin();itor!=tableItemSetSource.end();itor++ )
	{
		ItemSetBase setBase;
		size_t k;
		size_t num;

		setBase.dwID = itor->dwID;
		sprintf( setBase.pstrName, itor->pstrName );
		setBase.charType = itor->charType;
		//Get item id list
		std::vector<DWORD>	aParams;
		_parse_str_num( aParams, itor->pstrListID );
		num = 7;
		for( k=0;k<getMin(aParams.size(),num);k++ )
			setBase.itemList[k] = aParams[k];
		//Get effect one :: set property id list
		aParams.resize(0);
		_parse_str_num( aParams, itor->pstrEffectOneID );
		num = 3;
		for( k=0;k<getMin(aParams.size(),num);k++ )
			setBase.effectOneID[k] = aParams[k];
		//Get effect two :: set property id list
		aParams.resize(0);
		_parse_str_num( aParams, itor->pstrEffectTwoID );
		num = 5;
		for( k=0;k<getMin(aParams.size(),num);k++ )
			setBase.effectTwoID[k] = aParams[k];
		//Get all :: set property id list
		aParams.resize(0);
		_parse_str_num( aParams, itor->pstrAllID );
		num = 7;
		for( k=0;k<getMin(aParams.size(),num);k++ )
			setBase.idList[k] = aParams[k];

		//Push to global var
		g_tableItemSetBase.push_back( setBase );
	}
	//
	ClosePackFileForRead( pStream );

	pStream = OpenPackFileForRead("data\\datas.pak","datas\\tables\\CharacterStateBase.tbl");		//"data\\datas.pak"
	if(!pStream) return false;
	if(!ReadTableData<CharacterStateBase_t>(pStream,g_tableCharStateBase))
	{
		ClosePackFileForRead(pStream);
		return false;
	}
	ClosePackFileForRead(pStream);
	pStream = OpenPackFileForRead("data\\datas.pak","datas\\tables\\HairStyle.tbl");		//"data\\datas.pak"
	if(!pStream) return false;
	if(!ReadTableData<HairStyle_t>(pStream,g_tableHairStyle))
	{
		ClosePackFileForRead(pStream);
		return false;
	}
	ClosePackFileForRead(pStream);

	pStream = OpenPackFileForRead("data\\datas.pak","datas\\tables\\HairColour.tbl");		//"data\\datas.pak"
	if(!pStream) return false;
	if(!ReadTableData<HairColor_t>(pStream,g_tableHairColor))
	{
		ClosePackFileForRead(pStream);
		return false;
	}
	ClosePackFileForRead(pStream);


	pStream = OpenPackFileForRead("data\\datas.pak","datas\\tables\\Knight.tbl");		//"data\\datas.pak"
	if(!pStream) return false;
	if(!ReadTableData<Knight_t>(pStream,g_tableKnight))
	{
		ClosePackFileForRead(pStream);
		return false;
	}
	ClosePackFileForRead(pStream);

#ifdef _DEBUG
	for (int i=0;i<g_tableKnight.size();i++)
	{
		TRACE("%u %s %u\n",g_tableKnight[i].dwID,g_tableKnight[i].strName,g_tableKnight[i].dwValue);
	}
#endif

	pStream = OpenPackFileForRead("data\\datas.pak","datas\\tables\\Civilian.tbl");		//"data\\datas.pak"
	if(!pStream) return false;
	if(!ReadTableData<Civilian_t>(pStream,g_tableCivilian))
	{
		ClosePackFileForRead(pStream);
		return false;
	}
	ClosePackFileForRead(pStream);


	pStream = OpenPackFileForRead("data\\datas.pak","datas\\tables\\HeadListBase.tbl");		//"data\\datas.pak"
	if(!pStream) return false;
	if(!ReadTableData<HeadList_t>(pStream,g_tableHeadList))
	{
		ClosePackFileForRead(pStream);
		return false;
	}
	ClosePackFileForRead(pStream);

	pStream = OpenPackFileForRead("data\\datas.pak","datas\\tables\\CountryMaterial.tbl");		//"data\\datas.pak"
	if(!pStream) return false;
	if(!ReadTableData<CountryMaterial_t>(pStream,g_CountryMatrialList))
	{
		ClosePackFileForRead(pStream);
		return false;
	}
	ClosePackFileForRead(pStream);


	/*for(tTableCharStateBase::iterator it = g_tableCharStateBase.begin(); it != g_tableCharStateBase.end(); ++it)
		TRACE( "CharacterState::ID=%u	PIC=%u	SID=%u\n", (*it).dwID, (*it).dwPicID, (*it).dwStateID ); */

	CIni ini;
	ini.Open("data\\datas.pak","datas\\hair.ini");
	for(size_t i=0; i < count_of(c_charTypeInfo); ++i)
	{
		char szType[64];
		sprintf(szType,"%d",c_charTypeInfo[i].type);
		for(size_t j = 0; j < 16; ++j)
		{
			char szHair[64];
			sprintf(szHair,"%d",j);
			c_charTypeInfo[i].hair[j] = ini.GetInt(szType,szHair,0);
		}
		c_charTypeInfo[i].equipHairPic = ini.GetInt(szType,"paper",0);
	}
	return true;

	FUNCTION_END;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
bool ReLoadSkillTable()
{
	FUNCTION_BEGIN;

	Stream* pStream;
	pStream = OpenPackFileForRead("data\\datas.pak","datas\\tables\\SkillBase.tbl");		//"data\\datas.pak"
	if(!pStream) return false;
	if(!ReadTableData<SkillBase_t>(pStream,g_tableSkillBase))
	{
		ClosePackFileForRead(pStream);
		return false;
	}
	ClosePackFileForRead(pStream);

	/*for(tTableSkillBase::iterator it = g_tableSkillBase.begin(); it != g_tableSkillBase.end(); ++it)
	{ 
		TRACE( "skill:%u:%s, itemID=%d\n",(*it).dwID,(*it).pstrName,(*it).dwUpItemID );
	}*/
	
	return true;

	FUNCTION_END;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
void UnloadTables()
{
	FUNCTION_BEGIN;

	g_tableObjectBase.resize(0);
	g_tableNpcBase.resize(0);

	FUNCTION_END;
}

bool IsSelectFirstSkill(SkillBase* pSelectSkill)
{
	if ( NULL == pSelectSkill )
		return false;

	if ( pSelectSkill->dwUseMethod & SkillUseMethod_SelectFirst )
		return true;

	return false;
}

bool IsCanSelectInHandSkill(SkillBase* pSelectSkill)
{
	if ( NULL == pSelectSkill )
		return false;

	if ( pSelectSkill->dwType == SkillType_Passive )
		return false;

	if ( pSelectSkill && ( (pSelectSkill->dwType == SkillType_State) || (pSelectSkill->dwType == SkillType_StateAttack) )
		&& (pSelectSkill->dwTarget & SkillTarget_Single)
		&& !( (pSelectSkill->dwTarget & SkillTarget_Self) && !(pSelectSkill->dwTarget & SkillTarget_Single) ) )

		return true;
	if ( pSelectSkill && (pSelectSkill->dwTarget & SkillTarget_Death) )
		return true;

	if ( pSelectSkill
		&& ( (pSelectSkill->dwTarget & SkillTarget_Pet)
			|| (pSelectSkill->dwTarget & SkillTarget_Summon) ) )		//Only for pet
		return true;
	
	return false;
}

/**
 * \brief ºÚ∂Ã√Ë ˆ
 * 
 * œÍœ∏√Ë ˆ
 * 
 * \param pSkill : √Ë ˆ
 * \param dwLevel : √Ë ˆ
 * \return ∑µªÿ÷µµƒ√Ë ˆ
 */
bool ReSetSkillData(SkillBase* pSkill,DWORD dwLevel)
{
	FUNCTION_BEGIN;

	if(pSkill->dwLevel == dwLevel) return true;
	SkillLevelBase_t* pSkillLevel = GetSkillLevelBase(pSkill->dwID,dwLevel);
	//Assert(pSkillLevel);
	if(!pSkillLevel) return false;
	
	//back up skill up item
	DWORD upItemID = pSkill->dwUpItemID;

	memcpy(pSkill,pSkillLevel,sizeof(SkillLevelBase_t));
	//reset up item id
	pSkill->dwUpItemID = upItemID;

#ifdef _AUTOPLAY
	if(gAutoGameConfig.fnReSetSkillData)
		gAutoGameConfig.fnReSetSkillData(pSkill,dwLevel);
#endif
	return true;

	FUNCTION_END;
}