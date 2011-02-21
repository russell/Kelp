/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * kelp-window.c
 * Copyright (C) 2011 Russell Sim <russell.sim@gmail.com>
 *
 * kelp is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * kelp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "kelp-window.h"

// The GtkListStore columns
enum
  {
    COL_DATETIME_T = 0,
    NUM_COLS
};


void on_open_menuitem_activate (GtkMenuItem *menuitem, Kelp *kelp)
{
        GtkWidget *dialog;
        dialog = gtk_file_chooser_dialog_new ("Open File",
                                              kelp->window,
                                              GTK_FILE_CHOOSER_ACTION_OPEN,
                                              GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                              GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                              NULL);
        if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
                {
                        gchar *filename;
                        GList *dives;
                        GtkListStore *list;
                        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
                        kelp_load_xml (filename);
                        g_free (filename);
                        //dives = kelp_get_dives();
                        list = gtk_list_store_new(NUM_COLS,
                                                  G_TYPE_STRING);
                }
        gtk_widget_destroy (dialog);
}
