#include "logger.h"
#include <iostream>
// ����ʽ����
Logger& Logger::getInstance() {
    static Logger logger;
    return logger;
}
Logger::Logger() {
    // ����ר��д��־���߳�
    std::thread writeLogTask([&](){
        for(;;) {
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now); //��ȡ��ǰʱ��

            // ��֯�ļ���
            char fileName[128] = {0};
            sprintf(fileName, "%d-%d-%d-log.txt", nowtm->tm_year + 1900, nowtm->tm_mon+1, nowtm->tm_mday);

            FILE* pf = fopen(fileName,"a+");
            if(pf == nullptr) {
                std::cout<<"log file:"<<fileName<<" open error!"<<std::endl;
                exit(EXIT_FAILURE);
            }
            std::string msg = m_lckQue.pop();
            char timeBuf[128] = {0};
            sprintf(timeBuf, "%d:%d:%d=>[%s]",
                    nowtm->tm_hour,
                    nowtm->tm_min,
                    nowtm->tm_sec,
                    (m_logLevel == INFO ? "INFO" : "ERROR"));
            msg.insert(0, timeBuf);
            msg.append("\n");

            fputs(msg.c_str(),pf);
            fclose(pf); //дһ����־�͹ر��ļ�
        }
    });
    writeLogTask.detach();
}
void Logger::setLogLevel(LogLevel level) {
    m_logLevel = level;
}

void Logger::Log(std::string msg) {
    m_lckQue.push(msg);
}