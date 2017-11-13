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

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include "tags-tree-view.h"


GtkTreeModel *
create_and_fill_model (GList *tags)
{
    GtkListStore *store;
    GtkTreeIter iter;

    store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, GDK_TYPE_PIXBUF);

    const GList *tags_iter = NULL;
    for (tags_iter = tags; tags_iter; tags_iter = tags_iter->next) {
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store,
                            &iter,
                            COL_NAME, tags_iter->data,
                            -1);

    }

    return GTK_TREE_MODEL (store);
}

GtkWidget *
create_view_and_model (GList *tags)
{
    GtkCellRenderer *renderer;
    GtkTreeModel *model;
    GtkWidget *view;

    view = gtk_tree_view_new ();

    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                                 -1,
                                                 _("Tags"),
                                                 renderer,
                                                 "text", COL_NAME,
                                                 NULL);


    model = create_and_fill_model (tags);
    gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);
    g_object_unref (model);

    GtkTreeSelection *selection
        = gtk_tree_view_get_selection (GTK_TREE_VIEW (view));

    gtk_tree_selection_set_mode (selection, GTK_SELECTION_MULTIPLE);

    return view;
}
