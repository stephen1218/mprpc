#pragma once

#include "lockqueue.h"
#include <string>

class Logger;
// �û�ʹ����־ģ��ĺ궨�巽��
#define LOG_INFO(fmt, ...)          \
    do                              \
    {                               \
        Logger &logger = Logger::getInstance(); \
        logger.setLogLevel(INFO);               \
        char c[1024];                           \
        sprintf(c, fmt, ##__VA_ARGS__);         \
        logger.log(c);                          \
    } while(0)                                  \

#define LOG_ERROR(fmt, ...)          \
    do                              \
    {                               \
        Logger &logger = Logger::getInstance(); \
        logger.setLogLevel(ERROR);               \
        char c[1024];                           \
        sprintf(c, fmt, ##__VA_ARGS__);         \
        logger.log(c);                          \
    } while(0)                                  \

//������־����
enum LogLevel 
{
    INFO,
    ERROR
};

class Logger {
public:
    static Logger &getInstance();
    void setLogLevel(LogLevel level);
    void Log(std::string msg);
private:
    int m_logLevel;
    LockQueue<std::string> m_lckQue;
    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger &&) = delete;
};