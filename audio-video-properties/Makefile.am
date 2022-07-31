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

caja_extension_LTLIBRARIES=libcaja-av.la

resources_built_sources = caja-av-resources.h caja-av-resources.c
$(resources_built_sources): org.mate.caja.extensions.totemproperties.gresource.xml Makefile $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir $(srcdir) $(srcdir)/org.mate.caja.extensions.totemproperties.gresource.xml)
	$(AM_V_GEN) XMLLINT=$(XMLLINT) $(GLIB_COMPILE_RESOURCES) --target $@ --sourcedir $(srcdir) --generate --c-name totemproperties $<

libcaja_av_la_SOURCES =					\
	totem-properties-main.c				\
	totem-properties-view.c				\
	totem-properties-view.h				\
	bacon-video-widget-properties.c			\
	bacon-video-widget-properties.h			\
	totem-gst-helpers.c				\
	totem-gst-helpers.h				\
	totem-mime-types.h 				\
	$(resources_built_sources)			\
	$(NULL)
libcaja_av_la_CPPFLAGS =				\
	$(AM_CPPFLAGS)					\
	-DG_LOG_DOMAIN=\"Caja-Image-Converter\"		\
	$(NULL)
libcaja_av_la_CFLAGS =					\
	$(AM_CFLAGS)					\
	$(CAJA_CFLAGS)					\
	$(TOTEM_PROPERTIES_CFLAGS)			\
	$(NULL)
libcaja_av_la_LDFLAGS = -module -avoid-version
libcaja_av_la_LIBADD =					\
	-lm						\
	$(CAJA_LIBS)					\
	$(TOTEM_PROPERTIES_LIBS)			\
	$(NULL)

extensiondir = $(datadir)/caja/extensions
extension_in_files = libcaja-av.caja-extension.desktop.in
extension_DATA = $(extension_in_files:.caja-extension.desktop.in=.caja-extension)
$(extension_DATA): $(extension_in_files)
if USE_NLS
	$(AM_V_GEN) $(MSGFMT) --desktop --keyword= --keyword=Copyright --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@
else
	$(AM_V_GEN) cp $< $@
endif

EXTRA_DIST =						\
	ui/properties.ui				\
	org.mate.caja.extensions.totemproperties.gresource.xml \
	$(NULL)

CLEANFILES =						\
	$(extension_DATA)				\
	$(resources_built_sources)			\
	$(NULL)

-include $(top_srcdir)/git.mk
