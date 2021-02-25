/*
 *  caja-image-resizer.c
 *
 *  Copyright (C) 2004-2008 Jürg Billeter
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  Author: Jürg Billeter <j@bitron.ch>
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n-lib.h>

#include "caja-image-resizer.h"

#include <string.h>

#include <gio/gio.h>
#include <gtk/gtk.h>

#include <libcaja-extension/caja-file-info.h>

struct _CajaImageResizer {
	GObject parent_instance;

	GList *files;

	gchar *suffix;

	int images_resized;
	int images_total;
	gboolean cancelled;

	gchar *size;

	GtkDialog *resize_dialog;
	GtkRadioButton *default_size_radiobutton;
	GtkComboBoxText *size_combobox;
	GtkRadioButton *custom_pct_radiobutton;
	GtkSpinButton *pct_spinbutton;
	GtkRadioButton *custom_size_radiobutton;
	GtkSpinButton *width_spinbutton;
	GtkSpinButton *height_spinbutton;
	GtkRadioButton *append_radiobutton;
	GtkEntry *name_entry;
	GtkRadioButton *inplace_radiobutton;

	GtkWidget *progress_dialog;
	GtkWidget *progress_bar;
	GtkWidget *progress_label;
};

G_DEFINE_TYPE (CajaImageResizer, caja_image_resizer, G_TYPE_OBJECT)

enum {
	PROP_FILES = 1,
};

typedef enum {
	/* Place Signal Types Here */
	SIGNAL_TYPE_EXAMPLE,
	LAST_SIGNAL
} CajaImageResizerSignalType;

static void
caja_image_resizer_finalize (GObject *object)
{
	CajaImageResizer *resizer = CAJA_IMAGE_RESIZER (object);

	g_free (resizer->suffix);

	if (resizer->size)
		g_free (resizer->size);

	G_OBJECT_CLASS(caja_image_resizer_parent_class)->finalize(object);
}

static void
caja_image_resizer_set_property (GObject      *object,
                                 guint         property_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
	CajaImageResizer *resizer = CAJA_IMAGE_RESIZER (object);

	switch (property_id) {
		case PROP_FILES:
			resizer->files = g_value_get_pointer (value);
			resizer->images_total = g_list_length (resizer->files);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	}
}

static void
caja_image_resizer_get_property (GObject      *object,
                                 guint         property_id,
                                 GValue       *value,
                                 GParamSpec   *pspec)
{
	CajaImageResizer *resizer = CAJA_IMAGE_RESIZER (object);

	switch (property_id) {
		case PROP_FILES:
			g_value_set_pointer (value, resizer->files);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	}
}

static void
caja_image_resizer_class_init (CajaImageResizerClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GParamSpec *files_param_spec;

	object_class->finalize = caja_image_resizer_finalize;
	object_class->set_property = caja_image_resizer_set_property;
	object_class->get_property = caja_image_resizer_get_property;

	files_param_spec = g_param_spec_pointer ("files",
	"Files",
	"Set selected files",
	G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

	g_object_class_install_property (object_class,
	PROP_FILES,
	files_param_spec);
}

static void run_op (CajaImageResizer *resizer);

static GFile *
caja_image_resizer_transform_filename (CajaImageResizer *resizer,
                                       GFile            *orig_file)
{
	GFile *parent_file, *new_file;
	char *basename, *extension, *new_basename;

	g_return_val_if_fail (G_IS_FILE (orig_file), NULL);

	parent_file = g_file_get_parent (orig_file);

	basename = g_strdup (g_file_get_basename (orig_file));

	extension = g_strdup (strrchr (basename, '.'));
	if (extension != NULL)
		basename[strlen (basename) - strlen (extension)] = '\0';

	new_basename = g_strdup_printf ("%s%s%s", basename,
	                                resizer->suffix == NULL ? ".tmp" : resizer->suffix,
	                                extension == NULL ? "" : extension);
	g_free (basename);
	g_free (extension);

	new_file = g_file_get_child (parent_file, new_basename);

	g_object_unref (parent_file);
	g_free (new_basename);

	return new_file;
}

static void
op_finished (GPid              pid,
             gint              status,
             gpointer          data)
{
	CajaImageResizer *resizer = CAJA_IMAGE_RESIZER (data);
	gboolean          retry = TRUE;
	CajaFileInfo     *file = CAJA_FILE_INFO (resizer->files->data);

	if (status != 0) {
		/* resizing failed */
		GtkBuilder *builder;
		GtkWidget  *msg_dialog;
		GObject    *dialog_text;
		int         response_id;
		char       *msg;
		char       *name;

		name = caja_file_info_get_name (file);

		builder = gtk_builder_new_from_resource ("/org/mate/caja/extensions/imageconverter/error-dialog.ui");
		msg_dialog = GTK_WIDGET (gtk_builder_get_object (builder, "error_dialog"));
		dialog_text = gtk_builder_get_object (builder, "error_text");
		msg = g_strdup_printf ("'%s' cannot be resized. Check whether you have permission to write to this folder.", name);
		gtk_label_set_text (GTK_LABEL (dialog_text), msg);
		g_free (msg);
		g_object_unref (builder);

		response_id = gtk_dialog_run (GTK_DIALOG (msg_dialog));
		gtk_widget_destroy (msg_dialog);
		if (response_id == 0) {
			retry = TRUE;
		} else if (response_id == GTK_RESPONSE_CANCEL) {
			resizer->cancelled = TRUE;
		} else if (response_id == 1) {
			retry = FALSE;
		}

	} else if (resizer->suffix == NULL) {
		/* resize image in place */
		GFile *orig_location = caja_file_info_get_location (file);
		GFile *new_location = caja_image_resizer_transform_filename (resizer, orig_location);
		g_file_move (new_location, orig_location, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, NULL);
		g_object_unref (orig_location);
		g_object_unref (new_location);
	}

	if (status == 0 || !retry) {
		/* image has been successfully resized (or skipped) */
		resizer->images_resized++;
		resizer->files = resizer->files->next;
	}

	if (!resizer->cancelled && resizer->files != NULL) {
		/* process next image */
		run_op (resizer);
	} else {
		/* cancel/terminate operation */
		gtk_widget_destroy (resizer->progress_dialog);
	}
}

static void
run_op (CajaImageResizer *resizer)
{
	g_return_if_fail (resizer->files != NULL);

	CajaFileInfo *file = CAJA_FILE_INFO (resizer->files->data);

	GFile *orig_location = caja_file_info_get_location (file);
	char *filename = g_file_get_path (orig_location);
	GFile *new_location = caja_image_resizer_transform_filename (resizer, orig_location);
	char *new_filename = g_file_get_path (new_location);
	g_object_unref (orig_location);
	g_object_unref (new_location);

	/* FIXME: check whether new_uri already exists and provide "Replace _All", "_Skip", and "_Replace" options */

	gchar *argv[6];
	argv[0] = "convert";
	argv[1] = filename;
	argv[2] = "-resize";
	argv[3] = resizer->size;
	argv[4] = new_filename;
	argv[5] = NULL;

	pid_t pid;

	if (filename == NULL || new_filename == NULL ||
	    !g_spawn_async (NULL, argv, NULL, G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD, NULL, NULL, &pid, NULL)) {
		// FIXME: error handling
		g_free (filename);
		g_free (new_filename);
		return;
	}

	g_free (filename);
	g_free (new_filename);

	g_child_watch_add (pid, op_finished, resizer);

	char *tmp;

	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (resizer->progress_bar),
	                               (double) (resizer->images_resized + 1) / resizer->images_total);
	tmp = g_strdup_printf (_("Resizing image: %d of %d"),
	                       resizer->images_resized + 1,
	                       resizer->images_total);
	gtk_progress_bar_set_text (GTK_PROGRESS_BAR (resizer->progress_bar), tmp);
	g_free (tmp);

	char *name = caja_file_info_get_name (file);
	tmp = g_strdup_printf (_("<i>Resizing \"%s\"</i>"), name);
	g_free (name);
	gtk_label_set_markup (GTK_LABEL (resizer->progress_label), tmp);
	g_free (tmp);

}

static void
on_caja_image_resizer_response (GtkDialog        *dialog,
                                gint              response_id,
                                CajaImageResizer *resizer)
{
	if (response_id == GTK_RESPONSE_OK) {
		if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (resizer->append_radiobutton))) {
			if (strlen (gtk_entry_get_text (resizer->name_entry)) == 0) {
				GtkWidget *msg_dialog;
				msg_dialog = gtk_message_dialog_new (GTK_WINDOW (dialog),
				                                     GTK_DIALOG_DESTROY_WITH_PARENT,
				                                     GTK_MESSAGE_ERROR,
				                                     GTK_BUTTONS_OK,
				                                     _("Please enter a valid filename suffix!"));
				gtk_dialog_run (GTK_DIALOG (msg_dialog));
				gtk_widget_destroy (msg_dialog);
				return;
			}
			resizer->suffix = g_strdup (gtk_entry_get_text (resizer->name_entry));
		}
		if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (resizer->default_size_radiobutton))) {
			resizer->size = gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT (resizer->size_combobox));
		} else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (resizer->custom_pct_radiobutton))) {
			resizer->size = g_strdup_printf ("%d%%",
			                                 gtk_spin_button_get_value_as_int (resizer->pct_spinbutton));
		} else {
			resizer->size = g_strdup_printf ("%dx%d",
			                                 gtk_spin_button_get_value_as_int (resizer->width_spinbutton),
			                                 gtk_spin_button_get_value_as_int (resizer->height_spinbutton));
		}

		run_op (resizer);
	}

	gtk_widget_destroy (GTK_WIDGET (dialog));
}

static void
caja_image_resizer_init (CajaImageResizer *resizer)
{
	GtkBuilder *builder;

	builder = gtk_builder_new_from_resource ("/org/mate/caja/extensions/imageconverter/caja-image-resize.ui");
#ifdef ENABLE_NLS
	gtk_builder_set_translation_domain (builder, GETTEXT_PACKAGE);
#endif /* ENABLE_NLS */

	/* Grab some widgets */
	resizer->resize_dialog = GTK_DIALOG (gtk_builder_get_object (builder, "resize_dialog"));
	resizer->default_size_radiobutton =
		GTK_RADIO_BUTTON (gtk_builder_get_object (builder, "default_size_radiobutton"));
	resizer->size_combobox = GTK_COMBO_BOX_TEXT (gtk_builder_get_object (builder, "comboboxtext_size"));
	resizer->custom_pct_radiobutton =
		GTK_RADIO_BUTTON (gtk_builder_get_object (builder, "custom_pct_radiobutton"));
	resizer->pct_spinbutton = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "pct_spinbutton"));
	resizer->custom_size_radiobutton =
		GTK_RADIO_BUTTON (gtk_builder_get_object (builder, "custom_size_radiobutton"));
	resizer->width_spinbutton = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "width_spinbutton"));
	resizer->height_spinbutton = GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "height_spinbutton"));
	resizer->append_radiobutton = GTK_RADIO_BUTTON (gtk_builder_get_object (builder, "append_radiobutton"));
	resizer->name_entry = GTK_ENTRY (gtk_builder_get_object (builder, "name_entry"));
	resizer->inplace_radiobutton = GTK_RADIO_BUTTON (gtk_builder_get_object (builder, "inplace_radiobutton"));

	/* Set default item in combo box */
	/* gtk_combo_box_set_active  (resizer->size_combobox, 4);  1024x768 */

	/* Connect signal */
	g_signal_connect (resizer->resize_dialog, "response",
	                  G_CALLBACK (on_caja_image_resizer_response),
	                  resizer);

	g_object_unref (builder);
}

CajaImageResizer *
caja_image_resizer_new (GList *files)
{
	return g_object_new (CAJA_TYPE_IMAGE_RESIZER, "files", files, NULL);
}

void
caja_image_resizer_show_dialog (CajaImageResizer *resizer)
{
	gtk_widget_show (GTK_WIDGET (resizer->resize_dialog));
}
