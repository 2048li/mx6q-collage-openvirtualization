ifndef SDK_PATH
$(error SDK_PATH must be defined before this file is included)
endif

export $BOARD
export $ARM_CPU
export $ARM_ARCH
export $ENABLE_LIBCRYPT

.PHONY: all
all: 
	if [ ! -d src/package ]; then \
	mkdir -p src/package/src; \
	cp -R $(SDK_PATH)/../package/src src/package/; \
	fi

	$(warning =====otzone step1)
	$(MAKE) -f Makefile.in modules
	$(warning =====otzone step2)
	$(MAKE) -f Makefile.in clean
	$(warning =====otzone step3)
	$(MAKE) -f Makefile.in
	$(warning =====otzone step4)
	$(MAKE) -f Makefile.boot
ifeq ($(ENABLE_LIBCRYPT), y)	
	$(MAKE) -f Makefile.crypto modules
	$(MAKE) -f Makefile.crypto clean
	$(MAKE) -f Makefile.crypto
endif
#	$(MAKE) -f Makefile.libc
	$(warning =====otzone step5)
	rm -r -f src/package

boot:
	$(warning =====otzone step6)
	$(MAKE) -f Makefile.boot

.PHONY: clean
clean:
	$(MAKE) -f Makefile.in clean
	$(MAKE) -f Makefile.boot clean
ifeq ($(ENABLE_LIBCRYPT), y)	
	$(MAKE) -f Makefile.crypto clean
endif
#	$(MAKE) -f Makefile.libc clean
	rm -r -f src/package


