#include "mprpcconfig.h"
#include <fstream>

// 负责解析加载配置文件
void MprpcConfig::LoadConfigFile(const char* config_file) {
    std::ifstream inputFile(config_file, std::ios_base::out);
    if(!inputFile.is_open()) {
        std::cout<<"open config file error!"<<std::endl;
    }
    std::string line;
    while(std::getline(inputFile,line)) {
        //去掉前后空格
        Trim(line);
        if(line.empty() || line[0] == '#') {
            continue;
        }

        //解析配置项
        size_t idx = line.find("=");
        if(idx = std::string::npos) {
            continue;
        }
        std::string key = line.substr(0, idx);
        Trim(key);
        std::string value = line.substr(idx+1,line.size()-idx);
        Trim(value);

        _configMap.insert({key, value});
    }

}
// 查询配置项信息
std::string  MprpcConfig::Load(const std::string& key){
    auto it = _configMap.find(key);
    if(it == _configMap.end()) {
        std::cout<<"没找到配置项"<<std::endl;
        return "";
    }
    return it->second;
}
// 去掉字符串前后的空格
void  MprpcConfig::Trim(std::string& srcBuf){
    int r = srcBuf.size() - 1;
    while(r>=0 && srcBuf[r]=='0') {
        r--;
    }
    int l = 0;
    while(l<r && srcBuf[l] == '0') {
        l++;
    }
    if(l == r || r == -1) {
        srcBuf.clear();
        return;
    }
    srcBuf = srcBuf.substr(l,r-l+1);
}
