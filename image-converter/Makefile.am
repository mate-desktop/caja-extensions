NULL =
AM_CPPFLAGS =						\
	$(DISABLE_DEPRECATED_CPPFLAGS)			\
	-DMATELOCALEDIR=\""$(datadir)/locale"\"		\
	-I$(top_srcdir)					\
	-I$(top_builddir)				\
	$(NULL)

AM_CFLAGS =						\
	$(WARN_CFLAGS)					\
	$(NULL)

caja_extensiondir=$(CAJA_EXTENSION_DIR)

caja_extension_LTLIBRARIES=libcaja-image-converter.la

resources_built_sources = caja-image-converter-resources.h caja-image-converter-resources.c
$(resources_built_sources): org.mate.caja.extensions.imageconverter.gresource.xml Makefile $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir $(srcdir) $(srcdir)/org.mate.caja.extensions.imageconverter.gresource.xml)
	$(AM_V_GEN) XMLLINT=$(XMLLINT) $(GLIB_COMPILE_RESOURCES) --target $@ --sourcedir $(srcdir) --generate --c-name imageconverter $<

libcaja_image_converter_la_SOURCES =			\
	image-converter.c				\
	caja-image-converter.c				\
	caja-image-converter.h				\
	caja-image-resizer.c				\
	caja-image-resizer.h				\
	caja-image-rotator.c				\
	caja-image-rotator.h				\
	$(resources_built_sources)			\
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

extensiondir = $(datadir)/caja/extensions
extension_in_files = libcaja-image-converter.caja-extension.desktop.in
extension_DATA = $(extension_in_files:.caja-extension.desktop.in=.caja-extension)
$(extension_DATA): $(extension_in_files)
if USE_NLS
	$(AM_V_GEN) $(MSGFMT) --desktop --keyword= --keyword=Copyright --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@
else
	$(AM_V_GEN) cp $< $@
endif

EXTRA_DIST =						\
	caja-image-resize.ui				\
	caja-image-rotate.ui				\
	error-dialog.ui					\
	org.mate.caja.extensions.imageconverter.gresource.xml \
	$(NULL)

CLEANFILES =						\
	$(extension_DATA)				\
	$(resources_built_sources)			\
	$(NULL)

-include $(top_srcdir)/git.mk
