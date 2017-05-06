#include "powerlib.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char const *argv[])
{
	if(argc<2){printf("too few arg\n"); exit(1);}
	printf("%s^3=%f\n%s^3=%f\n", argv[1], power3(atof(argv[1])),argv[1], power4(atof(argv[1])));
	return 0;
}