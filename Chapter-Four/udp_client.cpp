#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>

#define MESSAGE_LEN 1024
#define PORT 8111

int main(int argc, char* argv[]) 
{
    int ret = -1;
    int socket_fd;
    char send_buff[MESSAGE_LEN] = {0, };
    char recv_buff[MESSAGE_LEN] = {0, };

    struct sockaddr_in serveraddr;

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_fd < 0) {
        std::cout << "Failed to create socket!" << std::endl;
        exit(-1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = PORT;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while(1) {
        memset(send_buff, 0, MESSAGE_LEN);
        fgets(send_buff, 20, stdin);
        
        socklen_t addr_len = sizeof(struct sockaddr);
        int len =  sendto(socket_fd, (const void*)&send_buff, MESSAGE_LEN, 0, (const struct sockaddr*)&serveraddr, addr_len);
        if(len < 0) {
            std::cout << "No data!" << std::endl;
            break;
        }

        len = recvfrom(socket_fd, (void *)&recv_buff, MESSAGE_LEN, 0, (struct sockaddr*)&serveraddr, &addr_len);
        recv_buff[len] = '\0';
        std::cout << "recv:" << recv_buff << std::endl; 
    }

    close(socket_fd);

    return 0;
}
