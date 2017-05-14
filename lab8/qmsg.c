#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct msgbuf {
	long type;
	int msg;
};

int **allocMem(int, int);
void freeMem(int**, int, int);
void fillingMtx(int**, int, int);
void printMtx(int**, int, int);
void multiply(int**, int**, int**, int, int, int);
int **readFromFile(char*, int*, int*);
void sendResult(int, int, struct msgbuf*);
int rcvResult(int, struct msgbuf*, long);

int main(int argc, char **argv)
{
    FILE *fd;
    int **A;
    int **B;
    int **C;
    int l, m, k, n;
// Очередь
    int code;
    int mqid;
    struct msgbuf buf;
    mqid = msgget(IPC_PRIVATE, 0600|IPC_CREAT);

    if(argc == 3){
        printf("Read matrices from files %s %s\n", argv[1], argv[2]);
        if(((int)(A = readFromFile(argv[1], &l, &m))==-1)||((int)(B = readFromFile(argv[2], &k, &n))==-1)) {
            printf("Error reading from file.\n Exit..\n");
        }
        
    } else {
        printf("Enter the dimension of the matrices (MxN)\n");
        scanf("%dx%d", &l, &m);
        scanf("%dx%d", &k, &n);
        printf("Matrices filling with random numbers..\n");
        A = allocMem(l, m);
        B = allocMem(m, n);
        fillingMtx(A, l, m);
        fillingMtx(B, m, n);
    }

    int pid[l][n];

    printf("Printing matrices:\n A =\n");
    printMtx(A, l, m);
    printf("B =\n");
    printMtx(B, m, n);

    if(m != k){
        printf("Can't be multiplied. M != N\nExit..\n");
        freeMem(A, l, m);
        freeMem(B, k, n);
        exit(1);
    }

    C = allocMem(l, n);
    printf("Multiply matrices..\n");

    for(int i = 0; i < l; i++)
        for(int j = 0; j < n; j++){
            C[i][j] = 0;
            pid[i][j] = fork();
            if(pid[i][j] == 0){
            	for(int r = 0; r < m; r++)
        			C[i][j] += A[i][r] * B[r][j];
            	sendResult(C[i][j], mqid, &buf);
            	freeMem(A, l, m);
    			freeMem(B, m, n);
    			freeMem(C, l, n);
            	exit(0);
        	}
        }

    for(int i = 0; i < l; i++)
        for(int j = 0; j < n; j++){
        	if(pid[i][j] == waitpid(pid[i][j], &code, 0)){
        		C[i][j] = rcvResult(mqid, &buf, pid[i][j]);
        	}
        }

    printf("Printing result:\nC =\n");
    printMtx(C, l, n);
    
    printf("Removing queue..\n");
    msgctl(mqid, IPC_RMID, NULL);

    printf("Freeing memory..\n");
    freeMem(A, l, m);
    freeMem(B, m, n);
    freeMem(C, l, n);

    printf("Success\n");
    return 0;
}

int **allocMem(int lines, int columns)
{
    int **pMtx;
    pMtx = malloc(lines * sizeof(int*));
    for(int i = 0; i < lines; i++)
        pMtx[i] = malloc(columns * sizeof(int));
    // printf("Allocated %dkB\n", (lines * sizeof(int)) + (columns * sizeof(int*)));
    return pMtx;
}

void fillingMtx(int **pMtx, int lines, int columns)
{
    srand(time(NULL));
    for(int i = 0; i < lines; i++)
        for(int j = 0; j < columns; j++)
            pMtx[i][j] = rand() % 5;  
}

int **readFromFile(char *fileName, int *lines, int *columns)
{
    FILE *fp;
    int **pMtx;
    if((fp = fopen(fileName, "r")) == NULL)
        return (int**)-1;
    fscanf(fp, "%d%d", lines, columns);
    pMtx = allocMem(*lines, *columns);
    printf("Matrix %dx%d. Reading...\n", *lines, *columns);
    for(int i = 0; i < *lines; i++)
        for(int j = 0; j < *columns; j++)
            fscanf(fp, "%d", &pMtx[i][j]);
    fclose(fp);
    return (int**)pMtx;
}

void printMtx(int **pMtx, int lines, int columns)
{
    for(int i = 0; i < lines; i++){
        printf("|");
        for(int j = 0; j < columns; j++)    
            printf("%3d", pMtx[i][j]);
        printf("|\n");
    }
    printf("\n");
}

void multiply(int **A, int **B, int **C, int l, int m, int n)
{
    for(int i = 0; i < l; i++)
        for(int j = 0; j < n; j++){
            C[i][j] = 0;
            for(int r = 0; r < m; r++)
                C[i][j] += A[i][r] * B[r][j];
        }
}

int rcvResult(int msqid, struct msgbuf *buf, long type)
{
	msgrcv(msqid, buf, sizeof(struct msgbuf), type, IPC_NOWAIT);
	return buf->msg;
}

void sendResult(int result, int msqid, struct msgbuf *buf)
{
	buf->type = getpid();
	buf->msg = result;
	msgsnd(msqid, buf, sizeof(struct msgbuf), 0);
}

void freeMem(int **pMtx, int lines, int columns)
{
    for(int i = 0; i < lines; i++)
        free(pMtx[i]);
    free(pMtx);
    // printf("Freeing %dkB\n", (lines * sizeof(int)) + (columns * sizeof(int*)));
}