AM_CPPFLAGS = $(CAJA_CFLAGS)

caja_extensiondir = $(CAJA_EXTENSION_DIR)
caja_extension_LTLIBRARIES = libcaja-wallpaper.la

libcaja_wallpaper_la_SOURCES = \
	caja-wallpaper-extension.c\
	caja-wallpaper-extension.h\
	caja-wallpaper-command.c

libcaja_wallpaper_la_LDFLAGS = -module -avoid-version
libcaja_wallpaper_la_LIBADD = $(CAJA_LIBS)

extensiondir = $(datadir)/caja/extensions
extension_in_files = libcaja-wallpaper.caja-extension.desktop.in
extension_DATA = $(extension_in_files:.caja-extension.desktop.in=.caja-extension)
$(extension_DATA): $(extension_in_files)
	$(AM_V_GEN) $(MSGFMT) --desktop --keyword=Copyright --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@

CLEANFILES = $(extension_DATA)
