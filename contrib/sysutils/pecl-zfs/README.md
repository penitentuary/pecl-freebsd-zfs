# FreeBSD Port for PECL ZFS Extension

This directory contains the FreeBSD port files for the PHP ZFS extension.

## Installation via Port

1. Copy this directory to your FreeBSD ports tree:
   ```bash
   sudo cp -r contrib/sysutils/pecl-zfs /usr/ports/sysutils/
   ```

2. Install the port:
   ```bash
   cd /usr/ports/sysutils/pecl-zfs
   sudo make install clean
   ```

## Manual Installation

If you prefer to build manually without using the port system:

1. Ensure dependencies are installed:
   ```bash
   pkg install php81-dev openzfs
   ```

2. Build the extension:
   ```bash
   cd /path/to/php-zfs-ext
   phpize
   ./configure --with-zfs
   make
   sudo make install
   ```

3. Enable the extension:
   Add `extension=zfs.so` to your php.ini

## Port Features

- Automatically handles FreeBSD-specific ZFS header paths
- Includes proper dependencies on OpenZFS
- Uses standard PECL build framework
- Integrates with FreeBSD package management

## Dependencies

- sysutils/openzfs - OpenZFS filesystem
- lang/php81 - PHP 8.1 or compatible version

## Notes

The port automatically configures the correct include paths for FreeBSD's ZFS headers located in the source tree, eliminating the need for manual header path configuration.
