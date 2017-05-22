#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#define MAX_QUEUE 5
#define MAX_CLIENT 20

void error(const char *msg)
{
    perror(strcat("HIVE:", msg));
    exit(0);
}
void *serv(void*);

struct {
    pthread_mutex_t mutex;
    int honey;
} hive = { 
    PTHREAD_MUTEX_INITIALIZER
};

int main(int argc, char *argv[])
{
    int listen_sd, service_sd, port, clilen, change, code;
    pthread_t tid[MAX_CLIENT];
    struct sockaddr_in serv_addr, cli_addr;

    if(argc < 3)
        error("too few arg. use ./hive <honey> <port>");
    port = atoi(argv[2]);
    clilen = sizeof(cli_addr);
    listen_sd = socket(AF_INET, SOCK_STREAM, 0);
    hive.honey = atoi(argv[1]);
    if(listen_sd < 0)
        error("error creating listener");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    if(bind(listen_sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        error("error binding");

    listen(listen_sd, MAX_QUEUE);
    int i = 0;
    while(i < MAX_CLIENT)
    {
        printf("HIVE: listening\n");
        service_sd = accept(listen_sd, (struct sockaddr*)&cli_addr, &clilen);
        if(service_sd < 0)
            error("error creating socket");
        pthread_create(&tid[i], NULL, serv, (void*) &service_sd);
        i++;
    }
    pthread_join(*tid, (void*)&code);
    close(listen_sd);    
    return 0;
}

void *serv(void *arg)
{
    int sock = *(int*)arg;
    int change = 0;
    int stat = 0;
    while(hive.honey > 0)
    {
        recv(sock, &change, sizeof(int), 0);
        pthread_mutex_lock(&hive.mutex);
        hive.honey += change;
        stat = hive.honey;
        pthread_mutex_unlock(&hive.mutex);
        printf("change %d. now honey %d\n", change, stat);
    }
    printf("HIVE: bear is dead. kill'em all\n");
    close(sock);
    exit(0);
}