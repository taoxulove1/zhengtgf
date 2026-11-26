#ifndef _OBJECT_H
#define _OBJECT_H

typedef unsigned long tItemThisID ;
#define INVALID_THISID 0xffffffff

// 道具的类型
enum enumItemType
{
	ItemType_None = 0,
	
	ItemType_Resource = 16,	//16代表原料类
	ItemType_Leechdom,	//17代表药品类
	ItemType_FoodRes,	//18代表食物原料类
	ItemType_Food,		//19代表食物类
	ItemType_Tools,		//20代表劳动工具类
	ItemType_Arrow,		//21代表配合弓使用的箭支类
	ItemType_FightHorse,	//22枣红战马
	ItemType_Pack,		//23代表包裹类
	ItemType_Money,		//24代表银子类
	ItemType_Scroll,	//25代表转移卷轴类
	ItemType_Move,		//26代表特殊移动道具类
	ItemType_LevelUp,	//27代表道具升级需要的材料类
	ItemType_AddLevelUpPercent,	//28代表增加道具升级成功率的物品.
	ItemType_Union,	//29代表创建帮会需要的道具.
	ItemType_Tonic,	//30表示自动补药类道具.
	ItemType_Gift,	//31代表礼品类物品.
	ItemType_Other, 
	ItemType_MASK = 33, 	//33代表蒙面巾
	ItemType_Quest = 34,
	ItemType_HORSE = 35,	//枣红马
	ItemType_SOULSTONE = 37, //37代表魂魄石类
	ItemType_Change = 41,   //41 代表合成道具
	ItemType_SkillUp = 44,   //43 代表技能升级道具
	ItemType_PackHide = 45,		// 代表仓库
	ItemType_ClearPoint,  // 46代表只能〈40级的玩家使用的洗点，40级以上为34(任务物品)
	ItemType_Repair = 47, //47代表修复宝石类
	ItemType_DoubleExp = 52, //52代表自动补给类型
    ItemType_Honor = 53, //53代表荣誉之星类型
	ItemType_State = 51, //51代表状态道具
	ItemType_ZhenSkillUp = 57,	//代表阵发书籍
	ItemType_FillHP = 58, //58代表大剂量道具
	ItemType_Amulet = 61, // 护身符类道具
	ItemType_FillMP = 62, //59代表自动补兰道具


	ItemType_ClothBody =101,		//101代表布质加生命类服装
	ItemType_FellBody =102,		    //102代表皮甲加魔防类服装
	ItemType_MetalBody =103,		//103代表金属铠甲加物防类服装
	ItemType_Blade =104,		    //104代表武术刀类武器
	ItemType_Sword =105,	        //105代表武术剑类武器
	ItemType_Axe =106,	           //106代表武术斧类武器
	ItemType_Hammer =107,	        //107代表武术斧类武器
	ItemType_Staff =108,		    //108代表法术杖类武器
	ItemType_Bow =109,	          //109代表箭术弓类武器
	ItemType_Fan =110,	           //110代表美女扇类
	ItemType_Stick =111,	        //111代表召唤棍类武器
	ItemType_Shield =112,	//112代表盾牌类
	ItemType_Helm =113,		//113代表角色头盔类
	ItemType_Caestus =114,	//114代表角色腰带类
	ItemType_Cuff = 115,		//115代表角色护腕类
	ItemType_Shoes = 116,		//116代表角色鞋子类
	ItemType_Necklace = 117,	//117代表角色项链类
	ItemType_Fing = 118,		//118代表角色戒指类

	ItemType_FashionBody = 119,		//119代表时装
	ItemType_Flower = 120,           //玫瑰花
	ItemType_BMW = 121,             //119代表宝马
};

#define BOW_ARROW_ITEM_ID 660
#define CROSSBOW_ARROW_ITEM_ID 661
#define CANNOT_SELL_ARROW_ID 800
#define LARGE_AMMOUNT_ARROW_ID	801

#define ARROW_TYPE_NUM	4
static const DWORD arrow_type[ARROW_TYPE_NUM] = { BOW_ARROW_ITEM_ID, CROSSBOW_ARROW_ITEM_ID, CANNOT_SELL_ARROW_ID, LARGE_AMMOUNT_ARROW_ID };


#pragma pack(1)

struct stObjectLocation{
	DWORD dwLocation;	// 格子类型
	DWORD dwTableID;	// 包袱ID
	WORD  x;
	WORD  y;
	bool operator == (const stObjectLocation & st) const
	{
		return dwLocation == st.dwLocation && dwTableID == st.dwTableID && x == st.x && y == st.y;
	}
};
struct oskill
{
	DWORD id;
	BYTE level;
	BYTE count;
	BYTE maxcount;
	BYTE other;
};

//typedef struct _Object
//{
//	DWORD qwThisID;   //物品唯一id
//	DWORD dwObjectID;  ////物品类别id
//	char strName[MAX_NAMESIZE]; //名称
//	
//	stObjectLocation pos;	// 位置
//	DWORD dwNum;	// 数量
//	BYTE upgrade;//物品升级等级
//	BYTE kind;	//物品类型, 0普通, 1蓝色, 2金色, 4神圣, 8套装
//	DWORD exp;  //道具经验
//	
//	WORD needlevel;				// 需要等级
//
//	WORD maxhp;					// 最大生命值
//	WORD maxmp;					// 最大法术值
//	WORD maxsp;					// 最大体力值
//
//	WORD pdamage;				// 最小攻击力
//	WORD maxpdamage;			// 最大攻击力
//	WORD mdamage;				// 最小法术攻击力
//	WORD maxmdamage;			// 最大法术攻击力
//
//	WORD pdefence;				// 物防
//	WORD mdefence;				// 魔防
//	WORD damagebonus;			// 伤害加成
//	
//	WORD akspeed;				// 攻击速度
//	WORD mvspeed;				// 移动速度
//	WORD atrating;				// 命中率
//	WORD akdodge;				// 躲避率
//
//	DWORD color;				// 颜色	
//
//	WORD str;  // 力量
//	WORD inte;  // 智力
//	WORD dex;  // 敏捷
//	WORD spi;  // 精神
//	WORD con;  // 体质
//	
//	WORD fivetype;  // 五行属性
//	WORD fivepoint; // 五行属性
//	
//	WORD hpr;  // 生命值恢复
//	WORD mpr;  // 法术值恢复
//	WORD spr;  // 体力值恢复
//
//	WORD holy;  //神圣一击	
//	WORD bang;  //重击
//	WORD pdam;  // 增加物理攻击力
//	WORD pdef;  // 增加物理防御力
//	WORD mdam;  // 增加魔法攻击力
//	WORD mdef;  // 增加魔法防御力
//	
//	WORD poisondef; //抗毒增加
//	WORD lulldef; //抗麻痹增加
//	WORD reeldef; //抗眩晕增加
//	WORD evildef; //抗噬魔增加
//	WORD bitedef; //抗噬力增加
//	WORD chaosdef; //抗混乱增加
//	WORD colddef; //抗冰冻增加
//	WORD petrifydef; //抗石化增加
//	WORD blinddef; //抗失明增加
//	WORD stabledef; //抗定身增加
//	WORD slowdef; //抗减速增加
//	WORD luredef; //抗诱惑增加
//		
//	WORD durpoint; //恢复装备耐久度点数
//	WORD dursecond; //恢复装备耐久度时间单位
//
//	struct skillbonus {
//		WORD id; //技能 id
//		WORD point; // 技能点数
//	} skill[10]; //技能加成
//
//	struct skillsbonus {
//		WORD id; //技能 id
//		WORD point; // 技能点数
//	} skills;	//全系技能加成
//
//	WORD poison; //中毒增加
//	WORD lull; //麻痹增加
//	WORD reel; //眩晕增加
//	WORD evil; //噬魔增加
//	WORD bite; //噬力增加
//	WORD chaos; //混乱增加
//	WORD cold; //冰冻增加
//	WORD petrify; //石化增加
//	WORD blind; //失明增加
//	WORD stable; //定身增加
//	WORD slow; //减速增加
//	WORD lure; //诱惑增加
//	
//	struct leech
//	{
//		BYTE odds;    //x
//		WORD effect;	//y
//	};
//	leech hpleech; //x%吸收生命值y
//	leech mpleech; //x%吸收法术值y
//	
//	BYTE hptomp; //转换生命值为法术值x％
//	BYTE dhpp; //物理伤害减少x%	
//	BYTE dmpp; //法术伤害值减少x%		
//
//	BYTE incgold; //增加银子掉落x%
//	BYTE doublexp; //x%双倍经验		
//	BYTE mf; //增加掉宝率x%
//
//	//五行套装相关属性
//	BYTE dpdam; //物理伤害减少%x
//	BYTE dmdam; //法术伤害减少%x
//	BYTE bdam; //增加伤害x%
//	BYTE rdam; //伤害反射%x
//	BYTE ignoredef; //%x忽视目标防御
//
//	WORD fiveset[5]; //五行套装, 按顺序排列
//
//	//...
//	BYTE width;  //宽度
//	BYTE height; //高度
//	WORD dur;    //当前耐久
//	WORD maxdur; //最大耐久
//	
//	DWORD socket[6]; //孔
//	DWORD price;     //价格
//	DWORD cardpoint; //点卡
//	char maker[MAX_NAMESIZE]; //打造者
//}t_Object;
typedef struct _Object
{
	DWORD qwThisID;   //物品唯一id
	DWORD dwObjectID;  ////物品类别id
	char strName[MAX_NAMESIZE]; //名称

	stObjectLocation pos;	// 位置
	DWORD dwNum;	// 数量
	BYTE upgrade;//物品升级等级
	BYTE kind;	//物品类型, 0普通, 1蓝色, 2金色, 4神圣, 8套装
	DWORD exp;  //道具经验

	WORD needlevel;				// 需要等级

	WORD maxhp;					// 最大生命值
	WORD maxmp;					// 最大法术值
	WORD maxsp;					// 最大体力值

	WORD pdamage;				// 最小攻击力
	WORD maxpdamage;			// 最大攻击力
	WORD mdamage;				// 最小法术攻击力
	WORD maxmdamage;			// 最大法术攻击力

	WORD pdefence;				// 物防
	WORD mdefence;				// 魔防
	BYTE damagebonus;			// 伤害加成 x% from 道具基本表
	BYTE damage;				// 增加伤害值x％ from 神圣装备表

	WORD akspeed;				// 攻击速度
	WORD mvspeed;				// 移动速度
	WORD atrating;				// 命中率
	WORD akdodge;				// 躲避率

	DWORD color;				// 颜色	 

	WORD str;  // 力量
	WORD inte;  // 智力
	WORD dex;  // 敏捷
	WORD spi;  // 精神
	WORD con;  // 体质

	WORD fivetype;  // 五行属性
	WORD fivepoint; // 五行属性

	WORD hpr;  // 生命值恢复
	WORD mpr;  // 法术值恢复
	WORD spr;  // 体力值恢复

	WORD holy;  //神圣一击	
	WORD bang;  //重击
	WORD pdam;  // 增加物理攻击力
	WORD pdef;  // 增加物理防御力
	WORD mdam;  // 增加魔法攻击力
	WORD mdef;  // 增加魔法防御力

	WORD poisondef; //抗毒增加
	WORD lulldef; //抗麻痹增加
	WORD reeldef; //抗眩晕增加
	WORD evildef; //抗噬魔增加
	WORD bitedef; //抗噬力增加
	WORD chaosdef; //抗混乱增加
	WORD colddef; //抗冰冻增加
	WORD petrifydef; //抗石化增加
	WORD blinddef; //抗失明增加
	WORD stabledef; //抗定身增加
	WORD slowdef; //抗减速增加
	WORD luredef; //抗诱惑增加

	WORD durpoint; //恢复装备耐久度点数
	WORD dursecond; //恢复装备耐久度时间单位

	struct skillbonus {
		WORD id; //技能 id
		WORD point; // 技能点数
	} skill[10]; //技能加成

	struct skillsbonus {
		WORD id; //技能 id
		WORD point; // 技能点数
	} skills;	//全系技能加成

	WORD poison; //中毒增加
	WORD lull; //麻痹增加
	WORD reel; //眩晕增加
	WORD evil; //噬魔增加
	WORD bite; //噬力增加
	WORD chaos; //混乱增加
	WORD cold; //冰冻增加
	WORD petrify; //石化增加
	WORD blind; //失明增加
	WORD stable; //定身增加
	WORD slow; //减速增加
	WORD lure; //诱惑增加

	struct leech
	{
		BYTE odds;    //x
		WORD effect;	//y
	};
	leech hpleech; //x%吸收生命值y
	leech mpleech; //x%吸收法术值y

	BYTE hptomp; //转换生命值为法术值x％
	BYTE dhpp; //物理伤害减少x%	
	BYTE dmpp; //法术伤害值减少x%		

	BYTE incgold; //增加银子掉落x%
	BYTE doublexp; //x%双倍经验		
	BYTE mf; //增加掉宝率x%

	BYTE bind;  //装备是否绑定

	//五行套装相关属性
	BYTE dpdam; //物理伤害减少%x
	BYTE dmdam; //法术伤害减少%x
	BYTE bdam; //增加伤害x%
	BYTE rdam; //伤害反射%x
	BYTE ignoredef; //%x忽视目标防御

	WORD fiveset[5]; //五行套装, 按顺序排列

	//...
	BYTE width;  //宽度
	BYTE height; //高度
	WORD dur;    //当前耐久
	WORD maxdur; //最大耐久

	DWORD socket[6]; //孔
	DWORD price;     //价格
	DWORD cardpoint; //点卡

	char maker[MAX_NAMESIZE]; //打造者

}t_Object;

//typedef struct _Object
//{
//	DWORD				qwThisID;
//	DWORD				dwObjectID;	// 物品ID
//	char				strName[MAX_NAMESIZE]; //名称
//	stObjectLocation	pos;	// 位置
//	DWORD				dwNum;	// 数量
//	DWORD				dwColor;// 物品颜色 0xffrrggbb
//	WORD needLevel; //需要等级
//	DWORD hp; //当前生命值
//	DWORD maxhp; //最大生命值
//	DWORD mp; //当前法术值
//	DWORD maxmp; //最大法术值
//	DWORD sp; //当前体力值
//	DWORD maxsp; //最大体力值
//	DWORD pdam; //最小攻击力
//	DWORD maxpdam; //最大攻击力
//	DWORD rdam; //最小远程伤害
//	DWORD maxrdam; //最大远程伤害
//	DWORD tdam; //最小仙术伤害
//	DWORD maxtdam; //最大仙术伤害
//	DWORD mdam; //最小法术攻击力
//	DWORD maxmdam; //最大法术攻击力
//	DWORD pdef; //最小防御力
//	DWORD maxpdef; //最大防御力
//	DWORD mdef; //最小法术防御力
//	DWORD maxmdef; //最大法术防御力
//	WORD hpr; //生命值恢复
//	WORD mpr; //法术值恢复
//	WORD spr; //体力值恢复
//	WORD atspeed; //攻击速度
//	WORD mgspeed; //施法速度
//	WORD mvspeed; //移动速度
//	WORD atrating; //命中率
//	WORD akdodge; //躲避率
//	WORD mgrating; //法术命中率
//	WORD mgdodge; //法术躲避率
//	WORD dur; //耐久度
//	WORD maxdur; //最大耐久度
//	DWORD price; //价格
//	BYTE kind;	//物品类型,0普通,1蓝色,2金色,4神圣, 8 套装
//	BYTE upgrade;//物品升级等级
//	union{
//		struct{
//			WORD gold; //金
//			WORD wood; //木
//			WORD soil; //土
//			WORD water; //水
//			WORD fire; //火
//		};
//		WORD fivevalue[5];
//	};
//	union{
//		struct{
//			WORD goldr; //金系抵抗
//			WORD woodr; //木系抵抗
//			WORD waterr; //土系抵抗
//			WORD firer; //水系抵抗
//			WORD soilr; //火系抵抗
//		};
//		WORD fivedefence[5];
//	};
//	WORD doublexp; //双倍经验
//	WORD pdefup; //防御力增强
//	WORD mdefup; //法术防御力增强
//	WORD poisondef; //抗毒增加
//	WORD colddef; //抗冰冻增加
//	WORD petrifydef; //抗石化增加
//	WORD blinddef; //抗失明增加
//	WORD chaosdef; //抗混乱增加
//	DWORD socket[6]; //孔
//	oskill skillo[6];//增加技能
//	WORD reflectp; //伤害反射x％
//	WORD reflect; //伤害反射（人物等级/x）
//	BYTE durpoint; //恢复装备耐久度点数
//	BYTE dursecond; //恢复装备耐久度描述
//	WORD holy; //神圣一击
//	WORD ehpp; //增加伤害值x％
//	WORD ehpv; //增加伤害值（人物等级/x）
//	WORD epdamp; //增加攻击力x％
//	WORD epdamv; //增加攻击力（人物等级/x）
//	WORD erdamp; //增加远程伤害x%
//	WORD erdamv; //增加远程伤害（人物等级/x）
//	WORD etdamp; //增加仙术x%
//	WORD etdamv; //增加仙术(人物等级/x)
//	WORD emdamp; //增加法术攻击力x%
//	WORD emdamv; //增加法术攻击力(人物等级/x)
//	WORD fivepro; //五行属性增加
//	WORD hpleech; //x%吸收生命值y％
//	WORD hpleech_value; 
//	WORD mpleech; //x%吸收法术值y％
//	WORD mpleech_value;
//	WORD hptomp; //转换生命值为法术值x％
//	WORD dhpv; //物理伤害值减少(人物等级/x)
//	WORD dhpp; //物理伤害减少x%
//	WORD dmpv; //法术伤害值减少(人物等级/x)
//	WORD dmpp; //法术伤害值减少x%
//	WORD invtarget; //x%忽视目标防御
//	WORD incgold; //增加银子掉落x%
//	WORD cold; //x%使目标冰冻
//	WORD poison; //x%使目标中毒
//	WORD petrify; //x%使目标石化
//	WORD chaos; //x%使目标混乱
//	WORD blind; //x%使目标失明
//	WORD mf; //增加掉宝率x%
//	WORD charm; //魅力增加x
//	WORD sleight; //熟练度增加x%
//	DWORD exp; //物品经验
//	char maker[MAX_NAMESIZE]; //打造者名称
//	char set[MAX_NAMESIZE]; //套装名称
//}t_Object;

#pragma pack()

#endif

