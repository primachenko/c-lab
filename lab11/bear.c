#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(const char *msg)
{
    perror(strcat("BEAR:", msg));
    exit(0);
}

int main(int argc, char *argv[])
{
    if(argc < 5){
        printf("too few arg. use ./bear <address> <port> <honey> <sleep>");
        exit(0);
    }
    int port, cli_sock, change, sleep_time;
    char *servIP;
    struct sockaddr_in serv_addr;
    cli_sock = socket(AF_INET, SOCK_STREAM, 0);
    servIP = argv[1];
    port = atoi(argv[2]);
    change = 0 - atoi(argv[3]);
    sleep_time = atoi(argv[4]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(servIP);
    serv_addr.sin_port = htons(port);
    if(connect(cli_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        error("Ошибка подключения");
    while(1)    
    {
        send(cli_sock, &change, sizeof(int), 0);
        sleep(sleep_time);
    }
        close(cli_sock);
    return 0;
}

