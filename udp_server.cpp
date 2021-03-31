#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

#define PORT 8111
#define MESSAGE_LEN 1024

int main(int argc, char* argv[])
{
    int ret = -1;
    int on = 1;
    int socket_fd;
    struct sockaddr_in localaddr, remoteaddr;
    char in_buff[MESSAGE_LEN];

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_fd == -1) {
        std::cout << "Can not create socket" << std::endl;
        exit(-1);
    }

    ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if(ret == -1) {
       std::cout << "Can not set socket options" << std::endl;
    }

    localaddr.sin_family = AF_INET;
    localaddr.sin_port = PORT;
    localaddr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(localaddr.sin_zero), 8);

    ret = bind(socket_fd, (const struct sockaddr*)&localaddr, sizeof(struct sockaddr));
    if(ret == -1) {
        std::cout << "Can not bind addr!" << std::endl;
        exit(-1);
    }
    
    while(1) {
        socklen_t addr_len = sizeof(struct sockaddr);
        int len = recvfrom(socket_fd, (void *)in_buff, MESSAGE_LEN, 0, (struct sockaddr*) &remoteaddr, &addr_len);
        if(len < 0) {
            std::cout << "No data!" << std::endl;
            break;
        }

        std::cout << "Receive Message:" << in_buff << std::endl;
        sendto(socket_fd, (void *)in_buff, MESSAGE_LEN, 0, (const struct sockaddr*) &remoteaddr, addr_len);
    }

    close(socket_fd);

    return 0;
}
