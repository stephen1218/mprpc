#include "mprpcprovider.h"
#include "mprpcapplication.h"
#include <string>
#include <functional>

void RpcProvider::NotiyService(google::protobuf::Service *service) {
    ServiceInfo service_info;
    //获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();
    // 获取服务的名字
    std::string service_name = pserviceDesc->name();
    int methodCnt = pserviceDesc->method_count();
    std::cout <<"service_name:"<<service_name << std::endl;
    for(int i = 0; i<methodCnt; ++i) {
        const google::protobuf::MethodDescriptor *pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        //将方法映射关系添加到对应服务对象的方法表中
        service_info.m_methodMap.insert({method_name,pmethodDesc});
        std::cout<<"method_name:"<<method_name<<std::endl;
    }
    service_info.m_service = service;
    m_serviceMap.insert({service_name, service_info});
}

//启动rpc服务节点，提供rpc远程网络调用服务
void RpcProvider::Run(){
    std::string ip = MprpcApplication::getInstance().getConfig().load("rpcseverip");
    uint16_t port = atoi(MprpcApplication::getInstance().getConfig().load("rpcserverport").c_str());
    muduo::net::InetAddress address(ip,port);
    //创建TCPServer对象

    muduo::net::TcpServer server(&m_eventloop, address, "RpcProvider");
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection,this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::onMessage,this,std::placeholders::_1,std::placeholders::_2, std::placeholders::_3));

    server.setThreadNum(4);
    server.start();

    std::cout<<"RpcProvider start service at ip:" << ip << " port:"<< std::endl;
    m_eventloop.loop();
}

void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr &conn) {
}
// 客户端发送的一个请求消息：  header_size（4字节）|（header）service_name  method_name  args_size | arg_str
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr &conn,
                            muduo::net::Buffer *buffer,
                            muduo::Timestamp Timestamp) {
    //1、从网络上接收远程rpc服务调用的字符流
    std::string recv_buf = buffer->retrieveAllAsString();
    //2、从字符流recv_buf读取前4个字节内容：header_size
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size,4,0);//小技巧
    //3、根据header_size读取数据头的原始字符流，反序列化数据，得到rpc请求的详细信息
    std::string rpc_header_str = recv_buf.substr(4,header_size);
    mprpc::RpcHeader rpcHeader;
    std::string service_name;
    std::string method_name;
    uint32_t args_size;

    if(rpcHeader.ParseFromString(rpc_header_str)) {
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    }
    else {
        std::cout<<"rpc_header_str "<<rpc_header_str<<" parse error"<<std::endl;
        return;
    }
    std::string args_str = recv_buf.substr(4+header_size, args_size);

    // 打印调试信息
    std::cout<<"===================================="<<std::endl;
    std::cout<<"header_size: "<< header_size<< std::endl;
    std::cout<<"rpc_header_str: "<< rpc_header_str<< std::endl;
    std::cout<<"service_name: "<< service_name<< std::endl;
    std::cout<<"method_name: "<< method_name<< std::endl;
    std::cout<<"args_str: "<< arges_str<< std::endl;
    std::cout<<"===================================="<<std::endl;
    //4、获取service对象和method对象
    auto it = m_serviceMap.find(service_name);
    if(it == m_serviceMap.end()) {
        std::cout << service_name << "is not exist!"<<std::endl;
        return;
    }
    auto mit = it->second.m_methodMap.find(method_name);
    if(mit = it->second.m_methodMap.end()) {
        std::cout << method_name << "is not exist!"<<std::endl;
        return;
    }
    google::protobuf::Service *service = it->second.m_service;
    const google::protobuf::MethodDescriptor *method = mit->second;

    //5、生成rpc方法调用的请求request和响应response参数
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if(!request->ParseFromString(args_str)) {
        std::cout<<"request parse error, content:"<<args_str<<std::endl;
    }
    google::protobuf::Message *response = service->GetResponsePrototype(method).New();
    //绑定一个方法调用后的回调
    google::protobuf::Closure *done = 
    google::protobuf::NewCallback<RpcProvider, const muduo::net::TcpConnectionPtr&, google::protobuf::Message*>(this,
                                                                                                                &RpcProvider::SendRpcResponse,
                                                                                                                conn,
                                                                                                                response);
    //done是执行完本地节点提供的方法后的回调，一般是把结果序列化发送回对端
    //6、在框架上根据远端rpc请求，调用当前rpc节点上发布的方法
    service->CallMethod(method,nullptr,request,response,done);
}