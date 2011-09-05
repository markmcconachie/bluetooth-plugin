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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>


#include "bluetooth.h"
#include "bluetooth-dialogs.h"
#include "bluetooth-window.h"

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
getOwnDevName(char* localName)
{

    int dev_id, sock;
    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) 
	{
        return;
    }

	hci_read_local_name(sock, 248, localName, 0);
    close( sock );
}

void
scanDev(GtkWidget *widget, gpointer list)
{
	inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, sock, len, flags;
    int i;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    dev_id = hci_get_route(NULL);
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) 
	{
        return;
    }

    len  = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    
    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) perror("hci_inquiry");

    for (i = 0; i < num_rsp; i++) 
	{
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name), name, 0) < 0)
        strcpy(name, _("unknown"));
        addVisDev(list, name, addr);
    }

    free( ii );
    close( sock );
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
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, _("Device Name"), renderer, "text", DEV_NAME, NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, _("Device Address"), renderer, "text", DEV_ADDR, NULL);

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
	GtkWidget *label;
	GtkWidget *frame;


	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	gtk_window_set_default_size (GTK_WINDOW (window), 300, 300);
	

	list = initMoView ();
	table = gtk_table_new (5, 4, TRUE);


	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), list);
	gtk_table_attach_defaults (GTK_TABLE (table), scrolled_window, 1, 4, 0, 4);

	bbox = gtk_vbutton_box_new();
	gtk_button_box_set_layout(GTK_BUTTON_BOX (bbox), GTK_BUTTONBOX_START);

	

	button = gtk_button_new_with_label (_("Scan"));
	g_signal_connect (button, "clicked",
		      G_CALLBACK (scanDev), (gpointer)(list));
	

	gtk_container_add (GTK_CONTAINER (bbox), button);

	gtk_table_attach_defaults (GTK_TABLE (table), bbox, 0, 1, 0, 1);

	
	char* ownDevName = (char *) malloc( 248 );;
	
	getOwnDevName(ownDevName);
	label = gtk_label_new(ownDevName);

	frame = gtk_frame_new(_("Own Device Name"));
	gtk_frame_set_shadow_type(GTK_FRAME (frame), GTK_SHADOW_ETCHED_OUT);

	gtk_table_attach_defaults (GTK_TABLE (table), label, 2, 4, 4, 5);
	gtk_table_attach_defaults (GTK_TABLE (table), frame, 2, 4, 4, 5);

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
