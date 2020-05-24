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

plugin_LTLIBRARIES = libnstburn.la

libnstburn_la_SOURCES = caja-burn.c
libnstburn_la_CPPFLAGS =			\
	$(AM_CPPFLAGS)				\
	-DDATADIR=\"$(datadir)\"		\
	-DICONDIR=\"$(icondir)\"		\
	-I$(srcdir)/../				\
	$(NULL)
libnstburn_la_CFLAGS =				\
	$(AM_CFLAGS)				\
	$(GIO_CFLAGS)				\
	$(NULL)
libnstburn_la_LDFLAGS = -module -avoid-version
libnstburn_la_LIBADD = $(GIO_LIBS) $(SENDTO_LIBS) $(builddir)/../libnstcommon.la

-include $(top_srcdir)/git.mk
