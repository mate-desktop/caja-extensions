interfacesdir = $(datadir)/caja-share
interfaces_DATA = share-dialog.ui
EXTRA_DIST = share-dialog.ui

libcaja_share_la_CFLAGS =				\
	-Wall						\
	-DG_LOG_DOMAIN=\"Caja-Share\"		\
	-DINTERFACES_DIR=\"$(interfacesdir)\"		\
	-DDATADIR=\"$(datadir)\"			\
	-DMATELOCALEDIR=\""$(datadir)/locale"\" 	\
	-I$(top_srcdir)					\
	-I$(top_builddir)				\
	$(WARN_CFLAGS)                                  \
	$(DISABLE_DEPRECATED_CFLAGS)			\
	$(CAJA_CFLAGS)				\
	$(SHARE_CFLAGS)

caja_extensiondir=$(CAJA_EXTENSION_DIR)
caja_extension_LTLIBRARIES=libcaja-share.la

libcaja_share_la_SOURCES =  \
	mate-file-manager-share.c        \
	mate-file-manager-share.h	\
	shares.c		\
	shares.h

libcaja_share_la_LDFLAGS = -module -avoid-version
libcaja_share_la_LIBADD  = $(SHARE_LIBS) $(CAJA_LIBS)
