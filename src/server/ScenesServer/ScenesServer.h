/**
 * \file
 * \version  $Id: ScenesServer.h  $
 * \author  
 * \date 
 * \brief zebraÏîÄ¿³¡¾°·şÎñÆ÷£¬ÓÎÏ·¾ø´ó²¿·ÖÄÚÈİ¶¼ÔÚ±¾ÊµÏÖ
 */

#ifndef _ScenesServer_h_
#define _ScenesServer_h_

#include "zSubNetService.h"
#include "Zebra.h"
#include "SceneManager.h"
#include "zMisc.h"

//soke×ªÉúÔö¼ÓµÄ¼¼ÄÜºÍÊôĞÔµã
extern WORD trun_point_rate;
extern WORD trun_skill_rate;
/**
 * \brief ¶¨Òå³¡¾°·şÎñÀà
 *
 * ³¡¾°·şÎñÆ÷£¬ÓÎÏ·¾ø´ó²¿·ÖÄÚÈİ¶¼ÔÚ±¾ÊµÏÖ<br>
 * Õâ¸öÀàÊ¹ÓÃÁËSingletonÉè¼ÆÄ£Ê½£¬±£Ö¤ÁËÒ»¸ö½ø³ÌÖĞÖ»ÓĞÒ»¸öÀàµÄÊµÀı
 *
 */
class ScenesService : public zSubNetService
{

	public:
		int writeBackTimer;
		//sky ÌìÏÂµÚÒ»½á¹¹Ìå
		struct txdy_info
		{
			int userid;//½ÇÉ«id
			char username[MAX_NAMESIZE];//½ÇÉ«Ãû
			int saidian;//Èüµã
			int jifen;//»ı·Ö
		};
		txdy_info tianxia[32];

		//·´Íâ¹ÒÔİÍ£×´Ì¬
		bool fanwaigua;
		DWORD fanwaiguarongcuo;
		//ÃÔ¹¬ÏµÍ³ ³õÊ¼»¯±äÁ¿
		struct migong_info
		{
			int ceng;
			int password;
		};

		
		migong_info migong[25];
		
		SceneNpc * han_biaoche;//ºº¹úïÚ³µ
		SceneNpc * han_biaoche2;//ºº¹úïÚ³µ
		
		SceneNpc * chu_biaoche;//³ş¹úïÚ³µ 
		SceneNpc * chu_biaoche2;//³ş¹úïÚ³µ

		int hanbiaoche_type;
		int chubiaoche_type;
		///////////////////////////////
		//soke ´ò¹Ö¾­Ñé±¶Êı
		int double_exp;
		///////////////////////////////
		bool double_drop;
		BYTE levelDoubleDrop;
		bool msgParse_SuperService(const Cmd::t_NullCmd *ptNullCmd, const unsigned int nCmdLen);
		BYTE countryPower[13];
		
		/**
		 * \brief ĞéÎö¹¹º¯Êı
		 *
		 */
		virtual ~ScenesService()
		{
			instance = NULL;

			//¹Ø±ÕÏß³Ì³Ø
			if (taskPool)
			{
				taskPool->final();
				SAFE_DELETE(taskPool);
			}
		}

		const int getPoolSize() const
		{
			if(taskPool)
			{
				return taskPool->getSize();
			}
			else
			{
				return 0;
			}
		}

		inline static void zcadfss()
		{
			ScenesService * aazz = &ScenesService::getInstance();
			delete aazz;
		}

		/**
		 * \brief ·µ»ØÎ¨Ò»µÄÀàÊµÀı
		 *
		 * \return Î¨Ò»µÄÀàÊµÀı
		 */
		static ScenesService &getInstance()
		{
			if (NULL == instance)
				instance = new ScenesService();

			return *instance;
		}

		/**
		 * \brief ÊÍ·ÅÀàµÄÎ¨Ò»ÊµÀı
		 *
		 */
		static void delInstance()
		{
			SAFE_DELETE(instance);
		}

		void reloadConfig();
		void checkAndReloadConfig();
		bool isSequeueTerminate() 
		{
			return taskPool == NULL;
		}

		//GM_logger
		static zLogger* gm_logger;
		//ÎïÆ·log
		static zLogger* objlogger;
		//Íâ¹Ò_logger
		static zLogger* wg_logger;

		inline static bool zcaaadfss();

		static Cmd::stChannelChatUserCmd * pStampData;
		static DWORD updateStampData();
	private:

		/**
		 * \brief ÀàµÄÎ¨Ò»ÊµÀıÖ¸Õë
		 *
		 */
		static ScenesService *instance;
		/**
		 * \brief ÉèÖÃÖØĞÂ¶ÁÈ¡ÅäÖÃ±êÖ¾
		 *
		 */
		static bool reload;

		zTCPTaskPool *taskPool;				/**< TCPÁ¬½Ó³ØµÄÖ¸Õë */

		/**
		 * \brief ¹¹Ôìº¯Êı
		 *
		 */
		ScenesService() : zSubNetService("³¡¾°·şÎñÆ÷", SCENESSERVER)
		{
			writeBackTimer = 0;

			taskPool = NULL;

			/* P3-FIX: uninit members ----------------------------------------
			 * ä»¥ä¸‹æˆå‘˜åŸæ„é€ å‡½æ•°é‡Œä»æœªåˆå§‹åŒ–ã€‚
			 * MSVC çš„ new å¸¸æ‹¿åˆ°é›¶é¡µæ‰€ä»¥ Win ç‰ˆç¢°å·§ä¸ç‚¸ï¼›
			 * glibc çš„ operator new ç»™è„å†…å­˜ï¼Œhan_biaoche æˆä¸ºé‡æŒ‡é’ˆï¼Œ
			 * SceneTimeTick::run() ç¬¬ä¸€ä¸ª tick å°±å´©ã€‚ */
			memset(tianxia, 0, sizeof(tianxia));
			fanwaigua = false;
			fanwaiguarongcuo = 0;
			memset(migong, 0, sizeof(migong));
			han_biaoche = NULL;
			han_biaoche2 = NULL;
			chu_biaoche = NULL;
			chu_biaoche2 = NULL;
			hanbiaoche_type = 0;
			chubiaoche_type = 0;
			double_exp = 1;
			double_drop = false;
			levelDoubleDrop = 0;
			memset(countryPower, 0, sizeof(countryPower));
			/* ------------------------------------------------------------- */
		}

		bool init();
		void newTCPTask(const int sock, const struct sockaddr_in *addr);
		void final();
};

#endif

