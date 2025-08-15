#!/bin/sh
# Build script for PHP ZFS Extension on FreeBSD

set -e

echo "Building PHP ZFS Extension for FreeBSD..."
echo "========================================="

# Check if we're on FreeBSD
if [ "$(uname)" != "FreeBSD" ]; then
    echo "Warning: This extension is designed for FreeBSD systems"
fi

# Check for required tools
echo "Checking for required tools..."

if ! command -v phpize >/dev/null 2>&1; then
    echo "Error: phpize not found. Please install PHP development package:"
    echo "  pkg install php81-dev"
    exit 1
fi

if ! command -v php-config >/dev/null 2>&1; then
    echo "Error: php-config not found. Please install PHP development package:"
    echo "  pkg install php81-dev"
    exit 1
fi

# Check for ZFS libraries
echo "Checking for ZFS libraries..."

if [ ! -f "/usr/local/include/libzfs.h" ] && [ ! -f "/usr/include/libzfs.h" ]; then
    echo "Error: libzfs.h not found. Please install OpenZFS:"
    echo "  pkg install openzfs"
    exit 1
fi

if [ ! -f "/usr/local/lib/libzfs.so" ] && [ ! -f "/usr/lib/libzfs.so" ]; then
    echo "Error: libzfs.so not found. Please install OpenZFS:"
    echo "  pkg install openzfs"
    exit 1
fi

echo "All prerequisites found!"

# Clean previous build
echo "Cleaning previous build..."
if [ -f "Makefile" ]; then
    make clean
fi
phpize --clean

# Generate configure script
echo "Running phpize..."
phpize

# Configure the extension
echo "Configuring extension..."
./configure --enable-zfs

# Build the extension
echo "Building extension..."
make

echo ""
echo "Build completed successfully!"
echo ""
echo "To install the extension, run:"
echo "  sudo make install"
echo ""
echo "Then add the following line to your php.ini:"
echo "  extension=zfs.so"
echo ""
echo "To test the extension, run:"
echo "  php test_zfs.php"
