AM_CPPFLAGS = \
    -DDATADIR=\"$(datadir)\"                            \
    -DMATELOCALEDIR=\""$(datadir)/locale"\"             \
    $(WARN_CFLAGS)                                      \
    $(CAJA_CFLAGS)

caja_extensiondir = $(CAJA_EXTENSION_DIR)
caja_extension_LTLIBRARIES = libcaja-xattr-tags-property-page.la

libcaja_xattr_tags_property_page_la_SOURCES =   \
    autocomplete.c                              \
    autocomplete.h                              \
    tags-tree-view.c                            \
    tags-tree-view.h                            \
    tags-utils.c                                \
    tags-utils.h                                \
    ui-utils.c                                  \
    ui-utils.h                                  \
    caja-xattr-tags-property-page.c             \
    caja-xattr-tags-property-page.h

libcaja_xattr_tags_property_page_la_LDFLAGS = -module -avoid-version
libcaja_xattr_tags_property_page_la_LIBADD = $(CAJA_LIBS)

gsettings_schemas_in_files = org.mate.caja-xattrs-tags-property-page.gschema.xml.in
gsettings_SCHEMAS = $(gsettings_schemas_in_files:.gschema.xml.in=.gschema.xml)

@INTLTOOL_XML_NOMERGE_RULE@

@GSETTINGS_RULES@

extensiondir = $(datadir)/caja/extensions
extension_in_files = libcaja-xattr-tags-property-page.caja-extension.in
extension_DATA = $(extension_in_files:.caja-extension.in=.caja-extension)
%.caja-extension: %.caja-extension.in \
    $(INTLTOOL_MERGE) $(wildcard $(top_srcdir)/po/*.po) ; $(AM_V_GEN) LC_ALL=C \
    $(INTLTOOL_MERGE) -d -u -c $(top_builddir)/po/.intltool-merge-cache \
    $(top_srcdir)/po $< $@

DISTCLEANFILES = \
    org.mate.caja-xattrs-tags-property-page.gschema.xml

CLEANFILES = $(extension_DATA)

EXTRA_DIST = \
    $(gsettings_schemas_in_files)
