NULL =

AM_CPPFLAGS =					\
	$(DISABLE_DEPRECATED_CPPFLAGS)		\
	-DMATELOCALEDIR=\""$(datadir)/locale"\"	\
	-I$(top_builddir)			\
	$(NULL)

AM_CFLAGS =					\
	$(WARN_CFLAGS)				\
	$(NULL)

caja_extensiondir = $(CAJA_EXTENSION_DIR)

caja_extension_LTLIBRARIES = libcaja-wallpaper.la

libcaja_wallpaper_la_SOURCES =			\
	caja-wallpaper-extension.c		\
	caja-wallpaper-extension.h		\
	caja-wallpaper-command.c		\
	$(NULL)
libcaja_wallpaper_la_CPPFLAGS =			\
	$(AM_CPPFLAGS)				\
	$(NULL)
libcaja_wallpaper_la_CFLAGS =			\
	$(AM_CFLAGS)				\
	$(CAJA_CFLAGS)				\
	$(NULL)
libcaja_wallpaper_la_LDFLAGS = -module -avoid-version
libcaja_wallpaper_la_LIBADD = $(CAJA_LIBS)

extensiondir = $(datadir)/caja/extensions
extension_in_files = libcaja-wallpaper.caja-extension.desktop.in
extension_DATA = $(extension_in_files:.caja-extension.desktop.in=.caja-extension)
$(extension_DATA): $(extension_in_files)
if USE_NLS
	$(AM_V_GEN) $(MSGFMT) --desktop --keyword= --keyword=Copyright --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@
else
	$(AM_V_GEN) cp $< $@
endif

CLEANFILES = $(extension_DATA)

-include $(top_srcdir)/git.mk
