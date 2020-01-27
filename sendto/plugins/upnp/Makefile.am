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

plugin_LTLIBRARIES = libnstupnp.la

libnstupnp_la_SOURCES = upnp.c
libnstupnp_la_CPPFLAGS =			\
	$(AM_CPPFLAGS)				\
	-DDATADIR=\"$(datadir)\"		\
	-DICONDIR=\"$(icondir)\"		\
	$(NULL)
libnstupnp_la_CFLAGS =				\
	$(AM_CFLAGS)				\
	$(UPNP_CFLAGS)				\
	$(NULL)
libnstupnp_la_LDFLAGS = -module -avoid-version
libnstupnp_la_LIBADD = $(UPNP_LIBS) $(SENDTO_LIBS)
