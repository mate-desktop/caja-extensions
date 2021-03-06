NULL =
SUBDIRS = . docs plugins

AM_CPPFLAGS =					\
	$(DISABLE_DEPRECATED_CPPFLAGS)		\
	-DDATADIR=\"$(datadir)\"		\
	-DPLUGINDIR=\"$(libdir)/caja-sendto/plugins\"	\
	-DMATELOCALEDIR="\"$(datadir)/locale\""	\
	-I$(top_srcdir)				\
	-I$(top_builddir)			\
	$(NULL)

AM_CFLAGS =					\
	$(WARN_CFLAGS)				\
	$(SENDTO_CFLAGS)			\
	$(CAJA_CFLAGS)				\
	$(NULL)

caja_sendto_includedir = $(includedir)/caja-sendto/
caja_sendto_include_HEADERS = caja-sendto-plugin.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = caja-sendto.pc

noinst_LTLIBRARIES = libnstplugin.la
libnstplugin_la_SOURCES =			\
	caja-sendto-plugin.h			\
	nst-enum-types.c			\
	nst-enum-types.h
libnstplugin_la_LIBADD = $(SENDTO_LIBS)

bin_PROGRAMS = caja-sendto

resources_built_sources = caja-sendto-resources.h caja-sendto-resources.c
$(resources_built_sources): org.mate.caja.extensions.sendto.gresource.xml Makefile $(shell $(GLIB_COMPILE_RESOURCES) --generate-dependencies --sourcedir $(srcdir) $(srcdir)/org.mate.caja.extensions.sendto.gresource.xml)
	$(AM_V_GEN) XMLLINT=$(XMLLINT) $(GLIB_COMPILE_RESOURCES) --target $@ --sourcedir $(srcdir) --generate --c-name sendto $<

caja_sendto_SOURCES = caja-sendto-command.c $(resources_built_sources)
caja_sendto_LDADD = $(SENDTO_LIBS) libnstplugin.la

caja_extensiondir = $(CAJA_EXTENSION_DIR)
caja_extension_LTLIBRARIES = libcaja-sendto.la
libcaja_sendto_la_SOURCES =		\
	caja-nste.c			\
	caja-nste.h			\
	caja-sendto-module.c

libcaja_sendto_la_LDFLAGS = -module -avoid-version -no-undefined
libcaja_sendto_la_LIBADD  = $(CAJA_LIBS)

nst_headers_to_scan_for_enums = caja-sendto-plugin.h
# Generate the enums source code, with glib-mkenums:
# This is based on the same Makefile.am stuff in pango:
nst_built_headers = nst-enum-types.h
nst_built_cfiles = nst-enum-types.c

nst-enum-types.h: $(nst_headers_to_scan_for_enums) Makefile
	$(AM_V_GEN) (cd $(srcdir) && glib-mkenums \
			--fhead "#ifndef __NST_ENUM_TYPES_H__\n#define __NST_ENUM_TYPES_H__\n\n#include <glib-object.h>\n\nG_BEGIN_DECLS\n" \
			--fprod "/* enumerations from \"@filename@\" */\n" \
			--vhead "GType @enum_name@_get_type (void);\n#define NST_TYPE_@ENUMSHORT@ (@enum_name@_get_type())\n" 	\
			--ftail "G_END_DECLS\n\n#endif /* __NST_ENUM_TYPES_H__ */" \
		$(nst_headers_to_scan_for_enums)) > $@

nst-enum-types.c: $(nst_headers_to_scan_for_enums) Makefile nst-enum-types.h
	$(AM_V_GEN) (cd $(srcdir) && glib-mkenums \
			--fhead "#include <caja-sendto-plugin.h>\n" \
			--fhead "#include \"nst-enum-types.h\"\n" \
			--fhead "#include <glib-object.h>" \
			--fprod "\n/* enumerations from \"@filename@\" */" \
			--vhead "GType\n@enum_name@_get_type (void)\n{\n  static GType etype = 0;\n  if (etype == 0) {\n    static const G@Type@Value values[] = {" 	\
			--vprod "      { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
			--vtail "      { 0, NULL, NULL }\n    };\n    etype = g_@type@_register_static (\"@EnumName@\", values);\n  }\n  return etype;\n}\n" \
		$(nst_headers_to_scan_for_enums)) > $@

gsettingsschema_in_files = org.mate.Caja.Sendto.gschema.xml.in
gsettings_SCHEMAS = $(gsettingsschema_in_files:.xml.in=.xml)
@GSETTINGS_RULES@

man_MANS = caja-sendto.1

extensiondir = $(datadir)/caja/extensions
extension_in_files = libcaja-sendto.caja-extension.desktop.in
extension_DATA = $(extension_in_files:.caja-extension.desktop.in=.caja-extension)
$(extension_DATA): $(extension_in_files)
if USE_NLS
	$(AM_V_GEN) $(MSGFMT) --desktop --keyword= --keyword=Copyright --keyword=Name --keyword=Description --template $< -d $(top_srcdir)/po -o $@
else
	$(AM_V_GEN) cp $< $@
endif

EXTRA_DIST = \
	$(man_MANS) \
	caja-sendto.pc.in \
	caja-sendto.ui \
	$(gsettingsschema_in_files) \
	org.mate.caja.extensions.sendto.gresource.xml \
	$(NULL)

CLEANFILES = \
	$(nst_built_headers) \
	$(nst_built_cfiles) \
	$(gsettings_SCHEMAS) \
	$(extension_DATA) \
	*.gschema.valid \
	$(resources_built_sources) \
	$(NULL)

-include $(top_srcdir)/git.mk
