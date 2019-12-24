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
gsettings_SCHEMAS = $(gsettings_schemas_in_files:.xml.in=.xml)
@GSETTINGS_RULES@

extensiondir = $(datadir)/caja/extensions
extension_in_files = libcaja-open-terminal.caja-extension.desktop.in
extension_DATA = $(extension_in_files:.caja-extension.desktop.in=.caja-extension)
$(extension_DATA): $(extension_in_files)
	$(AM_V_GEN) $(MSGFMT) --desktop --keyword=Copyright --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@

DISTCLEANFILES = \
	org.mate.caja-open-terminal.gschema.xml

CLEANFILES = \
	$(extension_DATA) \
	$(gsettings_SCHEMAS) \
	*.gschema.valid

EXTRA_DIST = \
	$(gsettings_schemas_in_files)
