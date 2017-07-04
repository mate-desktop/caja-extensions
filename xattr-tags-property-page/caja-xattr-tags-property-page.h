/*
 *  Caja xattr tags property page extension
 *
 *  Copyright (C) 2017 Felipe Barriga Richards
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Authors: Felipe Barriga Richards <spam@felipebarriga.cl>
 */
 
#ifndef CAJA_XATTR_TAGS_PROPERTY_PAGE_EXTENSION_H
#define CAJA_XATTR_TAGS_PROPERTY_PAGE_EXTENSION_H

#include <gtk/gtk.h>

#define CAJA_TYPE_XATTR_TAGS_PROPERTIES_PAGE caja_xattr_tags_properties_page_get_type()
#define CAJA_XATTS_TAGS_PROPERTIES_PAGE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), CAJA_TYPE_XATTR_TAGS_PROPERTIES_PAGE, CajaXattrTagsPropertiesPage))
#define CAJA_XATTS_TAGS_PROPERTIES_PAGE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), CAJA_TYPE_XATTR_TAGS_PROPERTIES_PAGE, CajaXattrTagsPropertiesPageClass))
#define CAJA_IS_XATTR_TAGS_PROPERTIES_PAGE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAJA_TYPE_XATTR_TAGS_PROPERTIES_PAGE))
#define CAJA_IS_XATTR_TAGS_PROPERTIES_PAGE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), CAJA_TYPE_XATTR_TAGS_PROPERTIES_PAGE))
#define CAJA_XATTS_TAGS_PROPERTIES_PAGE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAJA_TYPE_XATTR_TAGS_PROPERTIES_PAGE, CajaXattrTagsPropertiesPageClass))

typedef struct CajaXattrTagsPropertiesPageDetails CajaXattrTagsPropertiesPageDetails;

typedef struct
{
	GtkBox parent;
	CajaXattrTagsPropertiesPageDetails *details;
} CajaXattrTagsPropertiesPage;

typedef struct
{
	GtkBoxClass parent;
} CajaXattrTagsPropertiesPageClass;

GType caja_xattr_tags_properties_page_get_type (void);
void  caja_xattr_tags_properties_page_register (void);

#endif /* CAJA_XATTR_TAGS_PROPERTY_PAGE_EXTENSION_H */
