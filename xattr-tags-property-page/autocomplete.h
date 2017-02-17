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

#ifndef CAJA_EXTENSIONS_AUTOCOMPLETE_H
#define CAJA_EXTENSIONS_AUTOCOMPLETE_H

G_BEGIN_DECLS

#define CAJA_XATTRS_SCHEMA "org.mate.caja-xattr-tags-property-page"

#define CAJA_XATTRS_SCHEMA_RECENT_TAGS_ENABLED_KEY "recent-tags-enabled"
#define CAJA_XATTRS_SCHEMA_RECENT_TAGS_CAPACITY_KEY "recent-tags-capacity"
#define CAJA_XATTRS_SCHEMA_RECENT_TAGS_LIST_KEY "recent-tags-list"

gboolean recent_tags_enabled();

gboolean del_item_autocomplete (GtkEntryCompletion *completion, const gchar *tag);
gboolean add_item_autocomplete (GtkEntryCompletion *completion, const gchar *tag);

gboolean add_to_recent_tags (const gchar *tag);
GtkEntryCompletion * setup_entry_completion (GtkEntry *entry, GList *tags);


G_END_DECLS

#endif //CAJA_EXTENSIONS_AUTOCOMPLETE_H
