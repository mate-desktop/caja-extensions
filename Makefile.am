
DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc --with-cajadir='$${libdir}/caja/extensions-2.0-distcheck'

SUBDIRS = \
	po \
	gksu \
	image-converter \
	open-terminal \
	sendto \
	share

EXTRA_DIST = \
	intltool-extract.in \
	intltool-update.in \
	intltool-merge.in

DISTCLEANFILES = \
	intltool-extract \
	intltool-update \
	intltool-merge \
	po/.intltool-merge-cache
