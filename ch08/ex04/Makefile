ifeq ($(KERNELRELEASE),)
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
else
	obj-m += slab.o
	slab-objs := main.o
endif

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean 
