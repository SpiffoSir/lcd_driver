KERN_DIR = /spiffo-git/spiffo-git/linux-6.7/
MODULE =?    

all:
	make -C $(KERN_DIR) M=$(PWD) ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- modules

clean:
	make -C $(KERN_DIR) M=$(PWD) clean
	
obj-m += $(MODULE).o
