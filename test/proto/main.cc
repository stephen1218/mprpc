#include <iostream>
#include "test.pb.h"
#include <string>
using namespace fixbug;
int main() {
    GetFriendListResponse rsp;
    ResultCode *rc = rsp.mutable_result();//修改对象成员
    rc->set_errcode(0);

    User *user1 = rsp.add_friend_list();
    user1->set_name("zhang san");
    user1->set_age(20);
    user1->set_sex(User::MAN);

    User *user2 = rsp.add_friend_list();
    user2->set_name("lisi");
    user2->set_age(21);
    user2->set_sex(User::MAN);

    int n = rsp.friend_list_size();
    for(int i=0;i<n;++i) {
        std::cout<<rsp.friend_list(i).name()<<" "<< rsp.friend_list(i).age()<<" "<<rsp.friend_list(i).sex()<<std::endl;
    }
    return 0;
}
