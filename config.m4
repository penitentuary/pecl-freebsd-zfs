PHP_ARG_ENABLE(zfs, whether to enable ZFS support,
[  --enable-zfs           Enable ZFS support])

if test "$PHP_ZFS" != "no"; then
  dnl Check for libzfs
  AC_MSG_CHECKING([for libzfs])
  
  dnl FreeBSD specific paths
  ZFS_SEARCH_PATHS="/usr/local/include /usr/include /usr/src/cddl/lib/libzfs/common /usr/src/sys/cddl/contrib/opensolaris/lib/libzfs/common"
  ZFS_LIB_PATHS="/usr/local/lib /usr/lib /lib"
  
  for i in $ZFS_SEARCH_PATHS; do
    if test -f "$i/libzfs.h"; then
      ZFS_INCDIR=$i
      break
    fi
  done
  
  for i in $ZFS_LIB_PATHS; do
    if test -f "$i/libzfs.so" -o -f "$i/libzfs.a"; then
      ZFS_LIBDIR=$i
      break
    fi
  done
  
  if test -z "$ZFS_INCDIR" -o -z "$ZFS_LIBDIR"; then
    AC_MSG_ERROR([libzfs not found. Please install OpenZFS development libraries])
  fi
  
  AC_MSG_RESULT([found in $ZFS_INCDIR and $ZFS_LIBDIR])
  
  PHP_ADD_INCLUDE($ZFS_INCDIR)
  PHP_ADD_LIBRARY_WITH_PATH(zfs, $ZFS_LIBDIR, ZFS_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(zfs_core, $ZFS_LIBDIR, ZFS_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(nvpair, $ZFS_LIBDIR, ZFS_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(util, $ZFS_LIBDIR, ZFS_SHARED_LIBADD)
  
  PHP_SUBST(ZFS_SHARED_LIBADD)
  PHP_NEW_EXTENSION(zfs, php_zfs.c zfs_list.c zpool_list.c, $ext_shared)
fi
