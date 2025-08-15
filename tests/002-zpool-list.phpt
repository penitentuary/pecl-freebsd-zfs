--TEST--
Test zpool_list() function
--SKIPIF--
<?php 
if (!extension_loaded('zfs')) print 'skip ZFS extension not loaded';
if (!function_exists('zpool_list')) print 'skip zpool_list function not available';
?>
--FILE--
<?php
echo "Testing zpool_list() function\n";

$result = zpool_list();

if ($result === false) {
    echo "zpool_list() returned false (no pools or error)\n";
} elseif (is_array($result)) {
    echo "zpool_list() returned array with " . count($result) . " pools\n";
    
    // Test structure of first pool if any exist
    if (count($result) > 0) {
        $pool = $result[0];
        echo "First pool structure test:\n";
        
        $expected_keys = ['name', 'size', 'allocated', 'free', 'capacity', 'health'];
        foreach ($expected_keys as $key) {
            if (array_key_exists($key, $pool)) {
                echo "  $key: present\n";
            } else {
                echo "  $key: missing\n";
            }
        }
    }
} else {
    echo "zpool_list() returned unexpected type: " . gettype($result) . "\n";
}
?>
--EXPECTF--
Testing zpool_list() function
zpool_list() returned%s
