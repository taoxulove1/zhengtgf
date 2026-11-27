/**
 * \file
 * \version  $Id: zLogger.cpp  $
 * \author  
 * \date 
 * \brief Zebra项目日志系统定义文件
 *
 */

#include "zLogger.h"
#include <stdarg.h>
#include <stdio.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/dailyrollingfileappender.h>
#include <log4cxx/helpers/pool.h>
#include <log4cxx/helpers/transcoder.h>
#include <log4cxx/net/syslogappender.h>
#include <log4cxx/patternlayout.h>

using namespace log4cxx;
using log4cxx::helpers::Pool;
using log4cxx::helpers::Transcoder;
using log4cxx::net::SyslogAppender;
using log4cxx::net::SyslogAppenderPtr;

namespace {

const LogString logPattern = LOG4CXX_STR("%d{yyMMdd-HH:mm:ss }%c %5p: %m%n");

LogString toLogString(const std::string &value)
{
	LogString result;
	Transcoder::decode(value, result);
	return result;
}

}

const LevelPtr zLogger::zLevel::LEVELALARM(new Level(ALARM_INT,LOG4CXX_STR("ALARM"),3));
const LevelPtr zLogger::zLevel::LEVELIFFY(new Level(IFFY_INT,LOG4CXX_STR("IFFY"),3));
const LevelPtr zLogger::zLevel::LEVELTRACE(new Level(TRACE_INT,LOG4CXX_STR("TRACE"),3));
const LevelPtr zLogger::zLevel::LEVELGBUG(new Level(GBUG_INT,LOG4CXX_STR("GBUG"),3));

const zLogger::zLevel *  zLogger::zLevel::OFF=new zLevel(Level::getOff());
const zLogger::zLevel *  zLogger::zLevel::FATAL=new zLevel(Level::getFatal());
const zLogger::zLevel *  zLogger::zLevel::ALARM=new zLevel(LEVELALARM);
const zLogger::zLevel *  zLogger::zLevel::ERROR=new zLevel(Level::getError());
const zLogger::zLevel *  zLogger::zLevel::IFFY=new zLevel(LEVELIFFY);
const zLogger::zLevel *  zLogger::zLevel::WARN=new zLevel(Level::getWarn());
const zLogger::zLevel *  zLogger::zLevel::TRACE=new zLevel(LEVELTRACE);
const zLogger::zLevel *  zLogger::zLevel::INFO=new zLevel(Level::getInfo());
const zLogger::zLevel *  zLogger::zLevel::GBUG=new zLevel(LEVELGBUG);
const zLogger::zLevel *  zLogger::zLevel::DEBUG=new zLevel(Level::getDebug());
const zLogger::zLevel *  zLogger::zLevel::ALL=new zLevel(Level::getAll());

/**
 * \brief zLevel构造函数
 * \param  level 等级数字，类内部定义
 */
zLogger::zLevel::zLevel(LevelPtr level):zlevel(level)
{
}


/**
 * \brief 构造一个zLogger 
 *
 * \param  name zLogger的名字，将会出现在输出的日志中的每一行
 */
zLogger::zLogger(const std::string &name)
	: loggerName(name), logger(Logger::getLogger(name))
{
	bzero(message, sizeof(message));
	logger->setLevel(Level::getDebug());
	addConsoleLog();
}

/**
 * \brief 析构函数
 */
zLogger::~zLogger()
{
}

/**
 * \brief 得到Logger的名字，它出现在每条日志信息中
 * \return	Logger名字
 */
const std::string & zLogger::getName()
{
	return loggerName;
}

/**
 * \brief 设置Logger的名字，它出现在每条日志信息中
 * \param 要被设置的名字
 */
void zLogger::setName(const std::string &name)
{
	if (name == loggerName)
		return;

	LoggerPtr renamed = Logger::getLogger(name);
	renamed->setLevel(logger->getLevel());
	const AppenderList appenders = logger->getAllAppenders();
	for (AppenderList::const_iterator it = appenders.begin(); it != appenders.end(); ++it)
		renamed->addAppender(*it);
	loggerName = name;
	logger = renamed;
}

/**
 * \brief 添加控制台输出Log
 * \return	成功返回true，否则返回false 
 */
bool zLogger::addConsoleLog()
{
	LayoutPtr layout(new PatternLayout(logPattern));
	ConsoleAppenderPtr appender(new ConsoleAppender(layout));
	appender->setName(LOG4CXX_STR("console"));
	logger->addAppender(appender);
	return true;
}

/**
 * \brief 移除控制台Log输出
 */
void zLogger::removeConsoleLog()
{
	AppenderPtr appender = logger->getAppender(LOG4CXX_STR("console"));
	if (appender)
	{
		logger->removeAppender(appender);
		appender->close();
	}
}

/**
 * \brief 加一个本地文件Log输出
 *
 * \param file 要输出的文件名，Logger会自动地添加时间后缀 
 * \return 成功返回true，否则返回false
 */
bool zLogger::addLocalFileLog(const std::string &file)
{
	LayoutPtr layout(new PatternLayout(logPattern));
	DailyRollingFileAppenderPtr appender(new DailyRollingFileAppender(
		layout, toLogString(file), LOG4CXX_STR("'.'yyMMdd-HH")));
	appender->setName(toLogString("localfile:" + file));
	logger->addAppender(appender);
	return true;
}

/**
 * \brief 移出一个本地文件Log输出
 * \param file 要移除的Log文件名 
 */
void zLogger::removeLocalFileLog(const std::string &file)
{
	AppenderPtr appender = logger->getAppender(toLogString("localfile:" + file));
	if (appender)
	{
		logger->removeAppender(appender);
		appender->close();
	}
}

/**
 * \brief 添加一个Syslog输出
 * \param host Syslog服务器地址,默认为本主机
 * \return 成功返回true，否则返回false
 */
bool zLogger::addSysLog(const std::string &host)
{
	const LogString address = toLogString(host);
	LayoutPtr layout(new PatternLayout(LOG4CXX_STR("%c %5p: %m%n")));
	SyslogAppenderPtr appender(new SyslogAppender(layout, address,
		SyslogAppender::getFacility(LOG4CXX_STR("user"))));
	appender->setName(address);
	Pool pool;
	appender->activateOptions(pool);
	logger->addAppender(appender);
	return true;
}

/**
 * \brief 移除一个Syslog输出
 * \param host 要移除的Syslog服务器地址,默认为本主机
 */
void zLogger::removeSysLog(const std::string &host)
{
	AppenderPtr appender = logger->getAppender(toLogString(host));
	if (appender)
	{
		logger->removeAppender(appender);
		appender->close();
	}
}

#define getMessage(msg,msglen,pat)	\
do	\
{	\
	va_list ap;	\
	bzero(msg, msglen);	\
	va_start(ap, pat);		\
	::vsnprintf(msg, msglen - 1, pat, ap);	\
	va_end(ap);	\
}while(false)

/**
 * \brief 设置写日志等级
 * \param  zLevelPtr 日志等级.参见 #zLogger::zLevel
 */
void zLogger::setLevel(const zLevel * zLevelPtr)
{
	logger->setLevel(zLevelPtr->zlevel);
}

/**
 * \brief 设置写日志等级
 * \param  level 日志等级
 */
void zLogger::setLevel(const std::string &level)
{
	if ("off" == level)
		setLevel(zLevel::OFF);
	else if ("fatal" == level)
		setLevel(zLevel::FATAL);
	else if ("alarm" == level)
		setLevel(zLevel::ALARM);
	else if ("error" == level)
		setLevel(zLevel::ERROR);
	else if ("iffy" == level)
		setLevel(zLevel::IFFY);
	else if ("warn" == level)
		setLevel(zLevel::WARN);
	else if ("trace" == level)
		setLevel(zLevel::TRACE);
	else if ("info" == level)
		setLevel(zLevel::INFO);
	else if ("gbug" == level)
		setLevel(zLevel::GBUG);
	else if ("debug" == level)
		setLevel(zLevel::DEBUG);
	else if ("all" == level)
		setLevel(zLevel::ALL);
}

/**
 * \brief 写日志
 * \param  zLevelPtr 日志等级参见 #zLogger::zLevel
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::log(const zLevel * zLevelPtr,const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevelPtr->zlevel,message);
	msgMut.unlock();
	return true;
}

/**
 * \brief 强制写日志,不受日志等级限制
 * \param  zLevelPtr 日志等级参见 #zLogger::zLevel
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::forceLog(const zLevel * zLevelPtr,const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->forcedLog(zLevelPtr->zlevel,message);
	msgMut.unlock();
	return true;
}

/**
 * \brief 写fatal程序日志
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::fatal(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->fatal(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief 写error程序日志
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::error(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->error(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief 写error程序日志
 * \param  buf 输出内容
 * \return 成功返回true，否则返回false
 */
bool zLogger::error_out(const char * buf)
{
	msgMut.lock();
	logger->error(buf);
	msgMut.unlock();
	return true;
}

/**
 * \brief 写warn程序日志
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::warn(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->warn(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief 写info程序日志
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::info(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->info(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief 写debug程序日志
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::debug(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->debug(message);
	msgMut.unlock();
	return true;
}

/**
 * \brief 写alarm游戏日志
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::alarm(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevel::ALARM->zlevel,message);
	msgMut.unlock();
	return true;
}

/**
 * \brief 写iffy游戏日志
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::iffy(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevel::IFFY->zlevel,message);
	msgMut.unlock();
	return true;
}

/**
 * \brief 写trace游戏日志
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::trace(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevel::TRACE->zlevel,message);
	msgMut.unlock();
	return true;
}

/**
 * \brief 写gbug游戏日志
 * \param  pattern 输出格式范例，与printf一样
 * \return 成功返回true，否则返回false
 */
bool zLogger::gbug(const char * pattern, ...)
{
	msgMut.lock();
	getMessage(message,MSGBUF_MAX,pattern);

	logger->log(zLevel::GBUG->zlevel,message);
	msgMut.unlock();
	return true;
}
