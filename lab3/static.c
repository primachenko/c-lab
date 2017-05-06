#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 20

struct user
{
	char lastName[MAX_LEN];
	int birthYear;
	int number;
	double money;
};

void write(struct user p[],int);
void read(struct user p[], int);

int main(){
	int num;
	printf("Введите число записей:\n");
	scanf("%d", &num);
	struct user base[num];
	printf("Ввод данных\n");
	// printf("Распечатка базы, отсортированной по дате рождения:\n");
	write(&base, num);
	read(&base, num);
}

void write(struct user base[], int n){
	struct user *p[n];
	for(int i = 0; i < n; i++){
		printf("\nПерсонаж #%i\nВведите фамилию:\n", i+1);
		scanf("%s", &base[i].lastName);
		printf("Введите год рождения:\n");
		scanf("%i", &base[i].birthYear);
		printf("Введите номер отдела:\n");
		scanf("%i", &base[i].number);
		printf("Введите оклад сотрудника:\n");
		scanf("%d", &base[i].money);
	}
}

void read(struct user base[], int n){
	for(int i = 0; i < n; i++){
		printf("\nПерсонаж #%i\nФамилия:\n", i+1);
		printf("%s\n", base[i].lastName);
		printf("Год рождения:\n");
		printf("%i\n", base[i].birthYear);
		printf("Номер отдела:\n");
		printf("%i\n", base[i].number);
		printf("Оклад сотрудника:\n");
		printf("%d\n", base[i].money);
	}
}