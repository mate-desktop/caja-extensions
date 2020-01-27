NULL =
AM_CPPFLAGS =						\
	$(DISABLE_DEPRECATED_CPPFLAGS)			\
	-DDATADIR=\"$(datadir)\"			\
	-DMATELOCALEDIR=\""$(datadir)/locale"\"		\
	-I$(top_srcdir)					\
	-I$(top_builddir)				\
	$(NULL)

AM_CFLAGS =						\
	$(WARN_CFLAGS)					\
	$(NULL)

caja_extensiondir=$(CAJA_EXTENSION_DIR)

caja_extension_LTLIBRARIES=libcaja-image-converter.la

libcaja_image_converter_la_SOURCES =			\
	image-converter.c				\
	caja-image-converter.c				\
	caja-image-converter.h				\
	caja-image-resizer.c				\
	caja-image-resizer.h				\
	caja-image-rotator.c				\
	caja-image-rotator.h				\
	$(NULL)
libcaja_image_converter_la_CPPFLAGS =			\
	$(AM_CPPFLAGS)					\
	-DG_LOG_DOMAIN=\"Caja-Image-Converter\"		\
	$(NULL)
libcaja_image_converter_la_CFLAGS =			\
	$(AM_CFLAGS)					\
	$(CAJA_CFLAGS)					\
	$(IMAGE_CONVERTER_CFLAGS)			\
	$(NULL)
libcaja_image_converter_la_LDFLAGS = -module -avoid-version
libcaja_image_converter_la_LIBADD =			\
	$(CAJA_LIBS)					\
	$(IMAGE_CONVERTER_LIBS)				\
	$(NULL)

builderdir = $(datadir)/caja-extensions
builder_DATA = 				\
	caja-image-resize.ui	\
	caja-image-rotate.ui

extensiondir = $(datadir)/caja/extensions
extension_in_files = libcaja-image-converter.caja-extension.desktop.in
extension_DATA = $(extension_in_files:.caja-extension.desktop.in=.caja-extension)
$(extension_DATA): $(extension_in_files)
	$(AM_V_GEN) $(MSGFMT) --desktop --keyword=Copyright --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@

EXTRA_DIST = $(builder_DATA)

CLEANFILES = $(extension_DATA)
