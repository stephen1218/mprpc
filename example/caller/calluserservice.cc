#include <iostream>
#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include "user.pb.h"

int main(){
    //1、程序启动后，想使用mprpc框架提供的rpc服务调用,一定要先调用框架的初始化函数
    MprpcApplication::init(argc,argv);

    //演示调用远程发布的rpc方法login
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    //rpc方法请求参数组织
    fixbug::LoginRequest request;
    request.set_name("li si");
    request.set_pwd("123456");
    //rpc方法的响应
    fixbug::LoginResponse response;
    /*
    发起rpc方法调用，同步的rpc调用过程，底层都是转发到RpcChannel::CallMethod方法多态调用
    所以要实现一个继承自RpcChannel的类，并重写CallMethod方法
    */
   stub.Login(nullptr, &request,&response,nullptr);

   if(0 == response.result().errorcode())
   {
        std::cout<<"rpc login success"<<response.success()<<std::endl;
   } else {
    std::cout<<" rpc login response error:"<<response.result().errcode()<<std::endl;
   }

   return 0;
}