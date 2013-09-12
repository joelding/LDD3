ifeq ($(KERNELRELEASE),)
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc -Wall -o load50 load50.c
else
	obj-m += ldelay.o
	ldelay-objs := main.o
endif

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean 
	rm load50
