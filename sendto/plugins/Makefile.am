SUBDIRS = . \
	caja-burn \
	emailclient \
	gajim \
	pidgin \
	removable-devices \
	upnp

NST_COMMON_SOURCES = nst-common.c nst-common.h

noinst_LTLIBRARIES = libnstcommon.la
libnstcommon_la_SOURCES = $(NST_COMMON_SOURCES)
libnstcommon_la_CFLAGS = $(SENDTO_CFLAGS)
libnstcommon_la_LIBADD = $(SENDTO_LIBS)

EXTRA_DIST = $(NST_COMMON_SOURCES)
