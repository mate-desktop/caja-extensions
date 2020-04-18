ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

SUBDIRS = po

if ENABLE_OPEN_TERMINAL
SUBDIRS += open-terminal
endif

if ENABLE_SENDTO
SUBDIRS += sendto
endif

if ENABLE_IMAGE_CONVERTER
SUBDIRS += image-converter
endif

if ENABLE_GKSU
SUBDIRS += gksu
endif

if ENABLE_SHARE
SUBDIRS += share
endif

if ENABLE_WALLPAPER
SUBDIRS += wallpaper
endif

if ENABLE_XATTR_TAGS
SUBDIRS += xattr-tags
endif

EXTRA_DIST = autogen.sh

DISTCLEANFILES =

DISTCHECK_CONFIGURE_FLAGS =   \
	--enable-gtk-doc \
	--with-cajadir='$${libdir}/caja/extensions-2.0-distcheck' \
	--enable-compile-warnings=no \
	CFLAGS='-Wno-deprecated-declarations'

# Build ChangeLog from GIT  history
ChangeLog:
	$(AM_V_GEN) if test -d $(top_srcdir)/.git; then \
		GIT_DIR="$(top_srcdir)/.git" git log --stat > $@; \
	fi

dist: ChangeLog

.PHONY: ChangeLog

