#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFF_LIM 2000
// Исключить строки с длиной, больше заданной (argv[3])
void readCutWrite(FILE *fpr, FILE *fpw, int lim){
	char buff[BUFF_LIM];
	while(fgets(buff, BUFF_LIM, fpr)!=NULL){
		if(strlen(buff)>lim){
			printf("Строка исключена: %s\n", buff);
		}else{
			fputs(&buff, fpw);
		}
	}
}

int main(int argc, char const *argv[])
{
	FILE *fpr, *fpw;
	if(argc < 4){
		printf("Недостаточно аргументов. Используйте:./file1 <textfile> <destination> <max length>\n");
		exit(1);
	}
	if((fpr=fopen(argv[1], "r"))==NULL || (fpw=fopen(argv[2], "w"))==NULL){
		printf("Не удалось открыть файл.\n");
		exit(1);
	}
	readCutWrite(fpr, fpw, atoi(argv[3]));
	fclose(fpr);
	fclose(fpw);
	return 0;
}