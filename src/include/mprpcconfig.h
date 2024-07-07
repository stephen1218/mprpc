#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

// ��ܶ�ȡ�����ļ���
class MprpcConfig
{
public:
    // ����������������ļ�
    void LoadConfigFile(const char* config_file);
    // ��ѯ��������Ϣ
    std::string Load(const std::string& key);
    // ȥ���ַ���ǰ��Ŀո�
    void Trim(std::string& srcBuf);

private:
    std::unordered_map<std::string, std::string> _configMap;
};
