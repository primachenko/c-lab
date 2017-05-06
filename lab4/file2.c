#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Удалить из текста заданный символ (argv[3])
void exclChar(FILE*, FILE*, char);

int main(int argc, char *argv[])
{
	if(strlen(argv[3])!=1){
		printf("Недопустимый формат аргумента\n");
		exit(3);
	}	
	FILE *fpr, *fpw;
	if(argc < 4){
		printf("Недостаточно аргументов. Используйте:./file2 <textfile> <destination> <del char>\n");
		exit(1);	
	}else if((fpr=fopen(argv[1], "r"))==NULL || (fpw=fopen(argv[2], "w"))==NULL){
		printf("Не удалось открыть файл.\n");
		exit(2);
	}
	exclChar(fpr, fpw, *argv[3]);
	fclose(fpr);
	fclose(fpw);
	return 0;
}

void exclChar(FILE *fpr, FILE *fpw, char deletedChar){
	char ch;
	int count=1;
	while((ch=fgetc(fpr)) != EOF){
		if(ch != deletedChar){
			fputc(ch, fpw);
		}else{count++;}
	}
	printf("Обработка завершена.\nИсключено символов '%c': %i\n", deletedChar, count);
}