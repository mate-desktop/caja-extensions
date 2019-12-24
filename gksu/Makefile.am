AM_CFLAGS = -g -O2 -Wall
AM_CPPFLAGS = -DLOCALEDIR=\"$(datadir)/locale\" -DDATA_DIR=\"$(datadir)\" -DPREFIX=\"$(prefix)\" $(CAJA_CFLAGS)

caja_extensiondir=$(libdir)/caja/extensions-2.0
caja_extension_LTLIBRARIES = libcaja-gksu.la
libcaja_gksu_la_SOURCES = libcaja-gksu.c
libcaja_gksu_la_LDFLAGS = -module -avoid-version -no-undefined
libcaja_gksu_la_LIBADD  = $(CAJA_LIBS)

extensiondir = $(datadir)/caja/extensions
extension_in_files = libcaja-gksu.caja-extension.desktop.in
extension_DATA = $(extension_in_files:.caja-extension.desktop.in=.caja-extension)
$(extension_DATA): $(extension_in_files)
	$(AM_V_GEN) $(MSGFMT) --desktop --keyword=Copyright --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@

CLEANFILES = $(extension_DATA)
