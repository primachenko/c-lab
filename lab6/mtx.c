#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Умножение матрицы NxN на вектор Nx1 случайных чисел

int multiply(int*, int*, int);
void filling(int*, int*, int);
void prt(int*, int*, int);
void prtMtx(int*, int);
void clear(int*, int*);

int main(int argc, char const *argv[]){	
	printf("PARENT%d: start\n", getpid());
	int code, status;
	int N;
	printf("N = ");
	scanf("%i", &N);
	int *mtx;
	pid_t pid[N];
	for (int i = 0; i < N; i++){
		pid[i]=fork();
		if(pid[i]!=0){
			printf("PARENT%d: fork CHILD%d\n", getpid(), pid[i]);
		} else if (pid[i]==0){
			printf("  CHILD%d: start\n", getpid());
			int *A, *B;
			int result;
			A = (int*)malloc(N*sizeof(int));
	    	B = (int*)malloc(N*sizeof(int));
	    	filling(A, B, N);
	    	prt(A, B, N);
	    	result=multiply(A, B, N);
			printf("  CHILD%d: C = %i\n", getpid(), result);
			clear(A, B);
			exit(result);
		}
	}
// Код родителя
	mtx=malloc(N*sizeof(int));
	for (int i = 0; i < N; i++) {
        status = waitpid(pid[i], &code, 0);
        if (pid[i] == status) {
            printf("PARENT%d: CHILD%d done, result=%d\n", getpid(), pid[i], WEXITSTATUS(code));
        	mtx[i]=WEXITSTATUS(code);
        }
    }
    prtMtx(mtx, N);
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
void filling(int *A, int *B, int N){
	srand(getpid());
	for (int i = 0; i < N; i++){
	        A[i] = rand() % 5;
	        B[i] = rand() % 5;
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