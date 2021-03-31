#include <iostream>
#include <sys/socket.h>
#include <stdlib.h> 
#include <netinet/in.h> // 结构体头文件
#include <unistd.h> // close函数头文件
#include <string.h>

#define PORT 8111
#define MESSAGE_LEN 1024

int main(int argc, char* argv[])
{
    int ret = -1; // socket option返回值
    int on = 1; // socket起作用标志位
    int backlog = 10; // 毫秒级10个并发量
    int socket_fd, accept_fd; // 文件描述符
    struct sockaddr_in localaddr, remoteaddr; // 通过设置sockaddr_in结构体（用户角度）设置sockaddr结构体（计算机角度）

    char in_buff[MESSAGE_LEN]; // 消息

    socket_fd = socket(AF_INET, SOCK_STREAM, 0); // IPV4、TCP流传输
    if(socket_fd == -1) { // 创建失败
        std::cout << "Failed to create socket!" << std::endl;
        exit(-1);
    }

    // 套接字描述符、选项所在的协议层为套接字层、需要访问的选项、是否起作用、缓冲区长度
    ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)); 
    if(ret == -1) {
        std::cout << "Failed to set socket options!" << std::endl; // 即使没有设置成功，影响也不大，所以无需退出
    }

    // 结构体参数设置
    localaddr.sin_family = AF_INET; // IPV4
    localaddr.sin_port = PORT; // 端口
    localaddr.sin_addr.s_addr = INADDR_ANY; // 相当于0,对应本地任何一块网卡地址。泛指本机。后台会把ip字符串转换成32整数
    bzero(&(localaddr.sin_zero), 8); // 将结构体清零

    ret = bind(socket_fd, (struct sockaddr *)&localaddr, sizeof(struct sockaddr)); // 把sockaddr强制转成sockaddr_in*
    if(ret == -1) {
        std::cout << "Failed to bind addr!" << std::endl;
        exit(-1);
    }
        
    ret = listen(socket_fd, backlog);
    if(ret == -1) {
        std::cout << "Failed to listen socket!" << std::endl;
        exit(-1);
    }

    for(;;){
        socklen_t addr_len = sizeof(struct sockaddr);
        accept_fd = accept(socket_fd, (struct sockaddr*) &remoteaddr, &addr_len); // 一个新的socket描述符
        
        for(;;) { // 服务端接收到的来自客户端发送的消息
            ret = recv(accept_fd, (void *)in_buff, MESSAGE_LEN, 0);
            if(ret == 0) { // 说明对端没有发数据
                break;
            }

            std::cout << "Recv Message:" << in_buff << std::endl;
            send(accept_fd, (void *)in_buff, MESSAGE_LEN, 0); // 给客户端把消息返回
        }
        close(accept_fd);
    }

    close(socket_fd);

    return 0;
}


