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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>


static void
addVisDev(GtkWidget *list, const gchar *str1, const gchar *str2)
{
	GtkListStore *store;
	GtkTreeIter iter;

	store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter,
                      DEV_NAME, str1,
        				DEV_ADDR, str2,
                      -1);
}

void
scanDev(GtkWidget *widget, gpointer list)
{
	
	//	addVisDev(list, name, addr);
	
}

static GtkWidget *
initMoView (void)
{
	GtkCellRenderer	*renderer;
	GtkTreeModel	*model;
	GtkWidget		*view;
	GtkListStore  	*store;
	GtkTreeIter    	iter;

	view = gtk_tree_view_new ();
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "Device Name", renderer, "text", DEV_NAME, NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "Device Address", renderer, "text", DEV_ADDR, NULL);

	store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);
	model = GTK_TREE_MODEL (store);

	gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);
	g_object_unref (model);

	return view;
}


void
open_main_window(XfcePanelPlugin *plugin)
{
	GtkWidget *window;
	GtkWidget *list;
	GtkWidget *scrolled_window;
	GtkWidget *table;
	GtkWidget *button;
	GtkWidget *bbox;


	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	g_signal_connect (window, "delete_event", gtk_main_quit, NULL); /* dirty */
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_default_size (GTK_WINDOW (window), 300, 300);
	

	list = initMoView ();
	table = gtk_table_new (2, 4, TRUE);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), list);
	gtk_table_attach_defaults (GTK_TABLE (table), scrolled_window, 1, 4, 0, 2);

	bbox = gtk_vbutton_box_new();
	gtk_button_box_set_layout(GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_START);

	

	button = gtk_button_new_with_label (("Scan"));
	g_signal_connect (button, "clicked", G_CALLBACK (scanDev), (gpointer)(list));
	

	gtk_container_add (GTK_CONTAINER (bbox), button);

	gtk_table_attach_defaults (GTK_TABLE (table), bbox, 0, 1, 0, 1);

		

	gtk_container_add (GTK_CONTAINER (window), table);

	gtk_widget_show_all (window);
}

gboolean
bluetoothActivated()
{
	return TRUE;
}

void
disableBluetooth()
{
	//system("/usr/sbin/hciconfig hci0 down &> /dev/null");
}

void
scanVisibleDev()
{

}
