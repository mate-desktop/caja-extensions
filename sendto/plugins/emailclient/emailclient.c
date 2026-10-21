/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/*
 * Copyright (C) 2004 Roberto Majadas <roberto.majadas@openshine.com>
 * Copyright (C) 2012 Stefano Karapetsas <stefano@karapetsas.com>
 * Copyright (C) 2012-2021 MATE developers.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more av.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301  USA.
 *
 * Authors:  Roberto Majadas <roberto.majadas@openshine.com>
 *           Stefano Karapetsas <stefano@karapetsas.com>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n-lib.h>

#include <string.h>
#include "caja-sendto-plugin.h"
#include <gio/gio.h>

typedef enum {
	MAILER_UNKNOWN,
	MAILER_DEFAULT,
	MAILER_BALSA,
	MAILER_SYLPHEED,
	MAILER_KMAIL,
	MAILER_THUNDERBIRD,
} MailerType;

static char *mail_cmd = NULL;
static MailerType type = MAILER_UNKNOWN;

static char *
get_evo_cmd (void)
{
	char *tmp, *retval;
	tmp = g_find_program_in_path ("evolution");

	if (tmp == NULL)
		return NULL;

	retval = g_strdup_printf ("%s --component=mail %%s", tmp);
	g_free (tmp);
	return retval;
}

static gboolean
init (NstPlugin *plugin)
{
	GAppInfo *app_info = NULL;

	g_print ("Init email client plugin\n");

#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, MATELOCALEDIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
#endif /* ENABLE_NLS */

	app_info = g_app_info_get_default_for_uri_scheme ("mailto");
	if (app_info) {
		mail_cmd = g_strdup(g_app_info_get_executable (app_info));
		g_object_unref (app_info);
	}

	if (mail_cmd == NULL || *mail_cmd == '\0') {
		g_free (mail_cmd);
		mail_cmd = get_evo_cmd ();
		type = MAILER_DEFAULT;
	} else {
		char *mail_cmd_aux = mail_cmd;
		mail_cmd = g_strdup_printf ("%s %%s", mail_cmd_aux);
		g_free (mail_cmd_aux);
		/* Find what the default mailer is */
		if (strstr (mail_cmd, "balsa"))
			type = MAILER_BALSA;
		else if (strstr (mail_cmd, "thunder") || strstr (mail_cmd, "seamonkey"))
			type = MAILER_THUNDERBIRD;
		else if (strstr (mail_cmd, "sylpheed") || strstr (mail_cmd, "claws"))
			type = MAILER_SYLPHEED;
		else if (strstr (mail_cmd, "kmail"))
			type = MAILER_KMAIL;
		else if (strstr (mail_cmd, "anjal") || strstr (mail_cmd, "evolution") || strstr (mail_cmd, "geary"))
			type = MAILER_DEFAULT;
	}

	if (mail_cmd == NULL)
		return FALSE;

	return TRUE;
}

static
GtkWidget* get_contacts_widget (NstPlugin *plugin)
{
	GtkWidget *entry;

	// TODO: add an email address format check
	entry = gtk_entry_new();

	return entry;
}

static void
get_default_mailto (GtkWidget *contact_widget, GString *mailto, GList *file_list)
{
  // Works with:
  //  - Evolution
  //  - Geary
  //  - KMail
  //  - ...

	GList *l;

	g_string_append (mailto, "mailto:");

	const char *text;

	text = gtk_entry_get_text (GTK_ENTRY (contact_widget));
	if (text != NULL && *text != '\0') {
		char *text_esc = g_uri_escape_string (text, NULL, TRUE); // to handle character ?
		g_string_append_printf (mailto, "\"%s\"", text_esc);
		g_free (text_esc);
	} else
		g_string_append (mailto, "\"\"");

	g_string_append_printf (mailto, "?");
	for (l = file_list ; l; l=l->next) {
		// The filename must be full %-encoded;
		// otherwise, we'll have problems with these characters: ? % & #
		char *filename_clean = g_filename_from_uri ((char *) l->data, NULL, NULL);
		if (filename_clean != NULL)
		{
			char *file_esc = g_uri_escape_string (filename_clean, NULL, TRUE);
			g_string_append_printf (mailto, "attach=\"%s\"&", file_esc); // the " around the %s are optional here (at least for clients Evolution and Geary)
			g_free (file_esc);
		}
		g_free (filename_clean);
	}
	if (mailto->str [mailto->len - 1] == '&') g_string_truncate (mailto, mailto->len - 1); //remove last & (at least for clients Evolution and Geary)
}

static void
get_balsa_mailto (GtkWidget *contact_widget, GString *mailto, GList *file_list)
{
	GList *l;

	if (strstr (mail_cmd, " -m ") == NULL && strstr (mail_cmd, " --compose=") == NULL)
		g_string_append (mailto, " --compose=");

	const char *text;

	text = gtk_entry_get_text (GTK_ENTRY (contact_widget));
	if (text != NULL && *text != '\0') {
		char *text_quote = g_shell_quote (text);
		g_string_append_printf (mailto, "%s", text_quote);
		g_free (text_quote);
	} else
		g_string_append (mailto, "\"\"");

	for (l = file_list ; l; l=l->next) {
		g_string_append_printf (mailto, " --attach=\"%s\"", (char *)l->data);
	}
}

static void
get_thunderbird_mailto (GtkWidget *contact_widget, GString *mailto, GList *file_list)
{
	GList *l;

	g_string_append (mailto, "-compose \"");

	const char *text;

	text = gtk_entry_get_text (GTK_ENTRY (contact_widget));
	if (text != NULL && *text != '\0') {
		char *text_esc = g_uri_escape_string (text, NULL, TRUE); // escape characters ? " '
		g_string_append_printf (mailto, "to=%s,", text_esc);  // Don't use outer quotation here (='%s'), because then it wouldn't be interpreted as percent-encoded text
		g_free (text_esc);
	}

	g_string_append_printf (mailto, "attachment='");
	for (l = file_list ; l; l=l->next) {
		GString *file_esc = g_string_new ((char *) l->data);
		g_string_replace (file_esc, "\'", "%27", 0); // escape ' (this is only for a very rare problem: e.g. attach this two filenames, in this exact order: a' and abc )
		g_string_replace (file_esc, ",", "%2C", 0); // escape , (needed to handle multiple files)
		g_string_append_printf (mailto, "%s,", file_esc->str);
		g_string_free (file_esc, TRUE);
	}
	if (mailto->str [mailto->len - 1] == ',') g_string_truncate (mailto, mailto->len - 1); //remove last ,
	g_string_append (mailto, "'\"");
}

static void
get_sylpheed_mailto (GtkWidget *contact_widget, GString *mailto, GList *file_list)
{
	GList *l;

	g_string_append (mailto, "--compose ");

	const char *text;

	text = gtk_entry_get_text (GTK_ENTRY (contact_widget));
	if (text != NULL && *text != '\0') {
		char *text_esc = g_uri_escape_string (text, NULL, TRUE); // to handle character ?
		g_string_append_printf (mailto, "%s ", text_esc);
		g_free (text_esc);
	}
	// Don't insert empty text ("") as email-adress, because it's not supported by Claws.

	g_string_append_printf (mailto, "--attach");
	for (l = file_list ; l; l=l->next) {
		char *filename_clean = g_filename_from_uri ((char *) l->data, NULL, NULL); // Sylpheed doesn't understand URIs
		if (filename_clean != NULL)
		{
			char *filename_clean_esc = g_shell_quote (filename_clean);
			g_string_append_printf (mailto, " %s", filename_clean_esc);
			g_free (filename_clean_esc);
		}
		g_free (filename_clean);
	}
}

static void
get_kmail_mailto (GtkWidget *contact_widget, GString *mailto, GList *file_list)
{
	// Alternatively: KMail would also supports mailto

	GList *l;

	g_string_append (mailto, "--composer ");

	const char *text;

	text = gtk_entry_get_text (GTK_ENTRY (contact_widget));
	if (text != NULL && *text != '\0') {
		char *text_quote = g_shell_quote (text);
		g_string_append_printf (mailto, "%s ", text_quote);
		g_free (text_quote);
	}

	for (l = file_list ; l; l=l->next){
		char *filename_esc = g_shell_quote ((char *) l->data);
		g_string_append_printf (mailto," --attach %s", filename_esc);
		g_free (filename_esc);
	}
}

static gboolean
send_files (NstPlugin *plugin,
	    GtkWidget *contact_widget,
	    GList *file_list)
{
	gchar *cmd;
	GString *mailto;

	mailto = g_string_new ("");
	switch (type) {
	case MAILER_BALSA:
		get_balsa_mailto (contact_widget, mailto, file_list);
		break;
	case MAILER_SYLPHEED:
		get_sylpheed_mailto (contact_widget, mailto, file_list);
		break;
	case MAILER_KMAIL:
		get_kmail_mailto (contact_widget, mailto, file_list);
		break;
	case MAILER_THUNDERBIRD:
		get_thunderbird_mailto (contact_widget, mailto, file_list);
		break;
	case MAILER_DEFAULT:
	default:
		get_default_mailto (contact_widget, mailto, file_list);
	}

	cmd = g_strdup_printf (mail_cmd, mailto->str);
	g_string_free (mailto, TRUE);

	g_message ("Mailer type: %d", type);
	g_message ("Command: %s", cmd);

	g_spawn_command_line_async (cmd, NULL);
	g_free (cmd);

	return TRUE;
}

static
gboolean destroy (NstPlugin *plugin){
	g_free (mail_cmd);
	mail_cmd = NULL;
	return TRUE;
}

static
NstPluginInfo plugin_info = {
	"emblem-mail",
	"emailclient",
	N_("Email"),
	NULL,
	CAJA_CAPS_NONE,
	init,
	get_contacts_widget,
	NULL,
	send_files,
	destroy
};

NST_INIT_PLUGIN (plugin_info)
