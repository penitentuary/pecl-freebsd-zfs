PHP_ARG_ENABLE([zfs],
  [whether to enable ZFS support],
  [AS_HELP_STRING([--enable-zfs],
    [Enable ZFS support])],
  [no])

if test "$PHP_ZFS" != "no"; then
  dnl Check for libzfs headers
  AC_MSG_CHECKING([for libzfs headers])
  
  dnl FreeBSD specific paths for headers
  ZFS_SEARCH_PATHS="/usr/local/include /usr/include"
  ZFS_LIB_PATHS="/usr/local/lib /usr/lib"
  
  dnl Look for libzfs.h
  for i in $ZFS_SEARCH_PATHS; do
    if test -f "$i/libzfs.h"; then
      ZFS_INCDIR=$i
      AC_MSG_RESULT([found in $ZFS_INCDIR])
      break
    fi
  done
  
  if test -z "$ZFS_INCDIR"; then
    AC_MSG_ERROR([libzfs.h not found. Please install OpenZFS development libraries])
  fi
  
  dnl Look for libzfs library
  AC_MSG_CHECKING([for libzfs library])
  for i in $ZFS_LIB_PATHS; do
    if test -f "$i/libzfs.so" -o -f "$i/libzfs.a"; then
      ZFS_LIBDIR=$i
      AC_MSG_RESULT([found in $ZFS_LIBDIR])
      break
    fi
  done
  
  if test -z "$ZFS_LIBDIR"; then
    AC_MSG_ERROR([libzfs library not found. Please install OpenZFS development libraries])
  fi
  
  dnl Add include path
  PHP_ADD_INCLUDE($ZFS_INCDIR)
  
  dnl Add libraries
  PHP_ADD_LIBRARY_WITH_PATH(zfs, $ZFS_LIBDIR, ZFS_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(zfs_core, $ZFS_LIBDIR, ZFS_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(nvpair, $ZFS_LIBDIR, ZFS_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(util, $ZFS_LIBDIR, ZFS_SHARED_LIBADD)
  
  dnl Define the extension
  AC_DEFINE(HAVE_ZFS, 1, [Whether you have ZFS support])
  
  PHP_SUBST(ZFS_SHARED_LIBADD)
  PHP_NEW_EXTENSION(zfs, php_zfs.c zfs_list.c zpool_list.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
