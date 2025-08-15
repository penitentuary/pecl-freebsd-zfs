#ifndef PHP_ZFS_H
#define PHP_ZFS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

/* FreeBSD ZFS includes */
#include <libzfs.h>
#include <libzfs_core.h>
#include <sys/nvpair.h>

#define PHP_ZFS_VERSION "1.0.0"
#define PHP_ZFS_EXTNAME "zfs"

extern zend_module_entry zfs_module_entry;
#define phpext_zfs_ptr &zfs_module_entry

/* Function declarations */
PHP_MINIT_FUNCTION(zfs);
PHP_MSHUTDOWN_FUNCTION(zfs);
PHP_MINFO_FUNCTION(zfs);

/* ZFS functions */
PHP_FUNCTION(zfs_list);
PHP_FUNCTION(zpool_list);

/* Internal helper functions */
libzfs_handle_t *get_libzfs_handle(void);
void release_libzfs_handle(libzfs_handle_t *g_zfs);

/* ZFS list implementation */
int zfs_list_datasets(libzfs_handle_t *g_zfs, zval *return_value);

/* ZPool list implementation */
int zpool_list_pools(libzfs_handle_t *g_zfs, zval *return_value);

/* Error handling */
void php_zfs_error(const char *fmt, ...);

#ifdef ZTS
#include "TSRM.h"
#endif

#endif /* PHP_ZFS_H */
