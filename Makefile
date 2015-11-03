obj-m += hello.o
obj-m += hello-packet.o
#obj-m += rootkit.o
obj-m += rickroll.o
obj-m += excited_virus.o
obj-m += task2file.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
