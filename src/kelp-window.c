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
                        GHashTableIter iter;
                        GtkTreeIter   toplevel, titer;
                        gpointer key, value;
                        GtkTreeViewColumn *col;
                        GtkCellRenderer *renderer;
                        gchar *datetime;

                        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
                        kelp_load_xml (filename);
                        g_free (filename);

                        // iter over all the dives an populate the list
                        list = gtk_list_store_new(NUM_COLS,
                                                  G_TYPE_STRING);

                        kelp_dive_iter_init (&iter);
                        while (kelp_dive_iter_next (&iter, &key, &value))
                                {
                                        Dive *dive = (Dive *) value;
                                        datetime = g_malloc0(sizeof(char[20]));
					gtk_list_store_append (list, &titer);

                                        sprintf(datetime, "%02d:%02d\n",
                                                dive->datetime->tm_hour,
                                                dive->datetime->tm_min);
					gtk_list_store_set (list, &titer,
                                                            COL_DATETIME_T,
                                                            datetime,
                                                            -1);
                                }

                        col = gtk_tree_view_column_new();

                        gtk_tree_view_column_set_title(col, "Dive");

                        /* pack tree view column into tree view */
                        gtk_tree_view_append_column(GTK_TREE_VIEW(kelp->dive_list), col);

                        renderer = gtk_cell_renderer_text_new();

                        /* pack cell renderer into tree view column */
                        gtk_tree_view_column_pack_start(col, renderer, TRUE);

                        /* set 'text' property of the cell renderer */
                        gtk_tree_view_column_add_attribute(col, renderer, "text", COL_DATETIME_T);


                        gtk_tree_view_set_model(GTK_TREE_VIEW(kelp->dive_list), GTK_TREE_MODEL(list));

                        g_object_unref(G_OBJECT(list)); /* destroy model automatically with view */

                        gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(kelp->dive_list)),
                                                    GTK_SELECTION_SINGLE);


                }
        gtk_widget_destroy (dialog);
}
