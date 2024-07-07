#include <iostream>
#include <string>
#include "mprpcapplication.h"
#include "mprpcprovider.h"
#include "user.pb.h"

class UserService : fixbug::UserServiceRpc {
public:
    bool Login(std::string name, std::string pwd) {
        std::cout<<"doing local service::login"<<std::endl;
        std::cout<<"name: "<<name<<" pwd: "<<pwd<<std::endl;
    }

    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done) {
        //从LoginRequest中获取请求参数                
        std::string name = request->name();
        std::string pwd = request->pwd();

        //调用本地方法 
        bool login_result = Login(name,pwd);

        //把响应写入
        fixbug::ResultCode *Code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        //做回调操作，序列化和网络发送（由框架完成）
        done->Run();
    }
}

int main(int argc, char** argv)
{
    // 调用框架的初始化操作 provider -i config.conf
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象，把UserServcie对象发布到rpc节点上
    RpcProvider provider; 
    provider.NotifyService(new UserService());

    // 启动一个rpc服务发布节点，Run以后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();

    return 0;
}