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

plugin_LTLIBRARIES = libnstemailclient.la

libnstemailclient_la_SOURCES = emailclient.c
libnstemailclient_la_CPPFLAGS =			\
	$(AM_CPPFLAGS)				\
	-DDATADIR=\"$(datadir)\"		\
	-DICONDIR=\"$(icondir)\"		\
	$(NULL)
libnstemailclient_la_CFLAGS =			\
	$(AM_CFLAGS)				\
	$(EMAILCLIENT_CFLAGS)			\
	$(NULL)
libnstemailclient_la_LDFLAGS = -module -avoid-version
libnstemailclient_la_LIBADD = $(SENDTO_LIBS) $(EMAILCLIENT_LIBS)

-include $(top_srcdir)/git.mk
