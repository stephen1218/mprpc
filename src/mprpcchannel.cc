#include "mprpcchannel.h"
#include <string>
#include "rpcheader.pb.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include "mprpcapplication.h"

/*客户端都是通过stub代理对象调用Rpc方法， 都转发到这里调用，作Rpc方法的数据序列化和网络发送*/
void MprpcChannel::CallMethod(const MethodDescriptor* method,
                          RpcController* controller, 
                          const Message* request,
                          Message* response, Closure* done) {
    const google::protobuf::ServiceDescriptor *sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    //1、获取方法参数的序列化字符串长度
    uint32_t args_size = 0;
    std::string args_str;
    if(request->SerializeToString(&args_str)) {
        args_size = args_str.size();
    } else {
        std::cout<< "serialize request error!"<< std::endl;
        return ;
    }
    //2、定义Rpc请求的header
    mprpc::RpcHeader rpcHeader;
    RpcHeader.set_service_name(service_name);
    RpcHeader.set_method_name(method_name);
    RpcHeader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string rpc_header_str;
    if(rpcHeader.SerializeToString(&rpc_header_str)) {
        header_size = rpc_header_str.size();
    } else {
        std::cout<<"rpcHeader sericalize error!"<<std::endl;
        return;
    }

    //3、组织待发送的Rpc请求字符串
    std::string send_rpc_str;
    send_rpc_str.insert(0,std::string((char*)&header_size),4);
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str;
    //4、使用TCP编程，完成对Rpc方法的远程调用
    int clientfd = socket(AF_INET,SOCK_STREAM, 0);
    if(-1 == clientfd) {
        char errtxt[512] = {0};
        sprintf(errtxt,"create socket error! errno:%d",errno);
        std::cout<<errtxt<<std::endl;
        return;
    }
    //读取配置文件rpcserver信息
    std::string ip = MprpcApplication::getInstance().getConfig().Load("rpcserverip");
    uint16_t port = atoi(MprpcApplication::getInstance().getConfig().Load("rpcserverport").c_str())

    struct sockaddr_in server_addr;
    sever_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    //连接rpc服务节点
    if(-1 == connect(clientfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
        close(clientfd);
        char errtxt[512] = {0};
        sprintf(errtxt,"connect error! errno:%d",error);
        std::cout<<errtxt<<std::endl;
        return;
    }
    //发送RPC请求
    if(-1 == send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(),0)) {
        close(clientfd);
        char errtxt[512] = {0};
        sprintf(errtxt,"send message error! errno:%d",error);
        std::cout<<errtxt<<std::endl;
        return;
    }

    //接受RPC请求的响应
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if(-1 = (recv_size = recv(clientfd,recv_buf,1024,0))) {
        close(clientfd);
        char errtxt[512] = {0};
        sprintf(errtxt,"recv message error! errno:%d",error);
        std::cout<<errtxt<<std::endl;
        return;
    }
    //5. 反序列化收到的Rpc调用的响应数据
    if(!response->ParseFromArray(recv_buf, recv_size)) {
        close(clientfd);
        char errtxt[512] = {0};
        sprintf(errtxt,"parse error! errno:%d",error);
        std::cout<<errtxt<<std::endl;
        return;
    }
    close(clientfd);
}