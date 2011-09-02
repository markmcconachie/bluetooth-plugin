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


void
open_main_window(XfcePanelPlugin *plugin)
{
	GtkWidget *window;
	GtkToggleButton *button;

	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(window, _("Bluetooth Plugin"));

	button = gtk_toggle_button_new_with_label(_("Enable Bluetooth"));


	gtk_container_add (GTK_CONTAINER (window), button);
	gtk_toggle_button_set_active(button, bluetoothActivated());
	gtk_widget_show (button);

	gtk_container_set_border_width (GTK_CONTAINER (window), 100);
	gtk_widget_show (window);

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
