# PHP ZFS Extension for FreeBSD

A PHP extension that provides interface to OpenZFS on FreeBSD systems through the libzfs and libzfs_core libraries.

## Features

- **zfs_list()** - List all ZFS datasets with their properties
- **zpool_list()** - List all ZFS pools with their status and properties

## Requirements

- FreeBSD with OpenZFS installed
- PHP development headers (php-dev package)
- libzfs and libzfs_core development libraries
- autotools (autoconf, automake)

## Installation

### Prerequisites

On FreeBSD, ensure you have the required packages:

```bash
# Install PHP development headers
pkg install php81-dev  # or your PHP version

# Install OpenZFS (if not already installed)
pkg install openzfs

# Install build tools
pkg install autotools
```

### Building the Extension

1. Clone or extract the extension source code
2. Navigate to the extension directory
3. Build and install:

```bash
cd /path/to/php-zfs-ext
make clean
make build
sudo make install
```

### Alternative Manual Build

```bash
phpize
./configure --enable-zfs
make
sudo make install
```

### Enable the Extension

Add the following line to your `php.ini` file:

```ini
extension=zfs.so
```

Restart your web server if using PHP with Apache/Nginx.

## Usage

### Basic Examples

```php
<?php
// List all ZFS pools
$pools = zpool_list();
foreach ($pools as $pool) {
    echo "Pool: " . $pool['name'] . "\n";
    echo "Size: " . $pool['size'] . " bytes\n";
    echo "Health: " . $pool['health'] . "\n";
    echo "Capacity: " . $pool['capacity'] . "%\n\n";
}

// List all ZFS datasets
$datasets = zfs_list();
foreach ($datasets as $dataset) {
    echo "Dataset: " . $dataset['name'] . "\n";
    echo "Type: " . $dataset['type'] . "\n";
    echo "Used: " . $dataset['used'] . "\n";
    echo "Mountpoint: " . $dataset['mountpoint'] . "\n\n";
}
?>
```

### Function Reference

#### zpool_list()

Returns an array of all ZFS pools with the following properties:

- `name` - Pool name
- `size` - Total pool size in bytes
- `allocated` - Allocated space in bytes
- `free` - Free space in bytes
- `capacity` - Used capacity as percentage
- `health` - Pool health status
- `version` - Pool version
- `bootfs` - Boot filesystem (if set)
- `altroot` - Alternate root (if set)

#### zfs_list()

Returns an array of all ZFS datasets with the following properties:

- `name` - Dataset name
- `type` - Dataset type (filesystem, volume, snapshot, bookmark)
- `used` - Used space
- `available` - Available space
- `referenced` - Referenced space
- `mountpoint` - Mount point
- `creation` - Creation time
- `compressratio` - Compression ratio

## Testing

Run the included test script:

```bash
php test_zfs.php
```

This will test both functions and display information about your ZFS pools and datasets.

## Error Handling

Both functions return `false` on error. PHP warnings will be generated with error details.

```php
$pools = zpool_list();
if ($pools === false) {
    echo "Error: Could not list ZFS pools\n";
}
```

## Implementation Details

This extension is based on the FreeBSD ZFS command-line utilities implementation and uses:

- `libzfs` for ZFS dataset operations
- `libzfs_core` for core ZFS functionality
- `libnvpair` for name-value pair operations

The implementation follows the same patterns used by the FreeBSD `zfs` and `zpool` command-line tools.

## Troubleshooting

### Extension Not Loading

1. Check that `extension=zfs.so` is in your `php.ini`
2. Verify the extension was installed: `php -m | grep zfs`
3. Check PHP error logs for loading errors

### Permission Errors

ZFS operations typically require root privileges. Ensure your PHP process has appropriate permissions or run with sudo for testing.

### Library Not Found Errors

Ensure OpenZFS development libraries are installed:

```bash
pkg install openzfs
```

## Development

### File Structure

- `config.m4` - Autotools configuration
- `php_zfs.h` - Main header file
- `php_zfs.c` - Main extension file
- `zfs_list.c` - ZFS dataset listing implementation
- `zpool_list.c` - ZFS pool listing implementation
- `Makefile` - Build automation
- `test_zfs.php` - Test script

### Adding New Functions

1. Add function declaration to `php_zfs.h`
2. Add function entry to `zfs_functions[]` in `php_zfs.c`
3. Implement the function in appropriate `.c` file
4. Update `config.m4` if new source files are added

## License

This extension is provided as-is for educational and development purposes.

## Contributing

Contributions are welcome! Please ensure all code follows the existing style and includes appropriate error handling.
