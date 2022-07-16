#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib/gi18n-lib.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gio/gio.h>
#include <libcaja-extension/caja-extension-types.h>
#include <libcaja-extension/caja-menu-provider.h>

#define GKSU_TYPE_CONTEXT_MENU (gksu_context_menu_get_type ())

typedef struct {
    GObject parent;
} GksuContextMenu;

typedef struct {
    GObjectClass parent_class;
} GksuContextMenuClass;

static GType gksucm_type = 0;
static GObjectClass *parent_class = NULL;

static void
gksu_context_menu_init (GksuContextMenu *self);
static void
gksu_context_menu_class_init (GksuContextMenuClass *class);
static void
menu_provider_iface_init (CajaMenuProviderIface *iface);

static GList*
gksu_context_menu_get_file_items (CajaMenuProvider *provider,
				  GtkWidget *window,
				  GList *files);
static void
gksu_context_menu_activate (CajaMenuItem *item,
			    CajaFileInfo *file);

static GType
gksu_context_menu_get_type (void)
{
    return gksucm_type;
}

static void
gksu_context_menu_register_type (GTypeModule *module)
{
    static const GTypeInfo info = {
	sizeof (GksuContextMenuClass),
	(GBaseInitFunc) NULL,
	(GBaseFinalizeFunc) NULL,
	(GClassInitFunc) gksu_context_menu_class_init,
	NULL,
	NULL,
	sizeof (GksuContextMenu),
	0,
	(GInstanceInitFunc) gksu_context_menu_init
    };
    static const GInterfaceInfo menu_provider_iface_info = {
	(GInterfaceInitFunc)menu_provider_iface_init,
	NULL,
	NULL
    };

    gksucm_type = g_type_module_register_type (module,
					    G_TYPE_OBJECT,
					    "GksuContextMenu",
					    &info, 0);
    g_type_module_add_interface (module,
				 gksucm_type,
				 CAJA_TYPE_MENU_PROVIDER,
				 &menu_provider_iface_info);
}

static void
gksu_context_menu_class_init (GksuContextMenuClass *class)
{
    parent_class = g_type_class_peek_parent (class);
}

static void menu_provider_iface_init (CajaMenuProviderIface *iface)
{
    iface->get_file_items = gksu_context_menu_get_file_items;
}

static void
gksu_context_menu_init (GksuContextMenu *self)
{
  g_message ("Initializing gksu extension...");
}

static GList *
gksu_context_menu_get_file_items (CajaMenuProvider *provider,
				  GtkWidget *window,
				  GList *files)
{
    GList *items = NULL;
    CajaFileInfo *file;
    CajaMenuItem *item;

    /* if we're already root, really or effectively, do not add
       the menu item */
    if (geteuid () == 0)
      return NULL;

    /* only add a menu item if a single file is selected ... */
    if (files == NULL || files->next != NULL)
      return NULL;

    file = files->data;

    /* ... and if it is not a caja special item */
    {
      gchar *uri_scheme = NULL;

      uri_scheme = caja_file_info_get_uri_scheme (file);
      if (!strncmp (uri_scheme, "x-caja-desktop", 18))
	{
	  g_free (uri_scheme);
	  return NULL;
	}
      g_free (uri_scheme);
    }

    /* create the context menu item */
    item = caja_menu_item_new ("Gksu::open_as_root",
				   _("Open as administrator"),
				   _("Opens the file with administrator privileges"),
				   NULL);
    g_signal_connect_object (item, "activate",
			     G_CALLBACK (gksu_context_menu_activate),
			     file, 0);
    items = g_list_prepend (items, item);

    return items;
}

static void
gksu_context_menu_activate (CajaMenuItem *item,
			    CajaFileInfo *file)
{
  gchar *exec_path;
  gchar *uri = NULL;
  gchar *mime_type = NULL;
  gchar *cmd = NULL;
  gchar *full_cmd = NULL;
  gchar *tmp = NULL;
  gboolean is_desktop = FALSE;

  uri = caja_file_info_get_uri (file);
  mime_type = caja_file_info_get_mime_type (file);

  if (!strcmp (mime_type, "application/x-desktop"))
    { /* we're handling a .desktop file */
      GKeyFile *key_file = g_key_file_new ();
      gint retval = 0;

      is_desktop = TRUE;

      gchar *file_path = g_filename_from_uri (uri, NULL, NULL);
      retval = g_key_file_load_from_file (key_file, file_path, 0, NULL);
      g_free (file_path);

      if (retval)
        cmd = g_key_file_get_string (key_file, "Desktop Entry", "Exec", NULL);
      g_key_file_free (key_file);
    }
  else
    {
      GAppInfo *app_info = g_app_info_get_default_for_type (mime_type, strncmp (uri, "file://", 7));
      if (app_info)
	{
	  cmd = g_strdup (g_app_info_get_executable (app_info));
          g_object_unref (app_info);
	}
    }

  if (cmd == NULL)
    {
      GtkWidget *dialog;

      dialog = gtk_message_dialog_new_with_markup (NULL, 0,
						   GTK_MESSAGE_ERROR,
						   GTK_BUTTONS_CLOSE,
						   _("<big><b>"
						     "Unable to determine the program to run."
						     "</b></big>\n\n"
						     "The item you selected cannot be open with "
						     "administrator powers because the correct "
						     "application cannot be determined."));
      gtk_dialog_run (GTK_DIALOG(dialog));
      gtk_widget_destroy (dialog);
      return;
    }

  /*
   * FIXME: remove any FreeDesktop substitution variable for now; we
   * need to process them!
   */
  tmp = strstr (cmd, "%");
  if (tmp)
    *tmp = '\0';

  if (is_desktop)
    full_cmd = cmd;
  else
    {
      full_cmd = g_strdup_printf ("%s '%s'", cmd, uri);
      g_free (cmd);
    }

  if ((exec_path = g_find_program_in_path ("gksu")) == NULL)
    {
       if ((exec_path = g_find_program_in_path ("beesu")) == NULL)
         {
           GtkWidget *dialog;

           dialog = gtk_message_dialog_new_with_markup (NULL, 0,
                                                        GTK_MESSAGE_ERROR,
                                                        GTK_BUTTONS_CLOSE,
                                                        _("<big><b>"
                                                          "Unable to determine the graphical wrapper for su"
                                                           "</b></big>\n\n"
                                                           "The item you selected cannot be open with "
                                                           "administrator powers because the graphical wrapper "
                                                           "for su cannot be determined, such as gtksu or beesu."));
           gtk_dialog_run (GTK_DIALOG (dialog));
           gtk_widget_destroy (dialog);
         }
    }

  if (exec_path != NULL)
    {
      GError *error = NULL;
      gchar **argv = (gchar**) g_malloc (sizeof (gchar*) * 3);

      argv[0] = exec_path;
      argv[1] = full_cmd;
      argv[2] = NULL;

      if (!g_spawn_async (NULL, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, &error))
        {
           GtkWidget *dialog;

           dialog = gtk_message_dialog_new (NULL, 0,
                                            GTK_MESSAGE_ERROR,
                                            GTK_BUTTONS_CLOSE,
                                            _("Error: %s"),
                                            error->message);
           gtk_dialog_run (GTK_DIALOG (dialog));
           gtk_widget_destroy (dialog);
           g_error_free (error);
        }
      g_strfreev (argv);
    }
  else
    {
      g_free (full_cmd);
    }

  g_free (uri);
  g_free (mime_type);
}

/* --- extension interface --- */
void
caja_module_initialize (GTypeModule *module)
{
    g_print ("Initializing caja-gksu extension\n");
    gksu_context_menu_register_type (module);
#ifdef ENABLE_NLS
    bindtextdomain (GETTEXT_PACKAGE, MATELOCALEDIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
#endif /* ENABLE_NLS */
}

void
caja_module_shutdown (void)
{
    g_print ("Shutting down caja-gksu extension\n");
}

void
caja_module_list_types (const GType **types,
			    int *num_types)
{
    static GType type_list[1];

    type_list[0] = GKSU_TYPE_CONTEXT_MENU;
    *types = type_list;
    *num_types = G_N_ELEMENTS (type_list);
}
