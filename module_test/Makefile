SUBDIRS=dependency_module
define make_subdir
	@for subdir in $(SUBDIRS) ; do \
	(cd $$subdir && make $1) \
	done;
endef

ifneq ($(KERNELRELEASE),)
	obj-m := helloworld.o
	helloworld-objs := hello.o helper.o zc_dev.o
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
