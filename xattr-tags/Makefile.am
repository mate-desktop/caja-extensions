AM_CPPFLAGS = $(CAJA_CFLAGS)

caja_extensiondir = $(CAJA_EXTENSION_DIR)
caja_extension_LTLIBRARIES = libcaja-xattr-tags.la

libcaja_xattr_tags_la_SOURCES = \
	caja-xattr-tags-extension.c \
	caja-xattr-tags-extension.h

libcaja_xattr_tags_la_LDFLAGS = -module -avoid-version
libcaja_xattr_tags_la_LIBADD = $(CAJA_LIBS)

extensiondir = $(datadir)/caja/extensions
extension_in_files = libcaja-xattr-tags.caja-extension.desktop.in
extension_DATA = $(extension_in_files:.caja-extension.desktop.in=.caja-extension)
$(extension_DATA): $(extension_in_files)
	$(AM_V_GEN) $(MSGFMT) --desktop --keyword= --keyword=Copyright --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@

CLEANFILES = $(extension_DATA)
