##*****************************************************************************
#  COPIER:
#         Rod Schultz <rod.schultz@bull.com>
#    from example writtey by
#         Morris Jette <jette1@llnl.gov>
#
#  SYNOPSIS:
#    X_AC_HDF5
#
#  DESCRIPTION:
#    Determine if the HDF5 libraries exists and if they support PCI data. 
##*****************************************************************************

AC_DEFUN([X_AC_HDF5],
[
  _x_ac_hdf5_dirs="/usr /usr/local"
  _x_ac_hdf5_libs="lib64 lib"
  x_ac_cv_hdf5_pci="no"

  AC_ARG_WITH(
    [hdf5],
    AS_HELP_STRING(--with-hdf5=PATH,Specify path to hdf5 installation),
    [_x_ac_hdf5_dirs="$withval $_x_ac_hdf5_dirs"])

  AC_CACHE_CHECK(
    [for hdf5 installation],
    [x_ac_cv_hdf5_dir],
    [
      for d in $_x_ac_hdf5_dirs; do
        test -d "$d" || continue
        test -d "$d/include" || continue
        test -f "$d/include/hdf5.h" || continue
	for bit in $_x_ac_hdf5_libs; do
          test -d "$d/$bit" || continue
          _x_ac_hdf5_cppflags_save="$CPPFLAGS"
          CPPFLAGS="-I$d/include $CPPFLAGS"
 	  _x_ac_hdf5_libs_save="$LIBS"
          LIBS="-L$d/$bit -lhdf5 $LIBS"
          AC_LINK_IFELSE(
            [AC_LANG_CALL([], H5close)],
            AS_VAR_SET(x_ac_cv_hdf5_dir, $d))
          AC_TRY_LINK([#include <hdf5.h>],
                      [int i = HDF5_OBJ_PCI_DEVICE;],
                      [x_ac_cv_hdf5_pci="yes"], [])
          CPPFLAGS="$_x_ac_hdf5_cppflags_save"
          LIBS="$_x_ac_hdf5_libs_save"
          test -n "$x_ac_cv_hdf5_dir" && break
	done
        test -n "$x_ac_cv_hdf5_dir" && break
      done
    ])
  if test -z "$x_ac_cv_hdf5_dir"; then
    AC_MSG_WARN([unable to locate hdf5 installation])
  else
    HDF5_CPPFLAGS="-I$x_ac_cv_hdf5_dir/include"
    HDF5_LDFLAGS="-Wl,-rpath -Wl,$x_ac_cv_hdf5_dir/$bit -L$x_ac_cv_hdf5_dir/$bit"
    HDF5_LIBS="-lhdf5"
    AC_DEFINE(HAVE_HDF5, 1, [Define to 1 if hdf5 library found])
    if test "$x_ac_cv_hdf5_pci" = "yes"; then
      AC_DEFINE(HAVE_HDF5_PCI, 1, [Define to 1 if hdf5 library supports PCI devices])
    fi
  fi

  AC_SUBST(HDF5_LIBS)
  AC_SUBST(HDF5_CPPFLAGS)
  AC_SUBST(HDF5_LDFLAGS)
])
