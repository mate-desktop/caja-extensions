NULL =

interfacesdir = $(datadir)/caja-extensions
interfaces_DATA = share-dialog.ui

AM_CPPFLAGS =					\
	$(DISABLE_DEPRECATED_CPPFLAGS)		\
	-DMATELOCALEDIR=\""$(datadir)/locale"\"	\
	-I$(top_builddir)			\
	$(NULL)

AM_CFLAGS =					\
	$(WARN_CFLAGS)				\
	$(NULL)

caja_extensiondir = $(CAJA_EXTENSION_DIR)

caja_extension_LTLIBRARIES = libcaja-share.la

libcaja_share_la_SOURCES =			\
	caja-share.c				\
	caja-share.h				\
	shares.c				\
	shares.h				\
	$(NULL)
libcaja_share_la_CPPFLAGS =			\
	$(AM_CPPFLAGS)				\
	-DG_LOG_DOMAIN=\"Caja-Share\"		\
	-DINTERFACES_DIR=\"$(interfacesdir)\"	\
	-DDATADIR=\"$(datadir)\"		\
	$(NULL)
libcaja_share_la_CFLAGS =			\
	$(AM_CFLAGS)				\
	$(CAJA_CFLAGS)				\
	$(SHARE_CFLAGS)				\
	$(NULL)
libcaja_share_la_LDFLAGS = -module -avoid-version
libcaja_share_la_LIBADD  = $(SHARE_LIBS) $(CAJA_LIBS)

extensiondir = $(datadir)/caja/extensions
extension_in_files = libcaja-share.caja-extension.desktop.in
extension_DATA = $(extension_in_files:.caja-extension.desktop.in=.caja-extension)
$(extension_DATA): $(extension_in_files)
	$(AM_V_GEN) $(MSGFMT) --desktop --keyword=Copyright --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@

EXTRA_DIST = share-dialog.ui

CLEANFILES = $(extension_DATA)
