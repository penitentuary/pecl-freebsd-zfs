#include "php_zfs.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <libzfs.h>
#include <libzfs_core.h>

/* Callback structure for dataset iteration */
typedef struct {
    zval *return_array;
    int error;
} zfs_list_cb_t;

/* Dataset properties we want to retrieve */
static const char *zfs_props[] = {
    "name",
    "used",
    "available", 
    "referenced",
    "mountpoint",
    "type",
    "creation",
    "compressratio",
    NULL
};

/* Callback function for iterating datasets - based on FreeBSD zfs list implementation */
static int zfs_list_callback(zfs_handle_t *zhp, void *data)
{
    zfs_list_cb_t *cb = (zfs_list_cb_t *)data;
    zval dataset_info;
    char property_buf[ZFS_MAXPROPLEN];
    zprop_source_t source;
    const char *dataset_name;
    zfs_type_t dataset_type;
    
    if (zhp == NULL) {
        return 0;
    }
    
    dataset_name = zfs_get_name(zhp);
    dataset_type = zfs_get_type(zhp);
    
    /* Initialize dataset info array */
    array_init(&dataset_info);
    
    /* Add dataset name */
    add_assoc_string(&dataset_info, "name", (char *)dataset_name);
    
    /* Add dataset type */
    switch (dataset_type) {
        case ZFS_TYPE_FILESYSTEM:
            add_assoc_string(&dataset_info, "type", "filesystem");
            break;
        case ZFS_TYPE_VOLUME:
            add_assoc_string(&dataset_info, "type", "volume");
            break;
        case ZFS_TYPE_SNAPSHOT:
            add_assoc_string(&dataset_info, "type", "snapshot");
            break;
        case ZFS_TYPE_BOOKMARK:
            add_assoc_string(&dataset_info, "type", "bookmark");
            break;
        default:
            add_assoc_string(&dataset_info, "type", "unknown");
            break;
    }
    
    /* Get used space */
    if (zfs_prop_get(zhp, ZFS_PROP_USED, property_buf, sizeof(property_buf), 
                     &source, NULL, 0, B_FALSE) == 0) {
        add_assoc_string(&dataset_info, "used", property_buf);
    }
    
    /* Get available space */
    if (zfs_prop_get(zhp, ZFS_PROP_AVAILABLE, property_buf, sizeof(property_buf),
                     &source, NULL, 0, B_FALSE) == 0) {
        add_assoc_string(&dataset_info, "available", property_buf);
    }
    
    /* Get referenced space */
    if (zfs_prop_get(zhp, ZFS_PROP_REFERENCED, property_buf, sizeof(property_buf),
                     &source, NULL, 0, B_FALSE) == 0) {
        add_assoc_string(&dataset_info, "referenced", property_buf);
    }
    
    /* Get mountpoint */
    if (zfs_prop_get(zhp, ZFS_PROP_MOUNTPOINT, property_buf, sizeof(property_buf),
                     &source, NULL, 0, B_FALSE) == 0) {
        add_assoc_string(&dataset_info, "mountpoint", property_buf);
    }
    
    /* Get creation time */
    if (zfs_prop_get(zhp, ZFS_PROP_CREATION, property_buf, sizeof(property_buf),
                     &source, NULL, 0, B_FALSE) == 0) {
        add_assoc_string(&dataset_info, "creation", property_buf);
    }
    
    /* Get compression ratio */
    if (zfs_prop_get(zhp, ZFS_PROP_COMPRESSRATIO, property_buf, sizeof(property_buf),
                     &source, NULL, 0, B_FALSE) == 0) {
        add_assoc_string(&dataset_info, "compressratio", property_buf);
    }
    
    /* Add this dataset to the return array */
    add_next_index_zval(cb->return_array, &dataset_info);
    
    zfs_close(zhp);
    return 0;
}

/* Main function to list ZFS datasets - based on FreeBSD zfs list implementation */
int zfs_list_datasets(libzfs_handle_t *g_zfs, zval *return_value)
{
    zfs_list_cb_t cb_data;
    int ret = 0;
    
    if (g_zfs == NULL) {
        php_zfs_error("libzfs handle is NULL");
        return -1;
    }
    
    cb_data.return_array = return_value;
    cb_data.error = 0;
    
    /* Iterate through all datasets
     * This is similar to how FreeBSD's zfs list command works:
     * zfs_for_each() iterates through all datasets and calls our callback
     */
    ret = zfs_for_each(g_zfs, 0, NULL, ZFS_TYPE_DATASET, NULL, NULL, 0,
                       zfs_list_callback, &cb_data);
    
    if (ret != 0) {
        php_zfs_error("Failed to iterate ZFS datasets: %s", libzfs_error_description(g_zfs));
        return -1;
    }
    
    if (cb_data.error != 0) {
        php_zfs_error("Error occurred during dataset enumeration");
        return -1;
    }
    
    return 0;
}
