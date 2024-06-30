    /*
    发起rpc方法调用，同步的rpc调用过程，底层都是转发到RpcChannel::CallMethod方法多态调用
    所以要实现一个继承自RpcChannel的类，并重写CallMethod方法
    */