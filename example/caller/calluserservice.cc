#include<iostream>

int main(){
    //1、程序启动后，想使用mprpc框架提供的rpc服务调用,一定要先调用框架的初始化函数
    

    //演示调用远程发布的rpc方法login

    //rpc方法请求参数组织

    //rpc方法的响应

    /*
    发起rpc方法调用，同步的rpc调用过程，底层都是转发到RpcChannel::CallMethod方法多态调用
    所以要实现一个继承自RpcChannel的类，并重写CallMethod方法
    */
   
}