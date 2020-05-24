NULL =

AM_CPPFLAGS =					\
	$(DISABLE_DEPRECATED_CPPFLAGS)		\
	-DMATELOCALEDIR=\""$(datadir)/locale"\"	\
	-I$(top_builddir)			\
	-I$(top_srcdir)/sendto			\
	$(NULL)

AM_CFLAGS =					\
	$(WARN_CFLAGS)				\
	$(SENDTO_CFLAGS)			\
	$(NULL)

plugindir = $(libdir)/caja-sendto/plugins

plugin_LTLIBRARIES = libnstremovable_devices.la

libnstremovable_devices_la_SOURCES = removable-devices.c
libnstremovable_devices_la_CPPFLAGS =		\
	$(AM_CPPFLAGS)				\
	-DDATADIR=\"$(datadir)\"		\
	-DICONDIR=\"$(icondir)\"		\
	-I$(srcdir)/../ 			\
	$(NULL)
libnstremovable_devices_la_CFLAGS =		\
	$(AM_CFLAGS)				\
	$(NULL)
libnstremovable_devices_la_LDFLAGS = -module -avoid-version
libnstremovable_devices_la_LIBADD = $(GIO_LIBS) $(SENDTO_LIBS) $(builddir)/../libnstcommon.la

-include $(top_srcdir)/git.mk
