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

plugin_LTLIBRARIES = libnstgajim.la

libnstgajim_la_SOURCES = gajim.c
libnstgajim_la_CPPFLAGS =			\
	$(AM_CPPFLAGS)				\
	-DDATADIR=\"$(datadir)\"		\
	-DICONDIR=\"$(icondir)\"		\
	$(NULL)
libnstgajim_la_CFLAGS =				\
	$(AM_CFLAGS)				\
	$(DBUS_CFLAGS)				\
	$(NULL)
libnstgajim_la_LDFLAGS = -module -avoid-version
libnstgajim_la_LIBADD = $(DBUS_LIBS) $(SENDTO_LIBS)

