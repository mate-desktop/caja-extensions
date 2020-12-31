/*
 *  caja-image-rotator.c
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
 #include <config.h> /* for GETTEXT_PACKAGE */
#endif

#include "caja-image-rotator.h"

#include <string.h>

#include <glib/gi18n.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include <libcaja-extension/caja-file-info.h>

struct _CajaImageRotator {
	GObject parent_instance;

	GList *files;

	gchar *suffix;

	int images_rotated;
	int images_total;
	gboolean cancelled;

	gchar *angle;

	GtkDialog *rotate_dialog;
	GtkRadioButton *default_angle_radiobutton;
	GtkComboBox *angle_combobox;
	GtkRadioButton *custom_angle_radiobutton;
	GtkSpinButton *angle_spinbutton;
	GtkRadioButton *append_radiobutton;
	GtkEntry *name_entry;
	GtkRadioButton *inplace_radiobutton;

	GtkWidget *progress_dialog;
	GtkWidget *progress_bar;
	GtkWidget *progress_label;
};

G_DEFINE_TYPE (CajaImageRotator, caja_image_rotator, G_TYPE_OBJECT)

enum {
	PROP_FILES = 1,
};

typedef enum {
	/* Place Signal Types Here */
	SIGNAL_TYPE_EXAMPLE,
	LAST_SIGNAL
} CajaImageRotatorSignalType;

static void
caja_image_rotator_finalize (GObject *object)
{
	CajaImageRotator *rotator = CAJA_IMAGE_ROTATOR (object);

	g_free (rotator->suffix);

	if (rotator->angle)
		g_free (rotator->angle);

	G_OBJECT_CLASS(caja_image_rotator_parent_class)->finalize(object);
}

static void
caja_image_rotator_set_property (GObject      *object,
                                 guint         property_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
	CajaImageRotator *rotator = CAJA_IMAGE_ROTATOR (object);

	switch (property_id) {
		case PROP_FILES:
			rotator->files = g_value_get_pointer (value);
			rotator->images_total = g_list_length (rotator->files);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(object,property_id,pspec);
	}
}

static void
caja_image_rotator_get_property (GObject      *object,
                                 guint         property_id,
                                 GValue       *value,
                                 GParamSpec   *pspec)
{
	CajaImageRotator *rotator = CAJA_IMAGE_ROTATOR (object);

	switch (property_id) {
		case PROP_FILES:
			g_value_set_pointer (value, rotator->files);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	}
}

static void
caja_image_rotator_class_init(CajaImageRotatorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	GParamSpec *files_param_spec;

	object_class->finalize = caja_image_rotator_finalize;
	object_class->set_property = caja_image_rotator_set_property;
	object_class->get_property = caja_image_rotator_get_property;

	files_param_spec = g_param_spec_pointer ("files",
	"Files",
	"Set selected files",
	G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

	g_object_class_install_property (object_class,
	PROP_FILES,
	files_param_spec);
}

static void run_op (CajaImageRotator *rotator);

static GFile *
caja_image_rotator_transform_filename (CajaImageRotator *rotator, GFile *orig_file)
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
	                                rotator->suffix == NULL ? ".tmp" : rotator->suffix,
	                                extension == NULL ? "" : extension);
	g_free (basename);
	g_free (extension);

	new_file = g_file_get_child (parent_file, new_basename);

	g_object_unref (parent_file);
	g_free (new_basename);

	return new_file;
}

static void
op_finished (GPid      pid,
             gint      status,
             gpointer  data)
{
	CajaImageRotator *rotator = CAJA_IMAGE_ROTATOR (data);
	gboolean          retry = TRUE;
	CajaFileInfo     *file = CAJA_FILE_INFO (rotator->files->data);

	if (status != 0) {
		/* rotating failed */
		GtkBuilder *builder;
		GtkWidget  *msg_dialog;
		GObject    *dialog_text;
		int         response_id;
		char       *msg;
		char       *name;

		name  = caja_file_info_get_name (file);

		builder = gtk_builder_new_from_resource ("/org/mate/caja/extensions/imageconverter/error-dialog.ui");
		msg_dialog = GTK_WIDGET (gtk_builder_get_object (builder, "error_dialog"));
		dialog_text = gtk_builder_get_object (builder, "error_text");
		msg = g_strdup_printf ("'%s' cannot be rotated. Check whether you have permission to write to this folder.", name);
		gtk_label_set_text (GTK_LABEL (dialog_text), msg);
		g_free (msg);
		g_object_unref (builder);

		response_id = gtk_dialog_run (GTK_DIALOG (msg_dialog));
		gtk_widget_destroy (msg_dialog);
		if (response_id == 0) {
			retry = TRUE;
		} else if (response_id == GTK_RESPONSE_CANCEL) {
			rotator->cancelled = TRUE;
		} else if (response_id == 1) {
			retry = FALSE;
		}

	} else if (rotator->suffix == NULL) {
		/* rotate image in place */
		GFile *orig_location = caja_file_info_get_location (file);
		GFile *new_location = caja_image_rotator_transform_filename (rotator, orig_location);
		g_file_move (new_location, orig_location, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, NULL);
		g_object_unref (orig_location);
		g_object_unref (new_location);
	}

	if (status == 0 || !retry) {
		/* image has been successfully rotated (or skipped) */
		rotator->images_rotated++;
		rotator->files = rotator->files->next;
	}

	if (!rotator->cancelled && rotator->files != NULL) {
		/* process next image */
		run_op (rotator);
	} else {
		/* cancel/terminate operation */
		gtk_widget_destroy (rotator->progress_dialog);
	}
}

static void
run_op (CajaImageRotator *rotator)
{
	g_return_if_fail (rotator->files != NULL);

	CajaFileInfo *file = CAJA_FILE_INFO (rotator->files->data);

	GFile *orig_location = caja_file_info_get_location (file);
	char *filename = g_file_get_path (orig_location);
	GFile *new_location = caja_image_rotator_transform_filename (rotator, orig_location);
	char *new_filename = g_file_get_path (new_location);
	g_object_unref (orig_location);
	g_object_unref (new_location);

	/* FIXME: check whether new_uri already exists and provide "Replace _All", "_Skip", and "_Replace" options */

	gchar *argv[8];
	argv[0] = "/usr/bin/convert";
	argv[1] = filename;
	argv[2] = "-rotate";
	argv[3] = rotator->angle;
	argv[4] = "-orient";
	argv[5] = "TopLeft";
	argv[6] = new_filename;
	argv[7] = NULL;

	pid_t pid;

	if (!g_spawn_async (NULL, argv, NULL, G_SPAWN_DO_NOT_REAP_CHILD, NULL, NULL, &pid, NULL)) {
		// FIXME: error handling
		return;
	}

	g_free (filename);
	g_free (new_filename);

	g_child_watch_add (pid, op_finished, rotator);

	char *tmp;

	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (rotator->progress_bar), (double) (rotator->images_rotated + 1) / rotator->images_total);
	tmp = g_strdup_printf (_("Rotating image: %d of %d"), rotator->images_rotated + 1, rotator->images_total);
	gtk_progress_bar_set_text (GTK_PROGRESS_BAR (rotator->progress_bar), tmp);
	g_free (tmp);

	char *name = caja_file_info_get_name (file);
	tmp = g_strdup_printf (_("<i>Rotating \"%s\"</i>"), name);
	g_free (name);
	gtk_label_set_markup (GTK_LABEL (rotator->progress_label), tmp);
	g_free (tmp);

}

static void
on_caja_image_rotator_response (GtkDialog        *dialog,
                                gint              response_id,
                                CajaImageRotator *rotator)
{
	if (response_id == GTK_RESPONSE_OK) {
		if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (rotator->append_radiobutton))) {
			if (strlen (gtk_entry_get_text (rotator->name_entry)) == 0) {
				GtkWidget *msg_dialog = gtk_message_dialog_new (GTK_WINDOW (dialog),
					GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR,
					GTK_BUTTONS_OK, _("Please enter a valid filename suffix!"));
				gtk_dialog_run (GTK_DIALOG (msg_dialog));
				gtk_widget_destroy (msg_dialog);
				return;
			}
			rotator->suffix = g_strdup (gtk_entry_get_text (rotator->name_entry));
		}
		if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (rotator->default_angle_radiobutton))) {
			switch (gtk_combo_box_get_active (GTK_COMBO_BOX (rotator->angle_combobox))) {
			case 0:
				rotator->angle = g_strdup_printf ("90");
				break;
			case 1:
				rotator->angle = g_strdup_printf ("-90");
				break;
			case 2:
				rotator->angle = g_strdup_printf ("180");
				break;
			default:
				g_assert_not_reached ();
			}
		} else if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (rotator->custom_angle_radiobutton))) {
			rotator->angle = g_strdup_printf ("%d",
			                                  gtk_spin_button_get_value_as_int (rotator->angle_spinbutton));
		} else {
			g_assert_not_reached ();
		}

		run_op (rotator);
	}

	gtk_widget_destroy (GTK_WIDGET (dialog));
}

static void
caja_image_rotator_init (CajaImageRotator *rotator)
{
	GtkBuilder *builder;

	builder = gtk_builder_new_from_resource ("/org/mate/caja/extensions/imageconverter/caja-image-rotate.ui");
	gtk_builder_set_translation_domain (builder, GETTEXT_PACKAGE);

	/* Grab some widgets */
	rotator->rotate_dialog = GTK_DIALOG (gtk_builder_get_object (builder, "rotate_dialog"));
	rotator->default_angle_radiobutton =
		GTK_RADIO_BUTTON (gtk_builder_get_object (builder, "default_angle_radiobutton"));
	rotator->angle_combobox = GTK_COMBO_BOX (gtk_builder_get_object (builder, "angle_combobox"));
	rotator->custom_angle_radiobutton =
		GTK_RADIO_BUTTON (gtk_builder_get_object (builder, "custom_angle_radiobutton"));
	rotator->angle_spinbutton =
		GTK_SPIN_BUTTON (gtk_builder_get_object (builder, "angle_spinbutton"));
	rotator->append_radiobutton =
		GTK_RADIO_BUTTON (gtk_builder_get_object (builder, "append_radiobutton"));
	rotator->name_entry = GTK_ENTRY (gtk_builder_get_object (builder, "name_entry"));
	rotator->inplace_radiobutton =
		GTK_RADIO_BUTTON (gtk_builder_get_object (builder, "inplace_radiobutton"));

	/* Set default value for combobox */
	gtk_combo_box_set_active  (rotator->angle_combobox, 0); /* 90° clockwise */

	/* Connect the signal */
	g_signal_connect (rotator->rotate_dialog, "response",
	                  G_CALLBACK (on_caja_image_rotator_response),
			  rotator);

	g_object_unref (builder);
}

CajaImageRotator *
caja_image_rotator_new (GList *files)
{
	return g_object_new (CAJA_TYPE_IMAGE_ROTATOR, "files", files, NULL);
}

void
caja_image_rotator_show_dialog (CajaImageRotator *rotator)
{
	gtk_widget_show (GTK_WIDGET (rotator->rotate_dialog));
}
