<?php
/**
 * Test script for PHP ZFS Extension
 * This script tests the zfs_list() and zpool_list() functions
 */

echo "PHP ZFS Extension Test Script\n";
echo "=============================\n\n";

// Check if the ZFS extension is loaded
if (!extension_loaded('zfs')) {
    echo "ERROR: ZFS extension is not loaded!\n";
    echo "Make sure you have:\n";
    echo "1. Built and installed the extension\n";
    echo "2. Added 'extension=zfs.so' to your php.ini\n";
    echo "3. Restarted your web server (if applicable)\n";
    exit(1);
}

echo "ZFS extension is loaded successfully!\n\n";

// Test zpool_list() function
echo "Testing zpool_list():\n";
echo "--------------------\n";

try {
    $pools = zpool_list();
    
    if ($pools === false) {
        echo "ERROR: zpool_list() returned false\n";
    } elseif (empty($pools)) {
        echo "No ZFS pools found on this system\n";
    } else {
        echo "Found " . count($pools) . " ZFS pool(s):\n\n";
        
        foreach ($pools as $index => $pool) {
            echo "Pool #" . ($index + 1) . ":\n";
            echo "  Name: " . $pool['name'] . "\n";
            echo "  Size: " . formatBytes($pool['size']) . "\n";
            echo "  Allocated: " . formatBytes($pool['allocated']) . "\n";
            echo "  Free: " . formatBytes($pool['free']) . "\n";
            echo "  Capacity: " . $pool['capacity'] . "%\n";
            echo "  Health: " . $pool['health'] . "\n";
            if (isset($pool['version'])) {
                echo "  Version: " . $pool['version'] . "\n";
            }
            if (isset($pool['bootfs']) && $pool['bootfs'] !== '-') {
                echo "  Boot FS: " . $pool['bootfs'] . "\n";
            }
            echo "\n";
        }
    }
} catch (Exception $e) {
    echo "ERROR: Exception occurred while testing zpool_list(): " . $e->getMessage() . "\n";
}

echo "\n";

// Test zfs_list() function
echo "Testing zfs_list():\n";
echo "------------------\n";

try {
    $datasets = zfs_list();
    
    if ($datasets === false) {
        echo "ERROR: zfs_list() returned false\n";
    } elseif (empty($datasets)) {
        echo "No ZFS datasets found on this system\n";
    } else {
        echo "Found " . count($datasets) . " ZFS dataset(s):\n\n";
        
        foreach ($datasets as $index => $dataset) {
            echo "Dataset #" . ($index + 1) . ":\n";
            echo "  Name: " . $dataset['name'] . "\n";
            echo "  Type: " . $dataset['type'] . "\n";
            if (isset($dataset['used'])) {
                echo "  Used: " . $dataset['used'] . "\n";
            }
            if (isset($dataset['available'])) {
                echo "  Available: " . $dataset['available'] . "\n";
            }
            if (isset($dataset['referenced'])) {
                echo "  Referenced: " . $dataset['referenced'] . "\n";
            }
            if (isset($dataset['mountpoint']) && $dataset['mountpoint'] !== '-') {
                echo "  Mountpoint: " . $dataset['mountpoint'] . "\n";
            }
            if (isset($dataset['creation'])) {
                echo "  Created: " . $dataset['creation'] . "\n";
            }
            if (isset($dataset['compressratio'])) {
                echo "  Compression Ratio: " . $dataset['compressratio'] . "\n";
            }
            echo "\n";
        }
    }
} catch (Exception $e) {
    echo "ERROR: Exception occurred while testing zfs_list(): " . $e->getMessage() . "\n";
}

echo "Test completed!\n";

/**
 * Helper function to format bytes into human readable format
 */
function formatBytes($bytes, $precision = 2) {
    $units = array('B', 'KB', 'MB', 'GB', 'TB', 'PB');
    
    for ($i = 0; $bytes > 1024 && $i < count($units) - 1; $i++) {
        $bytes /= 1024;
    }
    
    return round($bytes, $precision) . ' ' . $units[$i];
}
?>
