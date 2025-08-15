#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php_zfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

/* Global libzfs handle */
static libzfs_handle_t *g_zfs = NULL;

/* Function argument info */
ZEND_BEGIN_ARG_INFO_EX(arginfo_zfs_list, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_zpool_list, 0, 0, 0)
ZEND_END_ARG_INFO()

/* Function entries */
const zend_function_entry zfs_functions[] = {
    PHP_FE(zfs_list, arginfo_zfs_list)
    PHP_FE(zpool_list, arginfo_zpool_list)
    PHP_FE_END
};

/* Module entry */
zend_module_entry zfs_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_ZFS_EXTNAME,
    zfs_functions,
    PHP_MINIT(zfs),
    PHP_MSHUTDOWN(zfs),
    NULL,
    NULL,
    PHP_MINFO(zfs),
    PHP_ZFS_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_ZFS
ZEND_GET_MODULE(zfs)
#endif

/* Module initialization */
PHP_MINIT_FUNCTION(zfs)
{
    /* Initialize libzfs */
    g_zfs = libzfs_init();
    if (g_zfs == NULL) {
        php_error_docref(NULL, E_WARNING, "Failed to initialize libzfs");
        return FAILURE;
    }
    
    return SUCCESS;
}

/* Module shutdown */
PHP_MSHUTDOWN_FUNCTION(zfs)
{
    if (g_zfs != NULL) {
        libzfs_fini(g_zfs);
        g_zfs = NULL;
    }
    
    return SUCCESS;
}

/* Module info */
PHP_MINFO_FUNCTION(zfs)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "ZFS Support", "enabled");
    php_info_print_table_row(2, "Extension Version", PHP_ZFS_VERSION);
    php_info_print_table_row(2, "libzfs Support", "enabled");
    php_info_print_table_end();
}

/* Get libzfs handle */
libzfs_handle_t *get_libzfs_handle(void)
{
    return g_zfs;
}

/* Release libzfs handle */
void release_libzfs_handle(libzfs_handle_t *handle)
{
    /* Nothing to do here as we use a global handle */
}

/* Error handling function */
void php_zfs_error(const char *fmt, ...)
{
    va_list args;
    char buffer[1024];
    
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    
    php_error_docref(NULL, E_WARNING, "%s", buffer);
}

/* ZFS list function */
PHP_FUNCTION(zfs_list)
{
    libzfs_handle_t *zfs_handle;
    
    if (zend_parse_parameters_none() == FAILURE) {
        RETURN_FALSE;
    }
    
    zfs_handle = get_libzfs_handle();
    if (zfs_handle == NULL) {
        php_zfs_error("libzfs not initialized");
        RETURN_FALSE;
    }
    
    array_init(return_value);
    
    if (zfs_list_datasets(zfs_handle, return_value) != 0) {
        zval_dtor(return_value);
        RETURN_FALSE;
    }
}

/* ZPool list function */
PHP_FUNCTION(zpool_list)
{
    libzfs_handle_t *zfs_handle;
    
    if (zend_parse_parameters_none() == FAILURE) {
        RETURN_FALSE;
    }
    
    zfs_handle = get_libzfs_handle();
    if (zfs_handle == NULL) {
        php_zfs_error("libzfs not initialized");
        RETURN_FALSE;
    }
    
    array_init(return_value);
    
    if (zpool_list_pools(zfs_handle, return_value) != 0) {
        zval_dtor(return_value);
        RETURN_FALSE;
    }
}
