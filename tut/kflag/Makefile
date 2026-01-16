# Define the module name
MODULE_NAME := kflag
obj-m := $(MODULE_NAME).o

# Force the compiler name to match what the kernel expects
CC_NAME := x86_64-linux-gnu-gcc-13
KDIR    := /lib/modules/$(shell uname -r)/build
PWD     := $(shell pwd)

# Kernel build directory - standard path for most distros
KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) CC=$(CC_NAME) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) CC=$(CC_NAME) clean

# Compile, load with safety checks, and run a quick functional test
insmod: all
	@if lsmod | grep -q $(MODULE_NAME); then \
		echo "[!] Module $(MODULE_NAME) is already loaded. Reloading..."; \
		sudo rmmod $(MODULE_NAME); \
	fi
	@sudo insmod $(MODULE_NAME).ko
	@echo "[+] Module loaded. Testing /proc/flag..."
	@echo "init_test_log" | sudo tee /proc/flag > /dev/null
	@cat /proc/flag

# Safely remove the module
rmmod:
	@if lsmod | grep -q $(MODULE_NAME); then \
		sudo rmmod $(MODULE_NAME); \
		echo "[-] Module $(MODULE_NAME) removed."; \
	else \
		echo "[!] Module $(MODULE_NAME) is not currently loaded."; \
	fi

# Help menu for students
help:
	@echo "cs6265-tut Kernel Module Build System"
	@echo "--------------------------------------"
	@echo "Available commands:"
	@echo "  make         - Compile the kflag kernel module"
	@echo "  make insmod  - Compile, load, and verify the module"
	@echo "  make rmmod   - Unload the module from the kernel"
	@echo "  make clean   - Remove all build artifacts (.ko, .o, etc.)"
	@echo "  make help    - Display this menu"

.PHONY: all clean insmod rmmod help

