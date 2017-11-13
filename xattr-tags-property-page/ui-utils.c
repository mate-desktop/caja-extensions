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

#include "ui-utils.h"

GtkWidget *
append_label (GtkWidget *vbox, const char *str)
{
  GtkWidget *label = gtk_label_new (NULL);

  gtk_label_set_markup (GTK_LABEL (label), str);
#if GTK_CHECK_VERSION (3, 16, 0)
  gtk_label_set_xalign (GTK_LABEL (label), 0);
  gtk_label_set_yalign (GTK_LABEL (label), 0);
#else
  gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
#endif

  gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
  gtk_widget_show (label);

  return label;
}

void
set_cursor (GtkWidget *widget, const gchar *name)
{
  GtkWidget *toplevel = gtk_widget_get_toplevel (widget);
  GdkDisplay *display = gtk_widget_get_display (toplevel);
  GdkCursor *cursor = gdk_cursor_new_from_name (display, name);

  if (cursor) {
    GdkWindow *window = gtk_widget_get_window (toplevel);
    if (window == NULL ) {
      g_debug ("set_cursor: cannot set cursor, window not found.\n");
    } else {
      gdk_window_set_cursor (GDK_WINDOW (window), cursor);
    }
  }
}

void
display_error_dialog (GtkWidget *page, gchar *message)
{
  g_assert (GTK_IS_WIDGET (page));

  GtkWidget *toplevel = gtk_widget_get_toplevel (GTK_WIDGET (page));
  if (!GTK_IS_WINDOW (toplevel))
    toplevel = NULL;

  GtkWidget *err_dialog
      = gtk_message_dialog_new (toplevel ? GTK_WINDOW (toplevel) : NULL,
                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                GTK_MESSAGE_ERROR,
                                GTK_BUTTONS_OK,
                                _("Error, cannot add Tag.\n%s"), message);

  g_signal_connect (G_OBJECT (err_dialog),
                    "response",
                    G_CALLBACK (gtk_widget_destroy),
                    NULL);

  gtk_window_set_resizable (GTK_WINDOW (err_dialog), FALSE);
  gtk_widget_show (err_dialog);
}
