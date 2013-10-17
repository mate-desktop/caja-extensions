AC_INIT([caja-extensions], 1.7.0, [https://github.com/mate-desktop/caja-extensions])

AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS(config.h)

CAJA_REQUIRED=1.5.0
LIBMATE_DESKTOP_REQUIRED=1.5.0
GLIB_REQUIRED=2.13.3

GLIB_GSETTINGS

AC_PROG_CC
AM_PROG_LIBTOOL

# Check for pkgconfig first
AC_CHECK_PROG(HAVE_PKGCONFIG, pkg-config, yes, no)
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

# Give error and exit if we dont have pkgconfig
if test "x$HAVE_PKGCONFIG" = "xno"; then
  AC_MSG_ERROR([you need to have pkgconfig installed !])
fi

# Common requirements
PKG_CHECK_MODULES(CAJA, libcaja-extension >= $CAJA_REQUIRED glib-2.0 >= $GLIB_REQUIRED)
AC_SUBST(CAJA_CFLAGS)
AC_SUBST(CAJA_LIBS)

# open-terminal
PKG_CHECK_MODULES(OPENTERMINAL, mate-desktop-2.0 >= $LIBMATE_DESKTOP_REQUIRED)
AC_SUBST(OPENTERMINAL_CFLAGS)
AC_SUBST(OPENTERMINAL_LIBS)

# Get caja extensions directory
CAJA_EXTENSION_DIR=`$PKG_CONFIG --variable=extensiondir libcaja-extension`
AC_SUBST(CAJA_EXTENSION_DIR)

# intltool stuff
AC_PROG_INTLTOOL(0.18)
dnl AM_WITH_NLS
GETTEXT_PACKAGE=caja-extensions
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [gettext package])
AM_GLIB_GNU_GETTEXT

AC_OUTPUT([
  Makefile 
  open-terminal/Makefile
  po/Makefile.in 
])

