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

#include <string.h>
#include <gtk/gtk.h>

#include <libxfce4ui/libxfce4ui.h>
#include <libxfce4panel/xfce-panel-plugin.h>

#include "bluetooth.h"
#include "bluetooth-dialogs.h"
#include "bluetooth-window.h"

/* the website url */
#define PLUGIN_WEBSITE "https://github.com/EarlOfEgo/bluetooth-plugin"



static void
bluetooth_configure_response (GtkWidget    *dialog,
                           gint          response,
                           bluetoothPlugin *bluetooth)
{
  gboolean result;

  if (response == GTK_RESPONSE_HELP)
    {
      /* show help */
      result = g_spawn_command_line_async ("exo-open --launch WebBrowser " PLUGIN_WEBSITE, NULL);

      if (G_UNLIKELY (result == FALSE))
        g_warning (_("Unable to open the following url: %s"), PLUGIN_WEBSITE);
    }
  else
    {
      /* remove the dialog data from the plugin */
      g_object_set_data (G_OBJECT (bluetooth->plugin), "dialog", NULL);

      /* unlock the panel menu */
      xfce_panel_plugin_unblock_menu (bluetooth->plugin);

      /* save the plugin */
      bluetooth_save (bluetooth->plugin, bluetooth);

      /* destroy the properties dialog */
      gtk_widget_destroy (dialog);
    }
}



void
bluetooth_configure (XfcePanelPlugin *plugin,
                  bluetoothPlugin    *bluetooth)
{
  GtkWidget *dialog;

  /* block the plugin menu */
  xfce_panel_plugin_block_menu (plugin);

  /* create the dialog */
  dialog = xfce_titled_dialog_new_with_buttons (_("bluetooth Plugin"),
                                                GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (plugin))),
                                                GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_NO_SEPARATOR,
                                                GTK_STOCK_HELP, GTK_RESPONSE_HELP,
                                                GTK_STOCK_CLOSE, GTK_RESPONSE_OK,
                                                NULL);

  /* center dialog on the screen */
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);

  /* set dialog icon */
  gtk_window_set_icon_name (GTK_WINDOW (dialog), "xfce4-settings");

  /* link the dialog to the plugin, so we can destroy it when the plugin
   * is closed, but the dialog is still open */
  g_object_set_data (G_OBJECT (plugin), "dialog", dialog);

  /* connect the reponse signal to the dialog */
  g_signal_connect (G_OBJECT (dialog), "response",
                    G_CALLBACK(bluetooth_configure_response), bluetooth);

  /* show the entire dialog */
  gtk_widget_show (dialog);
}



void
bluetooth_about (XfcePanelPlugin *plugin)
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
