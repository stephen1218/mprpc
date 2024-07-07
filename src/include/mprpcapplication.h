//singleton
#pragma once
#include "mprpcconfig.h"
#include "mprpcchannel.h"
#include "mprpccontroller.h"
class MprpcApplication
{
    public:
    static void init(int argc, char **argv);
    static MpcpcConfig& GetConfig();
    static MprpcApplication& getInstance();
private:
    static MprpcConfig m_config;
    
    MprpcApplication(){}
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
}