--TEST--
ZFS extension basic functionality
--SKIPIF--
<?php if (!extension_loaded('zfs')) print 'skip ZFS extension not loaded'; ?>
--FILE--
<?php
echo "ZFS extension is loaded\n";

// Test zpool_list function
if (function_exists('zpool_list')) {
    echo "zpool_list function exists\n";
    $pools = zpool_list();
    if (is_array($pools)) {
        echo "zpool_list returned array with " . count($pools) . " pools\n";
    } else {
        echo "zpool_list returned: " . var_export($pools, true) . "\n";
    }
} else {
    echo "zpool_list function not found\n";
}

// Test zfs_list function
if (function_exists('zfs_list')) {
    echo "zfs_list function exists\n";
    $datasets = zfs_list();
    if (is_array($datasets)) {
        echo "zfs_list returned array with " . count($datasets) . " datasets\n";
    } else {
        echo "zfs_list returned: " . var_export($datasets, true) . "\n";
    }
} else {
    echo "zfs_list function not found\n";
}
?>
--EXPECTF--
ZFS extension is loaded
zpool_list function exists
zpool_list returned%s
zfs_list function exists
zfs_list returned%s
