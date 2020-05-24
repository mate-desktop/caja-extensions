NULL =

AM_CPPFLAGS =					\
	$(DISABLE_DEPRECATED_CPPFLAGS)		\
	-DMATELOCALEDIR=\""$(datadir)/locale"\"	\
	-I$(top_builddir)			\
	$(NULL)

AM_CFLAGS =					\
	$(WARN_CFLAGS)				\
	$(NULL)

caja_extensiondir = $(libdir)/caja/extensions-2.0

caja_extension_LTLIBRARIES = libcaja-gksu.la

libcaja_gksu_la_SOURCES = libcaja-gksu.c
libcaja_gksu_la_CPPFLAGS =			\
	$(AM_CPPFLAGS)				\
	-DDATA_DIR=\"$(datadir)\"		\
	-DPREFIX=\"$(prefix)\"			\
	$(NULL)
libcaja_gksu_la_CFLAGS =			\
	$(AM_CFLAGS)				\
	$(CAJA_CFLAGS)				\
	$(NULL)
libcaja_gksu_la_LDFLAGS = -module -avoid-version -no-undefined
libcaja_gksu_la_LIBADD  = $(CAJA_LIBS)

extensiondir = $(datadir)/caja/extensions
extension_in_files = libcaja-gksu.caja-extension.desktop.in
extension_DATA = $(extension_in_files:.caja-extension.desktop.in=.caja-extension)
$(extension_DATA): $(extension_in_files)
	$(AM_V_GEN) $(MSGFMT) --desktop --keyword= --keyword=Copyright --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@

CLEANFILES = $(extension_DATA)

-include $(top_srcdir)/git.mk
