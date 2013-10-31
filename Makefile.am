
DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc --with-cajadir='$${libdir}/caja/extensions-2.0-distcheck'

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

EXTRA_DIST = \
	intltool-extract.in \
	intltool-update.in \
	intltool-merge.in

DISTCLEANFILES = \
	intltool-extract \
	intltool-update \
	intltool-merge \
	po/.intltool-merge-cache
