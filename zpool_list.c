#include "php_zfs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <libzfs.h>
#include <libzfs_core.h>

/* Callback structure for pool iteration */
typedef struct {
    zval *return_array;
    int error;
} zpool_list_cb_t;

/* Pool properties we want to retrieve */
static const char *zpool_props[] = {
    "name",
    "size",
    "allocated",
    "free",
    "capacity",
    "health",
    "version",
    "bootfs",
    NULL
};

/* Callback function for iterating pools - based on FreeBSD zpool list implementation */
static int zpool_list_callback(zpool_handle_t *zhp, void *data)
{
    zpool_list_cb_t *cb = (zpool_list_cb_t *)data;
    zval pool_info;
    char property_buf[ZPOOL_MAXPROPLEN];
    const char *pool_name;
    uint64_t size, allocated, free;
    int capacity;
    zpool_status_t status;
    char *msgid;
    
    if (zhp == NULL) {
        return 0;
    }
    
    pool_name = zpool_get_name(zhp);
    
    /* Initialize pool info array */
    array_init(&pool_info);
    
    /* Add pool name */
    add_assoc_string(&pool_info, "name", (char *)pool_name);
    
    /* Get pool size */
    size = zpool_get_prop_int(zhp, ZPOOL_PROP_SIZE, NULL);
    add_assoc_long(&pool_info, "size", (long)size);
    
    /* Get allocated space */
    allocated = zpool_get_prop_int(zhp, ZPOOL_PROP_ALLOCATED, NULL);
    add_assoc_long(&pool_info, "allocated", (long)allocated);
    
    /* Get free space */
    free = zpool_get_prop_int(zhp, ZPOOL_PROP_FREE, NULL);
    add_assoc_long(&pool_info, "free", (long)free);
    
    /* Calculate and add capacity percentage */
    if (size > 0) {
        capacity = (int)((allocated * 100) / size);
        add_assoc_long(&pool_info, "capacity", capacity);
    } else {
        add_assoc_long(&pool_info, "capacity", 0);
    }
    
    /* Get pool health status */
    status = zpool_get_status(zhp, &msgid);
    switch (status) {
        case ZPOOL_STATUS_CORRUPT_CACHE:
            add_assoc_string(&pool_info, "health", "CORRUPT_CACHE");
            break;
        case ZPOOL_STATUS_MISSING_DEV_R:
            add_assoc_string(&pool_info, "health", "MISSING_DEV_R");
            break;
        case ZPOOL_STATUS_MISSING_DEV_NR:
            add_assoc_string(&pool_info, "health", "MISSING_DEV_NR");
            break;
        case ZPOOL_STATUS_CORRUPT_LABEL_R:
            add_assoc_string(&pool_info, "health", "CORRUPT_LABEL_R");
            break;
        case ZPOOL_STATUS_CORRUPT_LABEL_NR:
            add_assoc_string(&pool_info, "health", "CORRUPT_LABEL_NR");
            break;
        case ZPOOL_STATUS_BAD_GUID_SUM:
            add_assoc_string(&pool_info, "health", "BAD_GUID_SUM");
            break;
        case ZPOOL_STATUS_CORRUPT_POOL:
            add_assoc_string(&pool_info, "health", "CORRUPT_POOL");
            break;
        case ZPOOL_STATUS_CORRUPT_DATA:
            add_assoc_string(&pool_info, "health", "CORRUPT_DATA");
            break;
        case ZPOOL_STATUS_FAILING_DEV:
            add_assoc_string(&pool_info, "health", "FAILING_DEV");
            break;
        case ZPOOL_STATUS_VERSION_NEWER:
            add_assoc_string(&pool_info, "health", "VERSION_NEWER");
            break;
        case ZPOOL_STATUS_HOSTID_MISMATCH:
            add_assoc_string(&pool_info, "health", "HOSTID_MISMATCH");
            break;
        case ZPOOL_STATUS_IO_FAILURE_WAIT:
            add_assoc_string(&pool_info, "health", "IO_FAILURE_WAIT");
            break;
        case ZPOOL_STATUS_IO_FAILURE_CONTINUE:
            add_assoc_string(&pool_info, "health", "IO_FAILURE_CONTINUE");
            break;
        case ZPOOL_STATUS_BAD_LOG:
            add_assoc_string(&pool_info, "health", "BAD_LOG");
            break;
        case ZPOOL_STATUS_ERRATA:
            add_assoc_string(&pool_info, "health", "ERRATA");
            break;
        case ZPOOL_STATUS_UNSUP_FEAT_READ:
            add_assoc_string(&pool_info, "health", "UNSUP_FEAT_READ");
            break;
        case ZPOOL_STATUS_UNSUP_FEAT_WRITE:
            add_assoc_string(&pool_info, "health", "UNSUP_FEAT_WRITE");
            break;
        case ZPOOL_STATUS_FAULTED_DEV_R:
            add_assoc_string(&pool_info, "health", "FAULTED_DEV_R");
            break;
        case ZPOOL_STATUS_FAULTED_DEV_NR:
            add_assoc_string(&pool_info, "health", "FAULTED_DEV_NR");
            break;
        case ZPOOL_STATUS_VERSION_OLDER:
            add_assoc_string(&pool_info, "health", "VERSION_OLDER");
            break;
        case ZPOOL_STATUS_FEAT_DISABLED:
            add_assoc_string(&pool_info, "health", "FEAT_DISABLED");
            break;
        case ZPOOL_STATUS_RESILVERING:
            add_assoc_string(&pool_info, "health", "RESILVERING");
            break;
        case ZPOOL_STATUS_OFFLINE_DEV:
            add_assoc_string(&pool_info, "health", "OFFLINE_DEV");
            break;
        case ZPOOL_STATUS_REMOVED_DEV:
            add_assoc_string(&pool_info, "health", "REMOVED_DEV");
            break;
        case ZPOOL_STATUS_OK:
        default:
            add_assoc_string(&pool_info, "health", "ONLINE");
            break;
    }
    
    /* Get pool version */
    if (zpool_get_prop(zhp, ZPOOL_PROP_VERSION, property_buf, 
                       sizeof(property_buf), NULL, B_FALSE) == 0) {
        add_assoc_string(&pool_info, "version", property_buf);
    }
    
    /* Get bootfs property */
    if (zpool_get_prop(zhp, ZPOOL_PROP_BOOTFS, property_buf,
                       sizeof(property_buf), NULL, B_FALSE) == 0) {
        add_assoc_string(&pool_info, "bootfs", property_buf);
    }
    
    /* Get altroot property */
    if (zpool_get_prop(zhp, ZPOOL_PROP_ALTROOT, property_buf,
                       sizeof(property_buf), NULL, B_FALSE) == 0) {
        add_assoc_string(&pool_info, "altroot", property_buf);
    }
    
    /* Add this pool to the return array */
    add_next_index_zval(cb->return_array, &pool_info);
    
    zpool_close(zhp);
    return 0;
}

/* Main function to list ZFS pools - based on FreeBSD zpool list implementation */
int zpool_list_pools(libzfs_handle_t *g_zfs, zval *return_value)
{
    zpool_list_cb_t cb_data;
    int ret = 0;
    
    if (g_zfs == NULL) {
        php_zfs_error("libzfs handle is NULL");
        return -1;
    }
    
    cb_data.return_array = return_value;
    cb_data.error = 0;
    
    /* Iterate through all pools
     * This is similar to how FreeBSD's zpool list command works:
     * zpool_iter() iterates through all pools and calls our callback
     */
    ret = zpool_iter(g_zfs, zpool_list_callback, &cb_data);
    
    if (ret != 0) {
        php_zfs_error("Failed to iterate ZFS pools: %s", libzfs_error_description(g_zfs));
        return -1;
    }
    
    if (cb_data.error != 0) {
        php_zfs_error("Error occurred during pool enumeration");
        return -1;
    }
    
    return 0;
}
