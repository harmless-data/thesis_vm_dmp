obj-m := logmodule.o
logmodule-objs := main.o

ccflags-y += -I$(PWD)

PWD := $(CURDIR)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
