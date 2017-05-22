#include <pthread.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <wait.h> 
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#define SLEEP_TIME 2
// Медоед и пчелы
void *bear(void*);
void *bee(void*);

struct {
	pthread_mutex_t	mutex;
	int	honey;
} hive = { 
	PTHREAD_MUTEX_INITIALIZER
};

int main(int argc, char const *argv[])
{
	if(argc < 3){
		printf("too few arg. use ./threads <honey> <bee> <bee2> <bear>\n"); //fix
		exit(1);
	}
	pthread_t bee_tid[atoi(argv[2])];
	pthread_t bear_tid;
	int pt, ptt;
	hive.honey = atoi(argv[1]);
	srand(time(NULL));
	pt = atoi(argv[3]);
	for(int i = 0; i< atoi(argv[2]); i++){
		if(pthread_create(&bee_tid[i], NULL, bee, (void*)&pt)){
			perror("Ошибка создания потока-пчелы:");
			exit(-1);
		}
	}
	ptt = atoi(argv[4]);
	if(pthread_create(&bear_tid, NULL, bear, (void*)&ptt)){
			perror("Ошибка создания потока-медоеда:");
			exit(-2);
	}
	pthread_join(bear_tid, NULL);
	printf("Медоед сдох мочим пчел\n");
	for(int i = 0; i < (int)argv[2]; i++)
		pthread_detach(bee_tid[i]);
	printf("Пчел убил\nДо свидания\n");

	return 0;
}

void *bear(void *arg)
{
	int *pt = (int*)arg;
	int eat = *pt;
	for(;;){
		// printf("Медоед спит\n");
		sleep(SLEEP_TIME);
		// printf("Медоед проснулся, сейчас будет жрать\n");
		pthread_mutex_lock(&hive.mutex);
		if(hive.honey < eat){
			pthread_mutex_unlock(&hive.mutex);
			printf("ГДЕ МЕД??\n");
			sleep(SLEEP_TIME);
			pthread_mutex_lock(&hive.mutex);
			if(hive.honey < eat){
				printf("Медоед скончался\n");
				pthread_mutex_unlock(&hive.mutex);
				pthread_exit(0);
			}	
		}
		hive.honey-=eat;
		printf("Медоед съел %d, осталось меда %d\n", eat, hive.honey);
		pthread_mutex_unlock(&hive.mutex);
	}
}

void *bee(void *arg)
{
	int *pt = (int*)arg;
	int get = *pt;
	for(;;){
		// printf("Пчела вылетела из улья\n");
		sleep(rand()%5);
		// printf("Пчела вернулась к улью\n");
		pthread_mutex_lock(&hive.mutex);
		hive.honey+=get;
		printf("Пчела принесла %d, осталось меда %d\n", get, hive.honey);
		pthread_mutex_unlock(&hive.mutex);
	}

}