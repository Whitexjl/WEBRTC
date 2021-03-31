#include <iostream>
#include <sys/types.h>  
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8111
#define MESSAGE_LEN 1024

int main(int argc, char* argv[])
{
    int ret = -1;
    int socket_fd;
    struct sockaddr_in serveraddr;
    char sendbuf[MESSAGE_LEN] = {0,};
    char recvbuf[MESSAGE_LEN] = {0,};

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0) {
        std::cout << "Failed to create socket!" << std::endl;
        exit(-1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = PORT;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    ret = connect(socket_fd, (struct sockaddr*)&serveraddr, sizeof(struct sockaddr));
    if(ret < 0) {
        std::cout << "Failed to connect socket!" << std::endl;
        exit(-1);
    }
    
    while(1) {
        memset(sendbuf, 0, MESSAGE_LEN); // 清零
        // gets(sendbuf);
        fgets(sendbuf, 20, stdin);
        ret = send(socket_fd, sendbuf, strlen(sendbuf), 0);
        if(ret <= 0) { // 说明连接断开
            std::cout << "Failed to send data!" << std::endl;
            break;
        }

        if(strcmp(sendbuf, "quit") == 0){
            break;
        }

        ret = recv(socket_fd, recvbuf, MESSAGE_LEN, 0);
        recvbuf[ret] = '\0';
        std::cout << "recv:" << recvbuf << std::endl;
    }
    
    close(socket_fd);
    return 0;
}

