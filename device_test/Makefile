SUBDIRS=
define make_subdir
	@for subdir in $(SUBDIRS) ; do \
	(cd $$subdir && make $1) \
	done;
endef

ifneq ($(KERNELRELEASE),)
	obj-m := my_device.o
	my_device-objs := mydevice.o
	EXTRA_CFLAGS += -I$(PWD)/../include
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(call make_subdir,)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif
clean:
	$(call make_subdir,clean)
	rm -rf *.o .cmd *.ko *.mod.*
	rm -f modules.order Module.symvers
	rm -rf .[a-zA-Z0-9]*
