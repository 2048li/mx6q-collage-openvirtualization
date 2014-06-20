# master make file for Trustzone sdk kit

#Set this variable to y if you want to enable experimental support for
#openssl. Note that if you set this to y, the variable CONFIG_CRYPTO in the
#file config.package, found in $(PACKAGE_DIR), *MUST* be set to y. If not,
#expect compilation failures. Note that a separate excutable otzone-crypto.elf
#would be generated, which can then be used for verification purposes.
#Further, a newlib enabled toolchain is required for compiling and using openssl.
#the environment variable CROSS_COMPILE_NEWLIB must be defined appropriately.
#For example in bash -- export CROSS_COMPILE_NEWLIB=arm-none-eabi-
export ENABLE_LIBCRYPT:= n

ifeq ($(ENABLE_LIBCRYPT), y)
ifndef CROSS_COMPILE_NEWLIB
$(error Experimental support for openssl is enabled. CROSS_COMPILE_NEWLIB \
		must be defined and point to a toolchain with \
		newlib support. For example, in bash do --\
		export CROSS_COMPILE_NEWLIB=arm-none-eabi-)
endif
export CRYPTO_BUILD:= n
endif

export SVISOR_BUILD:= n
export OTZONE_BUILD:= y
export CONFIG_MULTI_GUESTS_SUPPORT:=n
export CONFIG_ANDROID_GUEST:=n
export NON_SECURE_BOOTWRAPPER_SUPPORT:=y
export CONFIG_NEON_SUPPORT :=n

export SDK_PATH:= $(CURDIR)
export PACKAGE_DIR:= $(SDK_PATH)/../package
export PACKAGE_PATCHES_DIR:=$(PACKAGE_DIR)/patches
export PACKAGE_STORAGE_DIR:=$(PACKAGE_DIR)/storage
export PACKAGE_BUILD_DIR:=$(PACKAGE_DIR)/build
export CONFIG_PACKAGE_FILE:=$(PACKAGE_DIR)/config.package
export ANDROID_PATH:= /home/pub/ov_android/android

export KDIR := $(SDK_PATH)/../otz_linux/linux-otz
export KERNEL_BUILT_DIR := $(SDK_PATH)/../otz_linux/linux-otz

NS_IMAGE := $(SDK_PATH)/../otz_linux/boot/RTSM/normal.elf
ROOT_FILE_SYSTEM_IMAGE := $(SDK_PATH)/../otz_linux/armv5t_min_EB_V6_V7.image 

-include $(CONFIG_PACKAGE_FILE)

export BOARD:= imx6
export ARM_CPU:= CORTEX_A9
export ARM_ARCH:= ARMV7

ifeq ($(ARM_CPU), CORTEX_A15)
ifneq ($(CONFIG_ANDROID_GUEST), y)
export OTZONE_ASYNC_NOTIFY_SUPPORT:= y
endif
endif
export OTZONE_ASYNC_NOTIFY_SUPPORT:= y

.PHONY: all

all:

ifeq ($(NON_SECURE_BOOTWRAPPER_SUPPORT),y)
	$(warning =====create normal.elf here)
#	cp -f $(NS_IMAGE) $(SDK_PATH)/bin/normal.elf;
else
	cp -f $(NS_IMAGE) $(SDK_PATH)/bin/normal.bin;
endif

ifneq ($(CONFIG_ANDROID_GUEST),y)
	$(MAKE) -C $(SDK_PATH)/otz_api
	$(MAKE) -C $(SDK_PATH)/ns_client_apps
endif
	$(MAKE) -C $(SDK_PATH)/otz_driver
	$(MAKE) -C $(SDK_PATH)/../package

	cp -f bin/mmc_fs otzone/mmc_fs
	
ifneq ($(BOARD), zynq7)

endif


	$(MAKE) -C $(SDK_PATH)/otzone
	rm -rf otzone/mmc_fs

boot:
	$(MAKE) -C $(SDK_PATH)/otzone boot

clean:
	$(MAKE) -C $(SDK_PATH)/../package clean
	$(MAKE) -C $(SDK_PATH)/otzone clean
ifneq ($(CONFIG_ANDROID_GUEST),y)
	$(MAKE) -C $(SDK_PATH)/ns_client_apps clean
	$(MAKE) -C $(SDK_PATH)/otz_api clean
endif
	$(MAKE) -C $(SDK_PATH)/otz_driver clean

	rm -f otzone/mmc_fs
	rm -f bin/*.elf
	rm -f bin/*.bin
	rm -f bin/*.ko
