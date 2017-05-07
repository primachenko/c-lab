#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
int main(int argc, char const *argv[])
{	
	if(argc<2){printf("too few arg\n"); exit(1);}
	void *handler;
	handler=dlopen("/home/oleg/lab/lab5/libpwr2.so", RTLD_LAZY);
	if(!handler){
		fprintf(stderr, "dlopen() error:%s\n", dlerror());
		exit(1);
	}
	double (*pf3)(double)= dlsym(handler, "power3");
	double (*pf4)(double)= dlsym(handler, "power4");
	printf("%s^3=%f\n%s^3=%f\n", argv[1], (*pf3)(atof(argv[1])),argv[1], (pf4)(atof(argv[1])));
	dlclose(handler);
	return 0;
}