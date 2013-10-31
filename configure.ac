AC_INIT([caja-extensions], 1.7.0, [https://github.com/mate-desktop/caja-extensions])

AM_INIT_AUTOMAKE([1.9 no-dist-gzip dist-xz tar-ustar])
AC_CONFIG_HEADERS(config.h)

AM_DISABLE_STATIC
AC_PROG_CC
AM_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_MAKE_SET

CAJA_REQUIRED=1.5.0
LIBMATE_DESKTOP_REQUIRED=1.5.0
GLIB_REQUIRED=2.26.0

GLIB_GSETTINGS

GTK_DOC_CHECK(1.9)

MATE_COMMON_INIT

# Check for pkgconfig first
AC_CHECK_PROG(HAVE_PKGCONFIG, pkg-config, yes, no)
AC_PATH_PROG(PKG_CONFIG, pkg-config, no)

# Give error and exit if we dont have pkgconfig
if test "x$HAVE_PKGCONFIG" = "xno"; then
  AC_MSG_ERROR([you need to have pkgconfig installed !])
fi

GLIB_GENMARSHAL=`$PKG_CONFIG --variable=glib_genmarshal glib-2.0`
AC_SUBST(GLIB_GENMARSHAL)

AC_MSG_CHECKING([GTK+ version])
AC_ARG_WITH([gtk],
  [AS_HELP_STRING([--with-gtk=2.0|3.0],[which gtk+ version to compile against (default: 2.0)])],
  [case "$with_gtk" in
     2.0|3.0) ;;
     *) AC_MSG_ERROR([invalid gtk version specified]) ;;
   esac],
  [with_gtk=2.0])
AC_MSG_RESULT([$with_gtk])

case "$with_gtk" in
  2.0) GTK_API_VERSION=2.0
       GTK_REQUIRED=2.18.0
       ;;
  3.0) GTK_API_VERSION=3.0
       GTK_REQUIRED=3.0.0
       ;;
esac

# Common requirements
PKG_CHECK_MODULES(CAJA,
                  libcaja-extension >= $CAJA_REQUIRED)
AC_SUBST(CAJA_CFLAGS)
AC_SUBST(CAJA_LIBS)

# image-converter extension
PKG_CHECK_MODULES(IMAGE_CONVERTER,
                  gtk+-$GTK_API_VERSION  >= $GTK_REQUIRED
                  glib-2.0  >= $GLIB_REQUIRED
                  gio-2.0   >= $GLIB_REQUIRED)
AC_SUBST(IMAGE_CONVERTER_CFLAGS)
AC_SUBST(IMAGE_CONVERTER_LIBS)

# open-terminal extension
PKG_CHECK_MODULES(OPENTERMINAL,
                  glib-2.0           >= $GLIB_REQUIRED
                  mate-desktop-2.0   >= $LIBMATE_DESKTOP_REQUIRED)
AC_SUBST(OPENTERMINAL_CFLAGS)
AC_SUBST(OPENTERMINAL_LIBS)

# sendto extension
GTHREAD_REQUIRED=2.6.0
DBUS_REQUIRED=1.0
DBUS_GLIB_REQUIRED=0.60
GUPNP_REQUIRED=0.13
PKG_CHECK_MODULES(SENDTO,
                  glib-2.0    >= $GLIB_REQUIRED
                  gio-2.0     >= $GLIB_REQUIRED
                  gthread-2.0 >= $GTHREAD_REQUIRED
                  gmodule-2.0 >= $GLIB_REQUIRED
                  gobject-2.0 >= $GLIB_REQUIRED
                  gtk+-$GTK_API_VERSION >= $GTK_REQUIRED
                  dbus-1      >= $DBUS_REQUIRED
                  dbus-glib-1 >= $DBUS_GLIB_REQUIRED
                  gupnp-1.0   >= $GUPNP_REQUIRED)
AC_SUBST(SENDTO_CFLAGS)
AC_SUBST(SENDTO_LIBS)
AC_DEFINE_UNQUOTED(GAJIM_SHARE_DIR, "$prefix/share/gajim", [path to gajim share dir])

# share extension
PKG_CHECK_MODULES(SHARE,
                  glib-2.0 >= $GLIB_REQUIRED)
AC_SUBST(SHARE_CFLAGS)
AC_SUBST(SHARE_LIBS)

# Get caja extensions directory
AC_ARG_WITH(cajadir,
    AS_HELP_STRING([--with-cajadir=DIR],[Installation path for Caja extension @<:@auto@:>@]),
    [ac_with_cajadir=$withval],
    [ac_with_cajadir=""])
if test "${ac_with_cajadir}" = ""; then
    ac_with_cajadir=`$PKG_CONFIG --variable=extensiondir libcaja-extension`
fi
AC_MSG_NOTICE([Installing caja plugins in ${ac_with_cajadir}])
AC_SUBST([CAJA_EXTENSION_DIR],[${ac_with_cajadir}])

# intltool stuff
AC_PROG_INTLTOOL(0.18)
GETTEXT_PACKAGE=caja-extensions
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [gettext package])
AM_GLIB_GNU_GETTEXT

AC_OUTPUT([
  Makefile 
  gksu/Makefile
  image-converter/Makefile
  open-terminal/Makefile
  sendto/Makefile
  sendto/caja-sendto.pc
  sendto/docs/Makefile
  sendto/docs/version.xml
  sendto/plugins/Makefile
  sendto/plugins/emailclient/Makefile
  sendto/plugins/gajim/Makefile
  sendto/plugins/caja-burn/Makefile
  sendto/plugins/pidgin/Makefile
  sendto/plugins/removable-devices/Makefile
  sendto/plugins/upnp/Makefile
  share/Makefile
  po/Makefile.in 
])

