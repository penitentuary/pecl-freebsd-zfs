--TEST--
Test zfs_list() function
--SKIPIF--
<?php 
if (!extension_loaded('zfs')) print 'skip ZFS extension not loaded';
if (!function_exists('zfs_list')) print 'skip zfs_list function not available';
?>
--FILE--
<?php
echo "Testing zfs_list() function\n";

$result = zfs_list();

if ($result === false) {
    echo "zfs_list() returned false (no datasets or error)\n";
} elseif (is_array($result)) {
    echo "zfs_list() returned array with " . count($result) . " datasets\n";
    
    // Test structure of first dataset if any exist
    if (count($result) > 0) {
        $dataset = $result[0];
        echo "First dataset structure test:\n";
        
        $expected_keys = ['name', 'type', 'used', 'available', 'referenced', 'mountpoint'];
        foreach ($expected_keys as $key) {
            if (array_key_exists($key, $dataset)) {
                echo "  $key: present\n";
            } else {
                echo "  $key: missing\n";
            }
        }
        
        // Test dataset type values
        if (isset($dataset['type'])) {
            $valid_types = ['filesystem', 'volume', 'snapshot', 'bookmark'];
            if (in_array($dataset['type'], $valid_types)) {
                echo "  type value is valid: " . $dataset['type'] . "\n";
            } else {
                echo "  type value is invalid: " . $dataset['type'] . "\n";
            }
        }
    }
} else {
    echo "zfs_list() returned unexpected type: " . gettype($result) . "\n";
}
?>
--EXPECTF--
Testing zfs_list() function
zfs_list() returned%s
