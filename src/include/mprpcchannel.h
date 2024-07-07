    /*
    MprpcChannel是mprpc框架提供给客户端进行Rpc调用
    发起rpc方法调用，同步的rpc调用过程，底层都是转发到RpcChannel::CallMethod方法多态调用
    所以要实现一个继承自RpcChannel的类，并重写CallMethod方法
    */
   #pragma once
   #include <google/protobuf/service.h>
   #include <google/protobuf/descriptor.h>
   #include <google/protobuf/message.h>

   class MprpcChannel : public google::protobuf::RpcChannel {
    public:
            // 客户端都是通过stub代理对象调用Rpc方法， 都转发到这里调用，作Rpc方法的数据序列化和网络发送
            void CallMethod(const MethodDescriptor* method,
                          RpcController* controller, const Message* request,
                          Message* response, Closure* done);
   }