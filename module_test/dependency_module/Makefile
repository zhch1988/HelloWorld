ifneq ($(KERNELRELEASE),)
	obj-m := dependency.o
	dependency-objs := dependency_file.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
clean:
	rm -rf *.o .cmd *.ko *.mod.*
	rm -f modules.order Module.symvers
	rm -rf .[a-zA-Z0-9]*
