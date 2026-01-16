obj-m = kflag.o

KVERSION = $(shell uname -r)

all:
	make -C /lib/modules/$(KVERSION)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(shell pwd) clean

insmod:
	sudo insmod $(obj-m:.o=.ko)
	lsmod | grep $(obj-m:.o=)
	echo hi > /proc/flag
	cat /proc/flag

rmmod:
	sudo rmmod kflag

.PHONY: all clean insmod rmmod
