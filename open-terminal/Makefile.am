AM_CPPFLAGS =						\
	-DG_LOG_DOMAIN=\"Caja-Open-Terminal\"	\
	-DDATADIR=\"$(datadir)\"			\
	-DMATELOCALEDIR=\""$(datadir)/locale"\" 	\
	-I$(top_srcdir)					\
	-I$(top_builddir)				\
	$(WARN_CFLAGS)                                  \
	$(DISABLE_DEPRECATED_CFLAGS)			\
	$(CAJA_CFLAGS)				\
	$(OPENTERMINAL_CFLAGS)

caja_extensiondir=$(CAJA_EXTENSION_DIR)

caja_extension_LTLIBRARIES=libcaja-open-terminal.la

libcaja_open_terminal_la_SOURCES = \
	caja-open-terminal.c       \
	caja-open-terminal.h       \
	open-terminal.c

libcaja_open_terminal_la_LDFLAGS = -module -avoid-version
libcaja_open_terminal_la_LIBADD  = $(CAJA_LIBS) $(OPENTERMINAL_LIBS)

gsettings_schemas_in_files = org.mate.caja-open-terminal.gschema.xml.in
gsettings_SCHEMAS = $(gsettings_schemas_in_files:.gschema.xml.in=.gschema.xml)

@INTLTOOL_XML_NOMERGE_RULE@

@GSETTINGS_RULES@

convertdir = $(datadir)/MateConf/gsettings
convert_DATA = caja-open-terminal.convert

extensiondir = $(datadir)/caja/extensions
extension_in_files = libcaja-open-terminal.caja-extension.in
extension_DATA = $(extension_in_files:.caja-extension.in=.caja-extension)
%.caja-extension: %.caja-extension.in $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*.po) ; $(AM_V_GEN) LC_ALL=C $(INTLTOOL_MERGE) -d -u -c $(top_builddir)/po/.intltool-merge-cache $(top_srcdir)/po $< $@

DISTCLEANFILES = \
	org.mate.caja-open-terminal.gschema.xml

EXTRA_DIST = \
	$(convert_DATA) \
	$(gsettings_schemas_in_files)
