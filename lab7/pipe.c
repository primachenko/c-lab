#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LEN 4

// Умножение матрицы NxN на вектор Nx1 случайных чисел

int multiply(int*, int*, int);
void filling(int**, int*, int);
void prt(int*, int*, int);
void prtMtx(int*, int);
void clear(int*, int*);

int main(int argc, char const *argv[]){	
	
	printf("PARENT%d: start\n", getpid());
	
	int code, status;
	int N;
	printf("N = ");
	scanf("%i", &N);

	int d[N][2];
	
	for(int i = 0; i<N; i++){
		if(pipe(d[i])==-1){
			perror("Ошибка при открытиии канала");
			exit(1);
		}
	}		
	int *mtx;
	pid_t pid[N];
	int **A;
	int *B;
	int result;
	A = (int**)malloc(N*sizeof(int*));
	// exit(0);
	for(int k = 0; k < N; k++){
		A[k]=(int*)malloc(N*sizeof(int));
	}
	B = (int*)malloc(N*sizeof(int));

	filling(A, B, N);
	for (int i = 0; i < N; i++){
		pid[i]=fork();
		if(pid[i]!=0){
			printf("PARENT%d: fork CHILD%d\n", getpid(), pid[i]);
		} else if (pid[i]==0){
			// Код дочернего процесса
			close(d[i][0]);
			int res;
			printf("  CHILD%d: start\n", getpid());
	    	prt(A[i], B, N);
	    	res=multiply(A[i], B, N);
			printf("  CHILD%d: C = %i\n", getpid(), res);
			write(d[i][1], &res, sizeof(int));
			clear(A[i], B);
			exit(0);
		}
	}
// Код родителя
	mtx=malloc(N*sizeof(int));
	for (int i = 0; i < N; i++) {
		close(d[i][1]);
		int res;
        if (pid[i]==waitpid(pid[i], &code, 0)) {
        	read(d[i][0], &res, sizeof(int));
        	mtx[i]=res;
            printf("PARENT%d: CHILD%d done, result=%d\n", getpid(), pid[i], mtx[i]);
    	}
    }
    prtMtx(mtx, N);
    clear(A, B);
	return 0;

}
// Умножение
int multiply(int *A, int *B, int N){
	int C = 0;
    for(int i=0; i<N; i++)
        C += A[i] * B[i];
	return C;
}
// Заполнение случайными числами
void filling(int **A, int *B, int N){
	srand(getpid());
	for (int i = 0; i < N; i++){
	        B[i] = rand() % 5;
	        for(int j = 0;j < N; j++)
	        	A[i][j] = rand() % 5;
	    }
}
// Печать исходных строк и вектора
void prt(int *A, int *B, int N){
	printf("  CHILD%d: A = |", getpid());
    	for(int i=0; i<N; i++)
    		printf(" %d ", A[i]);
    	printf("|\n  CHILD%d: B = |", getpid());
    	for(int i=0; i<N; i++)
    		printf(" %d ", B[i]);
    	printf("|\n");
}

void clear(int *A, int *B){
	free(A);
	free(B);
}

void prtMtx(int *C, int N){
	printf("PARENT%d: C = |", getpid());
    	for(int i=0; i<N; i++)
    		printf(" %d ", C[i]);
    	printf("|\nexit"); 
}