/*
 * NAME: CanaryLeak
 * GOAL: Pop a shell!
*/

// ubuntu 18.04, gcc 7.5.0
// gcc -o CanaryLeak CanaryLeak.c -no-pie -z norelro

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

void process(int sockfd)
{
    char buf[256];
    send(sockfd, "MSG: ", 5, 0);
    recv(sockfd, buf, 512, 0);
}

int main(void) {

    int serverfd, clientfd;
    int pid;
    struct sockaddr_in server, client;

    serverfd = socket(AF_INET, SOCK_STREAM, 0);

    const int enable = 1;
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(1234);

    bind(serverfd, (struct sockaddr*)&server, sizeof(server));
    listen(serverfd, 10);

    while(1)
    {
        clientfd = accept(serverfd, 0, 0);
        pid = fork();
        if(!pid) {
            process(clientfd);
            send(clientfd, "BYE", 4, 0);
        } else {
            close(clientfd);
        }
    }

    close(serverfd);
    return 0;
}