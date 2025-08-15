# Makefile for PHP ZFS Extension on FreeBSD
# This makefile helps build the extension using phpize

EXTENSION_NAME = zfs
PHP_CONFIG = php-config
PHPIZE = phpize

# Default target
all: build

# Configure and build the extension
build: configure
	$(MAKE) -f Makefile.frag

# Run phpize to generate configure script
configure: config.m4
	$(PHPIZE)
	./configure --enable-$(EXTENSION_NAME)

# Clean build files
clean:
	if [ -f Makefile.frag ]; then $(MAKE) -f Makefile.frag clean; fi
	$(PHPIZE) --clean

# Install the extension
install: build
	$(MAKE) -f Makefile.frag install

# Test the extension
test: install
	php -m | grep $(EXTENSION_NAME)
	php test_zfs.php

# Development helpers
dev-clean: clean
	rm -f configure configure.in config.h.in
	rm -rf autom4te.cache/
	rm -f acinclude.m4 aclocal.m4 config.guess config.sub
	rm -f install-sh ltmain.sh missing mkinstalldirs

.PHONY: all build configure clean install test dev-clean
