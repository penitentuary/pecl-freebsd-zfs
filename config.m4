dnl config.m4 for extension zfs

PHP_ARG_WITH(zfs, for zfs support,
[  --with-zfs=[DIR]       Include zfs support])

if test "$PHP_ZFS" != "no"; then
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/libzfs.h"
  if test -r $PHP_ZFS/$SEARCH_FOR; then # path given as parameter
    ZFS_DIR=$PHP_ZFS
  else
    AC_MSG_CHECKING([for zfs files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        ZFS_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  
  if test -z "$ZFS_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([The required libzfs library was not found. You can obtain that package from OpenZFS project])
  fi

  PHP_ADD_INCLUDE($ZFS_DIR/include)
  
  dnl Add FreeBSD-specific ZFS include paths
  PHP_ADD_INCLUDE(/usr/src/cddl/compat/opensolaris/include)
  PHP_ADD_INCLUDE(/usr/src/sys/contrib/openzfs/include)
  PHP_ADD_INCLUDE(/usr/src/sys/contrib/openzfs/lib/libspl/include)
  PHP_ADD_INCLUDE(/usr/src/sys/contrib/openzfs/lib/libspl/include/os/freebsd)

  PHP_CHECK_LIBRARY(zfs,libzfs_init,
  [
    PHP_ADD_LIBRARY_WITH_PATH(zfs, $ZFS_DIR/lib, ZFS_SHARED_LIBADD)
    AC_DEFINE(HAVE_ZFSLIB,1,[Have libzfs])
  ],[
    AC_MSG_ERROR([libzfs not found])
  ],[
    -L$ZFS_DIR/lib -lm 
  ])
  
  PHP_CHECK_LIBRARY(zfs_core,lzc_get_props,
  [
    PHP_ADD_LIBRARY_WITH_PATH(zfs_core, $ZFS_DIR/lib, ZFS_SHARED_LIBADD)
    AC_DEFINE(HAVE_ZFS_CORE,1,[Have libzfs_core])
  ],[
    AC_MSG_WARN([libzfs_core not found, some features may not be available])
  ],[
    -L$ZFS_DIR/lib -lm
  ])

  PHP_CHECK_LIBRARY(nvpair,nvlist_alloc,
  [
    PHP_ADD_LIBRARY_WITH_PATH(nvpair, $ZFS_DIR/lib, ZFS_SHARED_LIBADD)
    AC_DEFINE(HAVE_NVPAIR,1,[Have libnvpair])
  ],[
    AC_MSG_WARN([libnvpair not found, some features may not be available])
  ],[
    -L$ZFS_DIR/lib -lm 
  ])

  PHP_SUBST(ZFS_SHARED_LIBADD)

  PHP_NEW_EXTENSION(zfs, zfs.c zfs_list.c zpool_list.c, $ext_shared)
fi
