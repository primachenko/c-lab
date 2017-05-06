#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEN 20
#define NUM_REC 5

struct user
{
	char lastName[MAX_LEN];
	int birthYear;
	int number;
	double money;
};

void write(int);
void read(int);
int compare(const struct user **a, const struct user **b);

struct user *pst[NUM_REC];

int main(){
	write(NUM_REC);
	qsort(pst, NUM_REC, sizeof(struct user*), compare);
	read(NUM_REC);
}

void write(int length){
	printf("Ввод данных\n");
	for(int i = 0; i < length; i++){
		pst[i]=(struct user*)malloc(sizeof(struct user));
		printf("\nПерсонаж #%i\nВведите фамилию:\n", i+1);
		scanf("%s", &(pst[i]->lastName));
		printf("Введите год рождения:\n");
		scanf("%i", &(pst[i]->birthYear));
		printf("Введите номер отдела:\n");
		scanf("%i", &(pst[i]->number));
		printf("Введите оклад сотрудника:\n");
		scanf("%d", &(pst[i]->money));
		}
}

void read(int length){
	for(int i = 0; i < length; i++){
		printf("\nПерсонаж #%i\n", i+1	);
		printf("Фамилия:\n");
		printf("%s\n", pst[i]->lastName);
		printf("Год рождения:\n");
		printf("%i\n", pst[i]->birthYear);
		printf("Номер отдела:\n");
		printf("%i\n", pst[i]->number);
		printf("Оклад сотрудника:\n");
		printf("%d\n", pst[i]->money);
	}
}

int compare(const struct user **a, const struct user **b){
	return (*a)->birthYear - (*b)->birthYear;
}