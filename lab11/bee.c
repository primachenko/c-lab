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
#include <arpa/inet.h>
#include <time.h>

void *bee(void*);

void error(const char *msg)
{
    perror(strcat("HIVE:", msg));
    exit(0);
}

struct trans {
	int sock;
	int change;
} tr;

int main(int argc, char *argv[])
{
    if(argc < 4){
        printf("too few arg. use ./bee <address> <port> <honey> <number>");
        exit(0);
    }
    int port, change, number;
    char *servIP;
    struct sockaddr_in serv_addr;
    servIP = argv[1];
    port = atoi(argv[2]);
    change = atoi(argv[3]);
    number = atoi(argv[4]);
    int cli_sock[number];
    pthread_t tid[number];
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(servIP);
    serv_addr.sin_port = htons(port);
    tr.change = change;
    srand(time(NULL));
    for(int i = 0; i < number; i ++){
    	cli_sock[i] = socket(AF_INET, SOCK_STREAM, 0);
	    if(connect(cli_sock[i], (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
	        error("Ошибка подключения");
	    tr.sock = cli_sock[i];
	    pthread_create(&tid[i], NULL, bee, (void*) &tr);
    }
    int code;
    for(int i = 0; i < number; i ++){
    	pthread_join(tid[i], (void*)&code);
    	close(cli_sock[i]);
    }
    return 0;
}

void *bee(void *arg)
{
	struct trans tr = *(struct trans*)arg;
	int sock = tr.sock;
	int change = tr.change;
	while(1)    
    {
        send(sock, &change, sizeof(int), 0);
        sleep(rand()%3);
    }
        close(sock);
}