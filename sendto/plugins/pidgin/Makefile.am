NULL =

AM_CPPFLAGS =					\
	$(DISABLE_DEPRECATED_CPPFLAGS)		\
	-DMATELOCALEDIR=\""$(datadir)/locale"\"	\
	-I$(top_builddir)			\
	-I$(top_srcdir)/sendto 			\
	$(NULL)

AM_CFLAGS =					\
	$(WARN_CFLAGS)				\
	$(SENDTO_CFLAGS)			\
	$(NULL)

plugindir = $(libdir)/caja-sendto/plugins

plugin_LTLIBRARIES = libnstpidgin.la

libnstpidgin_la_SOURCES = pidgin.c
libnstpidgin_la_CPPFLAG =			\
	$(AM_CFLAGS)				\
	-DDATADIR=\"$(datadir)\"		\
	-DICONDIR=\"$(icondir)\"		\
	$(NULL)
libnstpidgin_la_CFLAGS =			\
	$(AM_CFLAGS)				\
	$(NST_EBOOK_CFLAGS)			\
	$(PIDGIN_CFLAGS)			\
	$(NULL)
libnstpidgin_la_LDFLAGS = -module -avoid-version
libnstpidgin_la_LIBADD =			\
	$(PIDGIN_LIBS)				\
	$(SENDTO_LIBS)				\
	$(NUll)

-include $(top_srcdir)/git.mk
