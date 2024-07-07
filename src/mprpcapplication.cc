#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>
#include <string>

static void ShowArgsHelp()
{
    std::cout << "format: command -i <configfile>" << std::endl;
}

MprpcConfig MprpcApplication::_config;

void MprpcApplication::Init(int argc, char** argv)
{
    if(argc != 3)
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while((c = getopt(argc, argv, "i:")) != -1)
    {
        switch (c)
        {
        case 'i':
            config_file = optarg;
            break;
        case '?':
            ShowArgsHelp();
            break;
        case ':':
            ShowArgsHelp();
            break;
        default:
            break;
        }
    }

    // 开始加载配置文件
    _config.LoadConfigFile(config_file.c_str());

    // std::cout << "rpcserverip: " << _config.Load("rpcserverip") << std::endl;
    // std::cout << "rpcserverport: " << _config.Load("rpcserverport") << std::endl;
    // std::cout << "zookeeperip: " << _config.Load("zookeeperip") << std::endl;
    // std::cout << "zookeeperport: " << _config.Load("zookeeperport") << std::endl;
}

MprpcApplication& MprpcApplication::getInstance()
{
    static MprpcApplication app;
    return app;
}

MprpcConfig& MprpcApplication::getConfig()
{
    return _config;
}