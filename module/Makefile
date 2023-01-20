obj-m += dmxio.o
dmxio-objs := ./src/dmxmodule.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
#	gcc -o uart src/uart.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
#	rm -f uart