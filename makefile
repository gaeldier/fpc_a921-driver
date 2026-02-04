obj-m += hello-1.o

all:
	make -C /lib/modules//build M= modules

clean:
	make -C /lib/modules//build M= clean
