#pragma once

#include "google/protofuf/service.h"
#include <memory>
#include <unordered_map>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpConnection.h>
class RpcProvider
{
public:
    void NotiyService(google::protobuf::Service *service);

    //启动rpc服务节点，开始提供rpc远程网络调用服务
    void Run();
private:
    //组合eventloop
    muduo::net::EventLoop m_eventloop;
    //新socket链接回调
    void onConnect(const muduo::net::TcpConnectionPtr &conn);
    //已建立链接用户的读写事件回调
    void onMessage(const muduo::net::TcpConnectionPtr&,muduo::net::Buffer*,muduo::Timestamp);

    struct ServiceInfo {
        google::protobuf::Service *m_service; //保存服务对象的地址
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap;//保存服务方法映射<方法名，方法描述指针>
    };
    // 存储注册成功的 服务对象 和 其服务的所有信息
    std::unordered_map<std::string, ServiceInfo> m_serviceMap;
}