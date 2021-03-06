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

#ifndef __BLUETOOTH_WINDOW_H__
#define __BLUETOOTH_WINDOW_H__

G_BEGIN_DECLS

enum
{
  DEV_NAME = 0,
  DEV_ADDR,
  NUM_COLS
};

void
scanVisDev(GtkWidget *widget, gpointer list);

void
getOwnDevName(char* localName);

static void
addVisDev(GtkWidget *list, const gchar *str1, const gchar *str2);

void
open_main_window(XfcePanelPlugin *plugin);


G_END_DECLS

#endif /* !__BLUETOOTH_WINDOW_H__ */
