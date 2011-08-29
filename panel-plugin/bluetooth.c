/*  $Id$
 *
 *  Copyright (c) 2011 Stephan Hagios <stephan.hagios@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <gtk/gtk.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/xfce-panel-plugin.h>
#include <libxfce4panel/xfce-hvbox.h>

#include "bluetooth.h"
#include "bluetooth-dialogs.h"
#include "bluetooth-windows.h"

/* default settings */
#define DEFAULT_SETTING1 NULL
#define DEFAULT_SETTING2 1
#define DEFAULT_SETTING3 FALSE



/* prototypes */
static void
bluetooth_construct (XfcePanelPlugin *plugin);


/* register the plugin */
XFCE_PANEL_PLUGIN_REGISTER_EXTERNAL (bluetooth_construct);



void
bluetooth_save (XfcePanelPlugin *plugin,
             bluetoothPlugin    *bluetooth)
{
  XfceRc *rc;
  gchar  *file;

  /* get the config file location */
  file = xfce_panel_plugin_save_location (plugin, TRUE);

  if (G_UNLIKELY (file == NULL))
    {
       DBG ("Failed to open config file");
       return;
    }

  /* open the config file, read/write */
  rc = xfce_rc_simple_open (file, FALSE);
  g_free (file);

  if (G_LIKELY (rc != NULL))
    {
      /* save the settings */
      DBG(".");
      if (bluetooth->setting1)
        xfce_rc_write_entry    (rc, "setting1", bluetooth->setting1);

      xfce_rc_write_int_entry  (rc, "setting2", bluetooth->setting2);
      xfce_rc_write_bool_entry (rc, "setting3", bluetooth->setting3);

      /* close the rc file */
      xfce_rc_close (rc);
    }
}



static void
bluetooth_read (bluetoothPlugin *bluetooth)
{
  XfceRc      *rc;
  gchar       *file;
  const gchar *value;

  /* get the plugin config file location */
  file = xfce_panel_plugin_save_location (bluetooth->plugin, TRUE);

  if (G_LIKELY (file != NULL))
    {
      /* open the config file, readonly */
      rc = xfce_rc_simple_open (file, TRUE);

      /* cleanup */
      g_free (file);

      if (G_LIKELY (rc != NULL))
        {
          /* read the settings */
          value = xfce_rc_read_entry (rc, "setting1", DEFAULT_SETTING1);
          bluetooth->setting1 = g_strdup (value);

          bluetooth->setting2 = xfce_rc_read_int_entry (rc, "setting2", DEFAULT_SETTING2);
          bluetooth->setting3 = xfce_rc_read_bool_entry (rc, "setting3", DEFAULT_SETTING3);

          /* cleanup */
          xfce_rc_close (rc);

          /* leave the function, everything went well */
          return;
        }
    }

  /* something went wrong, apply default values */
  DBG ("Applying default settings");

  bluetooth->setting1 = g_strdup (DEFAULT_SETTING1);
  bluetooth->setting2 = DEFAULT_SETTING2;
  bluetooth->setting3 = DEFAULT_SETTING3;
}

void
do_something (XfcePanelPlugin *plugin)
{

	gchar* authors[] = { "Stephan Hagios <stephan.hagios@gmail.com>", NULL };
	gchar* artists[] = { "Stephan Hagios <stephan.hagios@gmail.com>", NULL };
	gchar* comments = { _("A bluetooth plugin for xfce4 panel") };
	gchar* copyright = { "Copyright (c) Stephan Hagios" };
	gchar* documenters[] = { "Stephan Hagios <stephan.hagios@gmail.com>", NULL };	
	gchar* name = "Bluetoot plugin";
	gchar* version = "0.0.1";
	gchar* website = "https://github.com/EarlOfEgo/bluetooth-plugin";
	gchar* website_label = "github";

	GtkWindow *parent = GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (plugin)));

	gtk_window_set_title(parent, "Bluetoothplugin");
	
	gtk_show_about_dialog (parent,
				"authors", authors, 
				"artists", artists,
				"comments", comments, 
				"copyright", copyright, 
				"documenters", documenters,
				"name", name, 
				"version", version, 
				"website", website, 
				"website-label", website_label,
				"license", "GNU GPL-2.0",
				 NULL);
	             


}

static bluetoothPlugin *
bluetooth_new (XfcePanelPlugin *plugin)
{
  bluetoothPlugin   *bluetooth;
  GtkOrientation  orientation;
  GtkWidget      *label;

  /* allocate memory for the plugin structure */
  bluetooth = panel_slice_new0 (bluetoothPlugin);

  /* pointer to plugin */
  bluetooth->plugin = plugin;

  /* read the user settings */
  bluetooth_read (bluetooth);

  /* get the current orientation */
  orientation = xfce_panel_plugin_get_orientation (plugin);

  /* create some panel widgets */
  bluetooth->ebox = gtk_event_box_new ();
  gtk_widget_show (bluetooth->ebox);

  bluetooth->hvbox = xfce_hvbox_new (orientation, FALSE, 2);
  gtk_widget_show (bluetooth->hvbox);
  gtk_container_add (GTK_CONTAINER (bluetooth->ebox), bluetooth->hvbox);

  /* some bluetooth widgets */
  label = gtk_label_new (_("bluetooth"));
  gtk_widget_show (label);
  gtk_box_pack_start (GTK_BOX (bluetooth->hvbox), label, FALSE, FALSE, 0);

  label = gtk_label_new (_("Plugin"));
  gtk_widget_show (label);
  gtk_box_pack_start (GTK_BOX (bluetooth->hvbox), label, FALSE, FALSE, 0);


  return bluetooth;
}



static void
bluetooth_free (XfcePanelPlugin *plugin,
             bluetoothPlugin    *bluetooth)
{
  GtkWidget *dialog;

  /* check if the dialog is still open. if so, destroy it */
  dialog = g_object_get_data (G_OBJECT (plugin), "dialog");
  if (G_UNLIKELY (dialog != NULL))
    gtk_widget_destroy (dialog);

  /* destroy the panel widgets */
  gtk_widget_destroy (bluetooth->hvbox);

  /* cleanup the settings */
  if (G_LIKELY (bluetooth->setting1 != NULL))
    g_free (bluetooth->setting1);

  /* free the plugin structure */
  panel_slice_free (bluetoothPlugin, bluetooth);
}



static void
bluetooth_orientation_changed (XfcePanelPlugin *plugin,
                            GtkOrientation   orientation,
                            bluetoothPlugin    *bluetooth)
{
  /* change the orienation of the box */
  xfce_hvbox_set_orientation (XFCE_HVBOX (bluetooth->hvbox), orientation);
}



static gboolean
bluetooth_size_changed (XfcePanelPlugin *plugin,
                     gint             size,
                     bluetoothPlugin    *bluetooth)
{
  GtkOrientation orientation;

  /* get the orientation of the plugin */
  orientation = xfce_panel_plugin_get_orientation (plugin);

  /* set the widget size */
  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    gtk_widget_set_size_request (GTK_WIDGET (plugin), -1, size);
  else
    gtk_widget_set_size_request (GTK_WIDGET (plugin), size, -1);

  /* we handled the orientation */
  return TRUE;
}


static void
bluetooth_construct (XfcePanelPlugin *plugin)
{
  bluetoothPlugin *bluetooth;

  /* setup transation domain */
  xfce_textdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR, "UTF-8");

  /* create the plugin */
  bluetooth = bluetooth_new (plugin);

  /* add the ebox to the panel */
  gtk_container_add (GTK_CONTAINER (plugin), bluetooth->ebox);

  /* show the panel's right-click menu on this ebox */
  xfce_panel_plugin_add_action_widget (plugin, bluetooth->ebox);

  /* connect plugin signals */
  g_signal_connect (G_OBJECT (plugin), "free-data",
                    G_CALLBACK (bluetooth_free), bluetooth);

  g_signal_connect (G_OBJECT (plugin), "save",
                    G_CALLBACK (bluetooth_save), bluetooth);

  g_signal_connect (G_OBJECT (plugin), "size-changed",
                    G_CALLBACK (bluetooth_size_changed), bluetooth);

  g_signal_connect (G_OBJECT (plugin), "orientation-changed",
                    G_CALLBACK (bluetooth_orientation_changed), bluetooth);

	/*Clicking on the plugin*/
  g_signal_connect (G_OBJECT (plugin), "button_press_event",
					G_CALLBACK (open_main_window), bluetooth);

  /* show the configure menu item and connect signal */
  xfce_panel_plugin_menu_show_configure (plugin);
  g_signal_connect (G_OBJECT (plugin), "configure-plugin",
                    G_CALLBACK (bluetooth_configure), bluetooth);

  /* show the about menu item and connect signal */
  xfce_panel_plugin_menu_show_about (plugin);
  g_signal_connect (G_OBJECT (plugin), "about",
                    G_CALLBACK (bluetooth_about), NULL);

	
}
