-- Zebra reference schema from the 59-table MySQL 4.0 snapshot.
-- Select an empty latin1 database before importing. This file contains schema only.

CREATE TABLE `1answer` (
  `id` bigint(20) unsigned NOT NULL auto_increment,
  `type` varchar(100) NOT NULL default '',
  `ask` blob NOT NULL,
  `answer_a` blob NOT NULL,
  `answer_b` blob NOT NULL,
  `answer_c` blob NOT NULL,
  `answer_d` blob NOT NULL,
  `answer_e` blob NOT NULL,
  `answer_f` blob NOT NULL,
  `the_answer` bigint(20) NOT NULL default '0',
  `quiz_type` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM;

CREATE TABLE `accpriv` (
  `ACCID` int(10) unsigned NOT NULL default '0',
  `PRIV` int(10) unsigned NOT NULL default '0'
) ENGINE=MyISAM;

CREATE TABLE `act` (
  `ID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(32) NOT NULL default '',
  `STATE` tinyint(3) unsigned NOT NULL default '0',
  `TEXT` varchar(255) NOT NULL default ''
) ENGINE=MyISAM;

CREATE TABLE `ally` (
  `COUNTRYID` int(10) unsigned NOT NULL default '0',
  `ALLYCOUNTRYID` int(10) unsigned NOT NULL default '0',
  `CREATETIME` int(10) unsigned NOT NULL default '0',
  `LASTUPTIME` int(10) unsigned NOT NULL default '0',
  `FRIENDDEGREE` int(10) unsigned NOT NULL default '0'
) ENGINE=MyISAM;

CREATE TABLE `allytime` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `DWID` int(10) unsigned NOT NULL default '0',
  `ALLYID` int(10) unsigned NOT NULL default '0',
  `TYPE` int(10) unsigned NOT NULL default '0',
  `TIME` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM;

CREATE TABLE `army` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `COUNTRYID` int(10) unsigned NOT NULL default '0',
  `CITYID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `GENNAME` varchar(33) NOT NULL default '',
  `GENID` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM;

CREATE TABLE `auction` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `OWNER` varchar(100) NOT NULL default '',
  `STATE` tinyint(3) unsigned NOT NULL default '0',
  `NAME` varchar(32) NOT NULL default '',
  `TYPE` tinyint(3) NOT NULL default '0',
  `QUALITY` tinyint(3) NOT NULL default '0',
  `NEEDLEVEL` smallint(5) NOT NULL default '0',
  `MINMONEY` int(10) unsigned NOT NULL default '0',
  `MAXMONEY` int(10) unsigned NOT NULL default '0',
  `MINGOLD` int(10) unsigned NOT NULL default '0',
  `MAXGOLD` int(10) unsigned NOT NULL default '0',
  `BIDDER` varchar(32) NOT NULL default '',
  `BIDDER2` varchar(32) NOT NULL default '',
  `STARTTIME` int(10) unsigned NOT NULL default '0',
  `ENDTIME` int(10) unsigned NOT NULL default '0',
  `ITEM` blob NOT NULL,
  `CHARGE` int(10) unsigned NOT NULL default '0',
  `BIDTYPE` tinyint(3) unsigned NOT NULL default '0',
  `OWNERID` int(10) unsigned NOT NULL default '0',
  `BIDDERID` int(10) unsigned NOT NULL default '0',
  `BIDDER2ID` int(10) unsigned NOT NULL default '0',
  `ITEMID` int(10) unsigned NOT NULL default '0',
  `OBJID` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`),
  KEY `NAME` (`STATE`,`TYPE`,`QUALITY`)
) ENGINE=MyISAM;

CREATE TABLE `balance` (
  `ACCOUNT` varchar(48) NOT NULL default '',
  `ACCID` int(10) unsigned NOT NULL default '0',
  `PASSWORD` varchar(16) default '',
  `ALLGOLDIN` int(10) unsigned NOT NULL default '0',
  `ALLGOLDOUT` int(10) unsigned NOT NULL default '0',
  `ALLMONEYIN` int(10) unsigned NOT NULL default '0',
  `ALLMONEYOUT` int(10) unsigned NOT NULL default '0',
  `LASTTIME` timestamp NOT NULL,
  `GOLD` int(10) unsigned NOT NULL default '0',
  `MONEY` int(10) unsigned NOT NULL default '0',
  `GOLDTAX` int(10) unsigned NOT NULL default '0',
  `MONEYTAX` int(10) unsigned NOT NULL default '0',
  `GOLDLIST` int(10) unsigned NOT NULL default '0',
  `MONEYLIST` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ACCID`)
) ENGINE=MyISAM;

CREATE TABLE `captain` (
  `ARMYID` int(10) unsigned NOT NULL default '0',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `CHARNAME` varchar(33) NOT NULL default '',
  `NPCNUM` int(10) NOT NULL default '0',
  PRIMARY KEY  (`ARMYID`,`CHARID`)
) ENGINE=MyISAM;

CREATE TABLE `cartoonpet` (
  `CARTOONID` int(10) unsigned NOT NULL auto_increment,
  `NAME` char(32) NOT NULL default '0',
  `NPCID` int(10) unsigned NOT NULL default '0',
  `MASTERID` int(10) unsigned NOT NULL default '0',
  `MASTERNAME` char(32) NOT NULL default '',
  `LEVEL` tinyint(3) unsigned NOT NULL default '0',
  `EXP` int(10) unsigned NOT NULL default '0',
  `MAXEXP` int(10) unsigned NOT NULL default '0',
  `ADDEXP` int(10) unsigned NOT NULL default '0',
  `STATE` tinyint(3) unsigned NOT NULL default '0',
  `ADOPTER` char(32) NOT NULL default '',
  `TIME` int(10) unsigned NOT NULL default '0',
  `SP` int(10) unsigned NOT NULL default '0',
  `MAXSP` int(10) unsigned NOT NULL default '0',
  `MASTERLEVEL` int(10) unsigned NOT NULL default '0',
  `REPAIR` tinyint(3) unsigned NOT NULL default '0',
  `SYSADOPT` int(10) unsigned NOT NULL default '0',
  `AUTOADOPT` tinyint(3) unsigned NOT NULL default '0',
  `AUTOCHARGE` tinyint(3) unsigned NOT NULL default '0',
  `STARTADOPT` int(10) unsigned NOT NULL default '0',
  `ADOPTCOUNT` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`CARTOONID`)
) ENGINE=MyISAM;

CREATE TABLE `cdaregamble` (
  `USERID` int(10) unsigned NOT NULL default '0',
  `ATTID` int(10) unsigned NOT NULL default '0',
  `DFID` int(10) unsigned NOT NULL default '0',
  `DARETIME` int(10) unsigned NOT NULL default '0',
  `HASFIRE` int(10) NOT NULL default '0',
  `NUM` int(10) unsigned NOT NULL default '0',
  `STATE` int(10) unsigned NOT NULL default '0',
  `WINNUM` int(10) NOT NULL default '0',
  `TEMPNUM` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`USERID`,`ATTID`,`DFID`,`DARETIME`,`STATE`)
) ENGINE=MyISAM;

CREATE TABLE `changename` (
  `USERID` int(10) unsigned NOT NULL default '0',
  `TIME` int(10) unsigned NOT NULL default '0',
  `OLDNAME` varchar(33) NOT NULL default '',
  `NEWNAME` varchar(33) NOT NULL default ''
) ENGINE=MyISAM;

CREATE TABLE `charbase` (
  `CHARID` int(10) unsigned NOT NULL auto_increment,
  `ACCID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `TYPE` smallint(5) unsigned NOT NULL default '0',
  `LEVEL` smallint(5) unsigned NOT NULL default '1',
  `FACE` smallint(5) unsigned NOT NULL default '0',
  `HAIR` int(10) unsigned NOT NULL default '0',
  `BODYCOLOR` int(10) unsigned NOT NULL default '0',
  `GOODNESS` int(10) unsigned NOT NULL default '0',
  `PKADDITION` int(10) unsigned NOT NULL default '0',
  `MAPID` int(10) unsigned NOT NULL default '0',
  `MAPNAME` varchar(33) default '',
  `X` int(10) unsigned NOT NULL default '0',
  `Y` int(10) unsigned NOT NULL default '0',
  `HP` int(10) unsigned NOT NULL default '0',
  `MP` int(10) unsigned NOT NULL default '0',
  `SP` int(10) unsigned NOT NULL default '0',
  `CREATEIP` int(10) unsigned NOT NULL default '0',
  `EXP` bigint(20) unsigned NOT NULL default '0',
  `LUCKY` smallint(5) unsigned NOT NULL default '0',
  `SKILLPOINTS` smallint(5) unsigned NOT NULL default '0',
  `POINTS` smallint(5) unsigned NOT NULL default '0',
  `COUNTRY` int(10) unsigned NOT NULL default '0',
  `UNIONID` int(10) unsigned NOT NULL default '0',
  `CONSORT` int(10) unsigned NOT NULL default '0',
  `SEPTID` int(10) unsigned NOT NULL default '0',
  `SCHOOLID` int(10) unsigned NOT NULL default '0',
  `SYSSET` int(10) unsigned zerofill NOT NULL default '0000000000',
  `FORBIDTALK` bigint(20) unsigned NOT NULL default '0',
  `BITMASK` int(10) unsigned NOT NULL default '0',
  `ONLINETIME` int(10) unsigned NOT NULL default '0',
  `AVAILABLE` tinyint(3) unsigned NOT NULL default '1',
  `LASTACTIVEDATE` timestamp NOT NULL,
  `MEN` int(10) unsigned NOT NULL default '0',
  `INT` int(10) unsigned NOT NULL default '0',
  `DEX` int(10) unsigned NOT NULL default '0',
  `STR` int(10) unsigned NOT NULL default '0',
  `CON` int(10) unsigned NOT NULL default '0',
  `RELIVEWEAKTIME` smallint(5) unsigned NOT NULL default '0',
  `GRACE` int(10) unsigned NOT NULL default '0',
  `EXPLOIT` int(10) unsigned NOT NULL default '0',
  `TIRETIME` tinyblob NOT NULL,
  `OFFLINETIME` int(10) unsigned NOT NULL default '0',
  `FIVETYPE` int(10) unsigned NOT NULL default '0',
  `FIVELEVEL` int(10) unsigned NOT NULL default '0',
  `ALLBINARY` blob,
  `ANSWERCOUNT` int(10) unsigned NOT NULL default '0',
  `MONEY` int(10) unsigned NOT NULL default '0',
  `HONOR` int(10) unsigned NOT NULL default '5800',
  `GOMAPTYPE` int(10) unsigned NOT NULL default '0',
  `MAXHONOR` int(10) unsigned NOT NULL default '0',
  `MSGTIME` int(10) unsigned NOT NULL default '0',
  `ACCPRIV` int(10) unsigned NOT NULL default '0',
  `GOLD` int(10) unsigned NOT NULL default '0',
  `TICKET` int(10) unsigned NOT NULL default '0',
  `CREATETIME` int(10) unsigned NOT NULL default '0',
  `GOLDGIVE` int(10) unsigned NOT NULL default '0',
  `PETPACK` int(3) unsigned NOT NULL default '0',
  `PETPOINT` int(10) unsigned NOT NULL default '0',
  `LEVELSEPT` int(3) unsigned NOT NULL default '0',
  `PUNISHTIME` int(10) unsigned NOT NULL default '0',
  `TRAINTIME` int(10) unsigned NOT NULL default '0',
  `CAPTION` varchar(33) NOT NULL default '',
  `ZONE_STATE` int(10) unsigned NOT NULL default '0',
  `SOURCE_ID` int(10) unsigned NOT NULL default '0',
  `SOURCE_ZONE` int(10) unsigned NOT NULL default '0',
  `TARGET_ZONE` int(10) unsigned NOT NULL default '0',
  `SALARY` int(10) unsigned NOT NULL default '50000',
  `LASTIP` int(10) unsigned NOT NULL default '0',
  `ROUND` int(10) unsigned NOT NULL default '0',
  `NUMPASSWD` int(10) unsigned NOT NULL default '4294967294',
  `NUMPASSWDCANCELTIME` int(10) unsigned NOT NULL default '0',
  `TOTALCHARGE` int(10) default NULL,
  `LASTONLINETIME` int(10) unsigned NOT NULL default '0',
  `THISONLINETIME` int(10) unsigned NOT NULL default '0',
  `THISDAY` int(10) unsigned NOT NULL default '0',
  `HORSELEVEL` int(10) unsigned NOT NULL default '0',
  `HORSETRAINTIME` int(10) unsigned NOT NULL default '0',
  `ADDEXPRATE` int(10) unsigned NOT NULL default '0',
  `ADDEXPTIME` int(10) unsigned NOT NULL default '0',
  `SEPTTANKID` int(10) unsigned NOT NULL default '0',
  `RETYPOINT` int(10) unsigned NOT NULL default '0',
  `RMBPOINT` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`CHARID`),
  KEY `ACCID` (`ACCID`),
  KEY `NAME` (`NAME`)
) ENGINE=MyISAM;

CREATE TABLE `chartest` (
  `NAME` varchar(33) NOT NULL default '',
  `LEVEL` int(10) unsigned NOT NULL default '1',
  `UPDATETIME` int(10) unsigned NOT NULL default '0',
  `UPDATEUSETIME` int(10) unsigned NOT NULL default '0',
  `DEATHTIMES` int(10) NOT NULL default '0',
  `HPLEECHDOM` int(10) unsigned NOT NULL default '0',
  `MPLEECHDOM` int(10) unsigned NOT NULL default '0',
  `SPLEECHDOM` int(10) unsigned NOT NULL default '0',
  `GETMONEY` int(10) unsigned NOT NULL default '0',
  `GETHEIGH` int(10) unsigned NOT NULL default '0',
  `GETSOCKET` int(10) unsigned NOT NULL default '0',
  `GETMATERIAL` int(10) unsigned NOT NULL default '0',
  `GETSTONE` int(10) unsigned NOT NULL default '0',
  `GETSCROLL` int(10) unsigned NOT NULL default '0',
  `MONEY` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`NAME`,`LEVEL`)
) ENGINE=MyISAM;

CREATE TABLE `city` (
  `COUNTRY` int(10) unsigned NOT NULL default '0',
  `CITYID` int(10) unsigned NOT NULL default '0',
  `UNIONID` int(10) unsigned NOT NULL default '0',
  `LASTAWARDTIME` timestamp NOT NULL,
  `ISAWARD` int(11) NOT NULL default '0',
  `DAREUNIONID` int(10) unsigned NOT NULL default '0',
  `DAREUNIONLIST` varchar(255) NOT NULL default '',
  `GOLD` int(10) unsigned NOT NULL default '0',
  `CATCHERNAME` varchar(33) NOT NULL default '',
  `LASTDAILYMONEY` int(10) unsigned NOT NULL default '0'
) ENGINE=MyISAM;

CREATE TABLE `consigngold` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `ACCID` int(10) unsigned NOT NULL default '0',
  `NUM` int(10) unsigned NOT NULL default '0',
  `PRICE` int(10) unsigned NOT NULL default '0',
  `TIME` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`),
  KEY `ACCID` (`ACCID`),
  KEY `PRICE` (`PRICE`,`NUM`)
) ENGINE=MyISAM;

CREATE TABLE `consigngoldhistory` (
  `ID` int(10) NOT NULL default '0',
  `ACCID` int(10) unsigned NOT NULL default '0',
  `NUM` int(10) unsigned NOT NULL default '0',
  `COMMITPRICE` int(10) unsigned NOT NULL default '0',
  `PRICE` int(10) unsigned NOT NULL default '0',
  `COMMITTIME` int(10) unsigned NOT NULL default '0',
  `SYSMONEY` int(10) unsigned NOT NULL default '0',
  `OKTIME` int(10) unsigned NOT NULL default '0',
  KEY `ACCID` (`ACCID`),
  KEY `OKTIME` (`OKTIME`)
) ENGINE=MyISAM;

CREATE TABLE `consignmoney` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `ACCID` int(10) unsigned NOT NULL default '0',
  `NUM` int(10) unsigned NOT NULL default '0',
  `PRICE` int(10) unsigned NOT NULL default '0',
  `TIME` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`),
  KEY `ACCID` (`ACCID`),
  KEY `PRICE` (`PRICE`,`NUM`)
) ENGINE=MyISAM;

CREATE TABLE `consignmoneyhistory` (
  `ID` int(10) NOT NULL default '0',
  `ACCID` int(10) unsigned NOT NULL default '0',
  `NUM` int(10) unsigned NOT NULL default '0',
  `COMMITPRICE` int(10) unsigned NOT NULL default '0',
  `PRICE` int(10) unsigned NOT NULL default '0',
  `COMMITTIME` int(10) unsigned NOT NULL default '0',
  `SYSMONEY` int(10) unsigned NOT NULL default '0',
  `OKTIME` int(10) unsigned NOT NULL default '0',
  KEY `ACCID` (`ACCID`),
  KEY `OKTIME` (`OKTIME`)
) ENGINE=MyISAM;

CREATE TABLE `country` (
  `ID` int(10) unsigned NOT NULL default '0',
  `KINGNAME` varchar(100) NOT NULL default '',
  `KINGUNIONID` int(10) unsigned NOT NULL default '0',
  `DARETIME` int(10) unsigned NOT NULL default '0',
  `LASTDARETIME` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `FORMALWIN` int(10) unsigned NOT NULL default '0',
  `FORMALFAIL` int(10) unsigned NOT NULL default '0',
  `ANNOYWIN` int(10) unsigned NOT NULL default '0',
  `ANNOYFAIL` int(10) unsigned NOT NULL default '0',
  `DARECOUNTRYID` int(10) unsigned NOT NULL default '0',
  `STAR` int(10) unsigned NOT NULL default '0',
  `TAX` int(10) unsigned NOT NULL default '0',
  `GOLD` bigint(20) unsigned NOT NULL default '0',
  `SILK` bigint(20) unsigned NOT NULL default '0',
  `ORE` bigint(20) unsigned NOT NULL default '0',
  `BOWLDER` bigint(20) unsigned NOT NULL default '0',
  `WOOD` bigint(20) unsigned NOT NULL default '0',
  `COAT` bigint(20) unsigned NOT NULL default '0',
  `HERBAL` bigint(20) unsigned NOT NULL default '0',
  `MATERIAL` bigint(20) unsigned NOT NULL default '0',
  `NOTE` varchar(255) NOT NULL default '',
  `STOCK` bigint(20) unsigned NOT NULL default '0',
  `FORBIDTALK` int(10) NOT NULL default '0',
  `SENDPRISON` int(10) NOT NULL default '0',
  `GEN_EXP` int(10) unsigned NOT NULL default '0',
  `GEN_MAXEXP` int(10) unsigned NOT NULL default '0',
  `GEN_LEVEL` int(10) unsigned NOT NULL default '0',
  `DIPLOMATNAME` varchar(33) NOT NULL default '',
  `CATCHERNAME` varchar(33) NOT NULL default '',
  `CALLTIMES` int(10) unsigned NOT NULL default '0',
  `LASTDAILYMONEY` int(10) unsigned NOT NULL default '0',
  `KINGTIME` int(10) unsigned NOT NULL default '0',
  `KINGPOWER` int(10) unsigned NOT NULL default '100',
  `LAST_DWID` int(10) unsigned NOT NULL default '0',
  `LAST_TIMES` int(10) unsigned NOT NULL default '0',
  `SURPRISE_TIME` int(10) unsigned NOT NULL default '0',
  `SURPRISE_DARE_COUNTRY` int(10) unsigned NOT NULL default '0',
  `LAST_SURPRISE_TIME` int(10) unsigned NOT NULL default '0',
  `YUSHINAME1` varchar(100) NOT NULL default '',
  `YUSHINAME2` varchar(100) NOT NULL default '',
  `CAPTION_TIMES` int(10) unsigned NOT NULL default '0',
  `PKRATING` int(10) unsigned NOT NULL default '0',
  `COUNTRYLEVEL` int(10) unsigned NOT NULL default '0',
  `STRONG_COUNTRY` int(10) unsigned NOT NULL default '1',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM;

CREATE TABLE `darerecord` (
  `ID` int(11) NOT NULL auto_increment,
  `DARETIME` int(10) unsigned NOT NULL default '0',
  `ATTCOUNTRY` int(10) unsigned NOT NULL default '0',
  `DEFCOUNTRY` int(10) unsigned NOT NULL default '0',
  `ATTKINGNAME` varchar(33) NOT NULL default '',
  `DEFKINGNAME` varchar(33) NOT NULL default '',
  `RESULT` int(10) unsigned NOT NULL default '0',
  `ATTNUM` int(10) unsigned NOT NULL default '0',
  `DFNUM` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM;

CREATE TABLE `dare_caption` (
  `COUNTRYID` int(10) unsigned NOT NULL default '0',
  `ID` int(10) unsigned NOT NULL default '0',
  `DARE_CAPTION` varchar(33) NOT NULL default ''
) ENGINE=MyISAM;

CREATE TABLE `emperorforbid` (
  `DATA` char(40) binary NOT NULL default '0'
) ENGINE=MyISAM;

CREATE TABLE `forbidtalk` (
  `NAME` varchar(33) NOT NULL default '',
  `LEVEL` smallint(5) unsigned NOT NULL default '0',
  `ACCID` int(10) unsigned NOT NULL default '0',
  `SERVER` int(10) unsigned NOT NULL default '0',
  `COUNTRY` int(10) unsigned NOT NULL default '0',
  `REASON` varchar(255) NOT NULL default '',
  `OPERATION` smallint(5) unsigned NOT NULL default '0',
  `STARTTIME` bigint(20) unsigned NOT NULL default '0',
  `DELAY` int(10) NOT NULL default '0',
  `ISVALID` smallint(5) unsigned NOT NULL default '0',
  `GM` varchar(33) NOT NULL default '0',
  PRIMARY KEY  (`NAME`)
) ENGINE=MyISAM;

CREATE TABLE `fund` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `CHARID` int(10) unsigned NOT NULL default '0',
  `BUYTIME` int(10) unsigned NOT NULL default '0',
  `LEVEL` smallint(5) unsigned NOT NULL default '1',
  `SUM` smallint(5) unsigned NOT NULL default '1',
  `LASTLEVEL` smallint(5) unsigned NOT NULL default '0',
  `ACCID` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`),
  KEY `FINDEX` (`CHARID`)
) ENGINE=MyISAM;

CREATE TABLE `gametime` (
  `GAMETIME` bigint(20) unsigned NOT NULL default '0'
) ENGINE=MyISAM;

CREATE TABLE `gift` (
  `ACTID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(32) NOT NULL default '',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `ITEMGOT` tinyint(3) unsigned NOT NULL default '0',
  `ITEMID` int(10) unsigned NOT NULL default '0',
  `ITEMTYPE` tinyint(3) unsigned NOT NULL default '0',
  `ITEMNUM` int(10) unsigned NOT NULL default '0',
  `BIND` tinyint(3) unsigned NOT NULL default '0',
  `MONEY` int(10) unsigned NOT NULL default '0',
  `GOLD` int(10) unsigned NOT NULL default '0',
  `SOURCE_ID` int(10) unsigned NOT NULL default '0',
  `SOURCE_ZONE` int(10) unsigned NOT NULL default '0',
  `ZONE_STATE` int(10) unsigned NOT NULL default '0',
  `TARGET_ZONE` int(10) unsigned NOT NULL default '0',
  `TID` varchar(33) NOT NULL default '',
  `MAILTEXT` varchar(255) NOT NULL default ''
) ENGINE=MyISAM;

CREATE TABLE `herolist` (
  `COUNTRYID` tinyint(3) unsigned NOT NULL default '0',
  `DATEVALUE` tinyint(3) unsigned NOT NULL default '0',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `NUM` int(10) unsigned NOT NULL default '0',
  `CHARNAME` varchar(33) NOT NULL default '',
  `KILLTIME` time NOT NULL default '00:00:00',
  `GETJIFEN` tinyint(3) unsigned NOT NULL default '0',
  KEY `cindex` (`COUNTRYID`)
) ENGINE=MyISAM;

CREATE TABLE `home` (
  `OWNERNAME` varchar(32) NOT NULL default '',
  `LEVEL` tinyint(3) unsigned NOT NULL default '0',
  `TYPE` tinyint(3) unsigned NOT NULL default '0',
  `GROWTH` int(10) unsigned NOT NULL default '0',
  `CHECKFUNCTIME` int(10) unsigned NOT NULL default '0',
  `FURNITURENUM` tinyint(3) unsigned NOT NULL default '0',
  `FLAG` int(10) unsigned NOT NULL default '0',
  `VISITTIME` int(10) unsigned NOT NULL default '0',
  `PETID` int(10) unsigned NOT NULL default '0',
  `PETNAME` varchar(32) NOT NULL default '',
  `HUNGER` tinyint(3) unsigned NOT NULL default '0',
  `CHECKHUNGERTIME` int(10) unsigned NOT NULL default '0',
  `HAPPY` tinyint(3) unsigned NOT NULL default '0',
  `CHECKEXPTIME` int(10) unsigned NOT NULL default '0',
  `ADDEXP` int(10) unsigned NOT NULL default '0',
  `BIN` blob,
  `MSGS` blob,
  `HOSTESSNAME` varchar(32) default NULL,
  `VISITCOUNT` int(10) unsigned NOT NULL default '0',
  `HOMENAME` varchar(32) default NULL,
  `HOMEID` int(10) unsigned NOT NULL auto_increment,
  `ADDEXP2` int(10) unsigned NOT NULL default '0',
  `OWNERLEVEL` int(10) unsigned NOT NULL default '0',
  `HOSTESSLEVEL` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`HOMEID`),
  UNIQUE KEY `name` (`OWNERNAME`)
) ENGINE=MyISAM;

CREATE TABLE `kingname` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `NAME` varchar(33) default 'NULL',
  `COUNTRY` int(10) unsigned NOT NULL default '0',
  `TIME` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM;

CREATE TABLE `login` (
  `USERID` int(10) unsigned NOT NULL auto_increment,
  `LOGINID` varchar(32) default NULL,
  `PASSWORD` varchar(16) NOT NULL default '',
  `ISUSED` tinyint(3) unsigned NOT NULL default '0',
  `ISFORBID` tinyint(3) unsigned NOT NULL default '0',
  `NAME` varchar(32) default NULL,
  `BIRTH` varchar(16) default NULL,
  `PHONE` varchar(32) default NULL,
  `QUESTION1` varchar(64) default NULL,
  `ANSWER1` varchar(64) default NULL,
  `QUESTION2` varchar(64) default NULL,
  `ANSWER2` varchar(64) default NULL,
  `EMAIL` varchar(64) default NULL,
  `IDCARD` varchar(32) default NULL,
  `MOBILE` varchar(32) default NULL,
  `COMMENDNAME` varchar(32) default NULL,
  `CREATEDATE` datetime default NULL,
  `LASTACTIVEDATE` datetime default NULL,
  `VIPENDDATE` datetime default NULL,
  PRIMARY KEY  (`USERID`),
  KEY `unique` (`LOGINID`)
) ENGINE=MyISAM;

CREATE TABLE `loginstat` (
  `STATDAY` varchar(64) NOT NULL default '',
  `LOGINCOUNT` int(10) unsigned NOT NULL default '0',
  `LOGINSUCCESSCOUNT` int(10) unsigned NOT NULL default '0',
  `CREATEACCOUNT` int(10) unsigned NOT NULL default '0',
  `CREATESUCCESSACCOUNT` int(10) unsigned NOT NULL default '0',
  `CREATERECORD` int(10) unsigned NOT NULL default '0',
  `CREATESUCCESSRECORD` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`STATDAY`)
) ENGINE=MyISAM;

CREATE TABLE `mail` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `STATE` tinyint(3) unsigned NOT NULL default '0',
  `FROMNAME` varchar(32) NOT NULL default '',
  `TONAME` varchar(32) NOT NULL default '',
  `TITLE` varchar(32) NOT NULL default '',
  `TYPE` tinyint(3) unsigned NOT NULL default '0',
  `CREATETIME` int(10) unsigned NOT NULL default '0',
  `DELTIME` int(10) unsigned NOT NULL default '0',
  `ACCESSORY` tinyint(3) unsigned NOT NULL default '0',
  `ITEMGOT` tinyint(3) unsigned NOT NULL default '0',
  `TEXT` varchar(255) NOT NULL default '',
  `SENDMONEY` int(10) unsigned NOT NULL default '0',
  `RECVMONEY` int(10) unsigned NOT NULL default '0',
  `SENDGOLD` int(10) unsigned NOT NULL default '0',
  `RECVGOLD` int(10) unsigned NOT NULL default '0',
  `BIN` blob,
  `TOID` int(10) unsigned NOT NULL default '0',
  `FROMID` int(10) unsigned NOT NULL default '0',
  `ITEMID` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`),
  KEY `two` (`STATE`,`DELTIME`,`ACCESSORY`),
  KEY `three` (`TONAME`,`STATE`,`ID`,`ACCESSORY`)
) ENGINE=MyISAM;

CREATE TABLE `minigame` (
  `CHARID` int(10) unsigned NOT NULL default '0',
  `NAME` char(32) NOT NULL default '',
  `COUNTRY` int(10) unsigned NOT NULL default '0',
  `FACE` int(10) unsigned NOT NULL default '0',
  `WIN` int(10) unsigned NOT NULL default '0',
  `LOSE` int(10) unsigned NOT NULL default '0',
  `DRAW` int(10) unsigned NOT NULL default '0',
  `MONEY` int(10) unsigned NOT NULL default '0',
  `SCORE` int(10) NOT NULL default '0',
  PRIMARY KEY  (`CHARID`),
  KEY `TOP` (`SCORE`)
) ENGINE=MyISAM;

CREATE TABLE `npcdare` (
  `COUNTRY` int(10) unsigned NOT NULL default '0',
  `MAPID` int(10) unsigned NOT NULL default '0',
  `NPCID` int(10) unsigned NOT NULL default '0',
  `POSX` int(10) unsigned NOT NULL default '0',
  `POSY` int(10) unsigned NOT NULL default '0',
  `HOLDSEPTID` int(10) unsigned NOT NULL default '0',
  `DARESEPTID` int(10) unsigned NOT NULL default '0',
  `GOLD` int(10) unsigned NOT NULL default '0'
) ENGINE=MyISAM;

CREATE TABLE `pkgame` (
  `CHARID` int(10) unsigned NOT NULL default '0',
  `NAME` char(32) NOT NULL default '',
  `LEVEL` int(10) unsigned NOT NULL default '0',
  `GAMEID` int(10) unsigned NOT NULL default '0',
  `TEAMID` int(10) unsigned NOT NULL default '0',
  `JOB` int(10) unsigned NOT NULL default '0',
  `TITLE` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`CHARID`),
  UNIQUE KEY `name` (`NAME`),
  KEY `GAMEID` (`GAMEID`,`TEAMID`)
) ENGINE=MyISAM;

CREATE TABLE `recommend` (
  `ID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `TOTAL` int(10) unsigned NOT NULL default '0',
  `BALANCE` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM;

CREATE TABLE `recommendsub` (
  `ID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `LASTLEVEL` int(10) unsigned NOT NULL default '0',
  `TOTAL` int(10) unsigned NOT NULL default '0',
  `LASTTIME` int(10) unsigned NOT NULL default '0',
  `RECOMMENDID` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM;

CREATE TABLE `restorecard` (
  `ACCID` int(10) unsigned NOT NULL default '0',
  `SUBAT` int(10) unsigned NOT NULL default '0',
  `BALANCE` int(10) unsigned NOT NULL default '0',
  `TID` char(100) NOT NULL default '',
  PRIMARY KEY  (`ACCID`)
) ENGINE=MyISAM;

CREATE TABLE `restoregold` (
  `ACCID` int(10) unsigned NOT NULL default '0',
  `RESTOREGOLD` int(10) unsigned NOT NULL default '0',
  `BALANCE` int(10) unsigned NOT NULL default '0',
  `TID` char(100) NOT NULL default '',
  PRIMARY KEY  (`ACCID`)
) ENGINE=MyISAM;

CREATE TABLE `samplerelation` (
  `CHARID` int(10) unsigned NOT NULL default '0',
  `RELATIONID` int(10) unsigned NOT NULL default '0',
  `RELATIONNAME` varchar(33) NOT NULL default '',
  `TYPE` tinyint(3) unsigned NOT NULL default '0',
  `LASTTIME` int(10) unsigned NOT NULL default '0',
  `OCCUPATION` smallint(5) unsigned NOT NULL default '0',
  `DEGREE` smallint(5) unsigned NOT NULL default '0',
  PRIMARY KEY  (`CHARID`,`RELATIONID`)
) ENGINE=MyISAM;

CREATE TABLE `school` (
  `SCHOOLID` int(10) unsigned NOT NULL auto_increment,
  `NAME` varchar(33) NOT NULL default '',
  `MASTERSERIAL` int(10) unsigned NOT NULL default '0',
  `BULLETIN` blob NOT NULL,
  PRIMARY KEY  (`SCHOOLID`)
) ENGINE=MyISAM;

CREATE TABLE `schoolmember` (
  `SERIALID` int(10) unsigned NOT NULL auto_increment,
  `MASTERID` int(10) unsigned NOT NULL default '0',
  `PRESERIALID` int(10) unsigned NOT NULL default '0',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `LEVEL` smallint(5) unsigned NOT NULL default '0',
  `JOINTIME` int(10) unsigned NOT NULL default '0',
  `DEGREE` smallint(5) unsigned NOT NULL default '0',
  `LASTTIME` int(10) unsigned NOT NULL default '0',
  `SCHOOLID` int(10) unsigned NOT NULL default '0',
  `OCCUPATION` smallint(5) unsigned NOT NULL default '0',
  `TAG` tinyint(3) unsigned NOT NULL default '0',
  `MASTERBALANCE` int(10) unsigned NOT NULL default '0',
  `PRENTICETOTAL` int(10) unsigned NOT NULL default '0',
  `PRENTICELASTLVL` int(10) unsigned NOT NULL default '0',
  `MASTERTOTAL` bigint(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (`SERIALID`)
) ENGINE=MyISAM;

CREATE TABLE `sept` (
  `SEPTID` int(10) unsigned NOT NULL auto_increment,
  `NAME` varchar(33) NOT NULL default '',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `MASTER` varchar(33) NOT NULL default '',
  `VOTE` tinyint(3) unsigned NOT NULL default '0',
  `CREATETIME` int(10) unsigned NOT NULL default '0',
  `COUNTRYID` int(10) unsigned NOT NULL default '0',
  `REPUTE` int(10) unsigned NOT NULL default '0',
  `UNIONID` int(10) unsigned NOT NULL default '0',
  `LEVEL` int(10) unsigned NOT NULL default '0',
  `NOTE` varchar(255) NOT NULL default '',
  `SPENDGOLD` int(10) unsigned NOT NULL default '0',
  `ISEXP` int(10) unsigned NOT NULL default '0',
  `NORMALEXPTIME` int(10) unsigned NOT NULL default '0',
  `CALLTIMES` int(10) unsigned NOT NULL default '0',
  `CALLDAYTIME` int(10) unsigned NOT NULL default '0',
  `LEAVETIME` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`SEPTID`,`CHARID`)
) ENGINE=MyISAM;

CREATE TABLE `septmember` (
  `SEPTID` int(10) unsigned NOT NULL default '0',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `OCCUPATION` smallint(5) unsigned NOT NULL default '0',
  `ALIASNAME` varchar(33) NOT NULL default '',
  `USETANK` tinyint(3) unsigned NOT NULL default '1',
  PRIMARY KEY  (`SEPTID`,`CHARID`)
) ENGINE=MyISAM;

CREATE TABLE `serverlist` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `TYPE` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(32) NOT NULL default '',
  `IP` varchar(16) NOT NULL default '127.0.0.1',
  `PORT` int(10) unsigned NOT NULL default '0',
  `EXTIP` varchar(16) NOT NULL default '127.0.0.1',
  `EXTPORT` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM;

CREATE TABLE `sn` (
  `SN` varchar(16) NOT NULL default '',
  PRIMARY KEY  (`SN`)
) ENGINE=MyISAM;

CREATE TABLE `sortlist` (
  `CHARID` int(10) unsigned NOT NULL default '0',
  `LEVEL` smallint(5) unsigned NOT NULL default '0',
  `EXP` bigint(20) unsigned NOT NULL default '0',
  PRIMARY KEY  (`CHARID`)
) ENGINE=MyISAM;

CREATE TABLE `tank` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `SEPTID` int(10) unsigned NOT NULL default '0',
  `NAME` char(17) NOT NULL default '',
  `LEVEL` tinyint(3) unsigned NOT NULL default '1',
  `HP` int(10) unsigned NOT NULL default '50000',
  `MAXHP` int(10) unsigned NOT NULL default '50000',
  `FULLHP` int(10) unsigned NOT NULL default '50000',
  `ATK` int(10) unsigned NOT NULL default '500',
  `DEF` int(10) unsigned NOT NULL default '250',
  `TYPE` tinyint(3) unsigned NOT NULL default '0',
  `BUFFTYPE` tinyint(3) unsigned NOT NULL default '0',
  `ATKBUFF` tinyint(3) unsigned NOT NULL default '20',
  `DEFBUFF` tinyint(3) unsigned NOT NULL default '15',
  `HPBUFF` tinyint(3) unsigned NOT NULL default '30',
  `MOVEINTERVAL` int(5) unsigned NOT NULL default '640',
  `FIREINTERVAL` int(5) unsigned NOT NULL default '1000',
  `available` tinyint(3) unsigned NOT NULL default '1',
  `BORROWER` char(32) NOT NULL default '',
  `critical` tinyint(3) unsigned NOT NULL default '5',
  PRIMARY KEY  (`ID`),
  KEY `sept` (`SEPTID`)
) ENGINE=MyISAM;

CREATE TABLE `tbl_bonus` (
  `ACCID` int(4) unsigned NOT NULL default '0',
  `BONUS` bigint(8) NOT NULL default '0',
  `BALANCE` bigint(8) NOT NULL default '0',
  `HADFILLED` int(4) NOT NULL default '0',
  `TOTALCHARGE` int(10) default NULL,
  PRIMARY KEY  (`ACCID`)
) ENGINE=MyISAM;

CREATE TABLE `tbl_pcard` (
  `PCARDID` char(32) NOT NULL default '',
  `ISUSED` int(4) NOT NULL default '0',
  `POINT` int(4) NOT NULL default '0',
  `UID` int(4) NOT NULL default '0',
  `CHARGETIME` datetime NOT NULL default '0000-00-00 00:00:00',
  `IP` char(32) default NULL,
  `gamezone` int(4) default '0',
  PRIMARY KEY  (`PCARDID`)
) ENGINE=MyISAM;

CREATE TABLE `tech` (
  `UID` int(10) unsigned NOT NULL auto_increment,
  `COUNTRYID` int(10) unsigned NOT NULL default '0',
  `TYPE` int(10) unsigned NOT NULL default '0',
  `NAME` varchar(33) NOT NULL default '',
  `PROGRESS` int(10) unsigned NOT NULL default '0',
  `RESEARCHID` int(10) unsigned default '0',
  `RESEARCHNAME` varchar(33) default '',
  `LEVEL` int(10) unsigned NOT NULL default '1',
  `STATUS` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`UID`)
) ENGINE=MyISAM;

CREATE TABLE `union` (
  `UNIONID` int(10) unsigned NOT NULL auto_increment,
  `NAME` varchar(33) NOT NULL default '',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `LEVEL` smallint(5) NOT NULL default '0',
  `MASTER` varchar(33) NOT NULL default '',
  `EXP` bigint(20) NOT NULL default '0',
  `VOTE` tinyint(3) unsigned NOT NULL default '0',
  `CREATETIME` int(10) unsigned NOT NULL default '0',
  `COUNTRYID` int(10) unsigned NOT NULL default '0',
  `MANA` int(10) unsigned NOT NULL default '0',
  `ACTIONPOINT` int(10) unsigned NOT NULL default '0',
  `MONEY` int(10) unsigned NOT NULL default '0',
  `NOTE` varchar(255) NOT NULL default '',
  `CALLTIMES` int(10) NOT NULL default '0',
  PRIMARY KEY  (`UNIONID`,`CHARID`)
) ENGINE=MyISAM;

CREATE TABLE `unionmember` (
  `UNIONID` int(10) unsigned NOT NULL default '0',
  `CHARID` int(10) unsigned NOT NULL default '0',
  `ALIASNAME` varchar(33) NOT NULL default '',
  `NAME` varchar(33) NOT NULL default '',
  `OCCUPATION` smallint(5) unsigned NOT NULL default '0',
  `POWER` int(10) unsigned NOT NULL default '0',
  `SEPTID` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`UNIONID`,`CHARID`)
) ENGINE=MyISAM;

CREATE TABLE `vote` (
  `ID` int(10) unsigned NOT NULL auto_increment,
  `COUNTRYID` int(10) unsigned NOT NULL default '0',
  `STATUS` int(10) unsigned NOT NULL default '0',
  `TYPE` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`ID`)
) ENGINE=MyISAM;

CREATE TABLE `votedplayer` (
  `VOTEID` int(10) unsigned NOT NULL default '0',
  `CHARID` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`VOTEID`,`CHARID`)
) ENGINE=MyISAM;

CREATE TABLE `voteitem` (
  `VOTEID` int(10) unsigned NOT NULL default '0',
  `OPTIONID` int(11) NOT NULL default '0',
  `OPTIONDESC` varchar(100) NOT NULL default '',
  `BALLOT` int(10) unsigned NOT NULL default '0',
  PRIMARY KEY  (`VOTEID`,`OPTIONID`)
) ENGINE=MyISAM;

CREATE TABLE `zoneinfo` (
  `zoneOpenTime` int(10) unsigned NOT NULL default '0'
) ENGINE=MyISAM;

CREATE TABLE `answer` (
  `id` bigint(20) unsigned NOT NULL auto_increment,
  `type` varchar(100) NOT NULL default '',
  `ask` blob NOT NULL,
  `answer_a` blob NOT NULL,
  `answer_b` blob NOT NULL,
  `answer_c` blob NOT NULL,
  `answer_d` blob NOT NULL,
  `answer_e` blob NOT NULL,
  `answer_f` blob NOT NULL,
  `quiz_type` int(11) NOT NULL default '0',
  `the_answer` bigint(20) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM;
