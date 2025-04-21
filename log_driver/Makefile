obj-m := logmodule.o
logmodule-objs := main.o

ccflags-y += -I$(PWD)
# ccflags-y += -DENABLE_LOGMODULE_LOGGING
ccflags-y += -DENABLE_IOCTL_DELAY


PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
