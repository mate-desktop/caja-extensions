AM_CFLAGS = -g -O2 -Wall
AM_CPPFLAGS = -DLOCALEDIR=\"$(datadir)/locale\" -DDATA_DIR=\"$(datadir)\" -DPREFIX=\"$(prefix)\" $(CAJA_CFLAGS)

caja_extensiondir=$(libdir)/caja/extensions-2.0
caja_extension_LTLIBRARIES = libcaja-beesu.la
libcaja_beesu_la_SOURCES = libcaja-beesu.c
libcaja_beesu_la_LDFLAGS = LDFLAGS = -module -avoid-version -no-undefined
libcaja_beesu_la_LIBADD  = $(CAJA_LIBS)
