CURRENT = $(shell uname -r) 
KDIR = /lib/modules/4.10.0-20-generic/build 
PWD = $(shell pwd) 
DEST = /lib/modules/4.10.0-20-generic/misc 
TARGET = ioctl 
obj-m = ioctl.o 
default:
	make -C $(KDIR) M=$(PWD) modules 
clean:
	rm -f *.o .*.cmd .*.flags *.mod.c *.order
	rm -f .*.*.cmd *.symvers *~ *.*~ TODO.*
	rm -fR .tmp*
	rm -rf .tmp_versions