ifneq ($(KERNELRELEASE), )
	obj-m := mydev.o
else
	#KERNEL_DIR ?= /usr/src/linux-source-4.4.0/linux-source-4.4.0
	KERNEL_DIR ?= /lib/modules/4.4.0-21-generic/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) KBUILD_EXTRA_SYMBOLS=/lib/modules/4.4.0-21-generic/build/Module.symvers modules
endif
