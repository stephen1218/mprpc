#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

// 锟斤拷芏锟饺★拷锟斤拷锟斤拷募锟斤拷锟�
class MprpcConfig
{
public:
    // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷募锟�
    void LoadConfigFile(const char* config_file);
    // 锟斤拷询锟斤拷锟斤拷锟斤拷锟斤拷息
    std::string Load(const std::string& key);
    // 去锟斤拷锟街凤拷锟斤拷前锟斤拷目崭锟�
    void Trim(std::string& srcBuf);

private:
    std::unordered_map<std::string, std::string> _configMap;
};
