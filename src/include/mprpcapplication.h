//singleton
#pragma once
class MprpcApplication
{
    public:
    static void init(int argc, char **argv);
    static MprpcApplication& getInstance();
private:
    MprpcApplication(){}
    MprpcApplication(const MprpcApplication&) = delete;
    MprpcApplication(MprpcApplication&&) = delete;
}