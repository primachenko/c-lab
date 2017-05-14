#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <ctype.h>
// вычисление контрольной суммы
unsigned long long equal(char*);

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

int main(int argc, char const **argv)
{
	if(argc<2){
		printf("too few arg. Use ./shm <file> <file> <file> ...\n");
		return 0;
	}

	pid_t pid[argc-1];
	int status;

	int shmid;
	key_t key;
	unsigned long long  *shm;

	int semid;
	union semun arg;
	struct sembuf lock = {0, -1, 0};
	struct sembuf unlock = {0, 1, 0};

	semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
	arg.val = 1;
	semctl(semid, 0, SETVAL, arg);

	if((shmid = shmget(key, ((argc-1) * sizeof(unsigned long long)), IPC_CREAT | 0666)) < 0){
		perror("shmget");
		exit(3);
	}

	for(int i = 0; i < argc-1; i++){
		pid[i] = fork();
		if(pid[i] == 0)
		{
			if ((shm = (unsigned long long  *)shmat(shmid, NULL, 0)) == (unsigned long long  *)(-1)) {
				perror("shmat");
				exit(4);
			}

			unsigned long long var = equal((char*)argv[i+1]);

			if((semop(semid, &lock, 1)) == -1){
					fprintf(stderr, "Lock failed\n");
					exit(5);	
			}

			shm[i] = var;

			if((semop(semid, &unlock, 1)) == -1){
				fprintf(stderr, "Unlock failed\n");
				exit(6);
			}
			if(shmdt(shm) < 0) {
				printf("Detach error\n");
				exit(7);
			}
			exit(0);
		}
	}

	for (int i = 0; i < argc-1; i++) {
		waitpid(pid[i], &status, 0);
	}	

	if ((shm = (unsigned long long *)shmat(shmid, NULL, 0)) == (unsigned long long *)(-1)) {
		perror("shmat");
		exit(7);
	}
	printf("\tВычисленные контрольные суммы:\n");
	for (int i = 0; i < argc-1; i++)
		printf("%16s:\t%llo\n", argv[i+1], shm[i]);

	if (shmdt(shm) < 0) {
		printf("Detach error\n");
		exit(8);
	} 
	
	 if (shmctl(shmid, IPC_RMID, 0) < 0) {
		printf("Can't remove shm\n");
		exit(9);
	}
	
	if (semctl(semid, 0, IPC_RMID) < 0) {
		printf("Can't remove sem\n");
		exit(10);
	}

	return 0;
}

unsigned long long equal(char *fileName)
{
	FILE *fd;
	if((fd = fopen(fileName, "r")) == NULL){
		printf("Can't open the file. Exit..\n");
		exit(1);
	}
	unsigned long long count = 0;
	char buf;
	while((buf = getc(fd)) != EOF){
		count+=(unsigned long long)buf;
	}
	fclose(fd);
	return count;
}