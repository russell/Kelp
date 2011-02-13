/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * kelp-prefs.c
 * Copyright (C) Russell Sim 2011 <russell.sim@gmail.com>
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
#include <stdio.h>

#include <glib.h>
#include <gtk/gtk.h>

#include "kelp-app.h"
#include "kelp-computer-import.h"

#define KELP_PREFERENCES_FILE_LOCATION "kelp"

// The GtkListStore columns
enum
  {
    COL_COMPUTER_T = 0,
    NUM_COLS
};


gchar *
kelp_get_user_config_dir (void)
{
  gchar *config_dir = NULL;
  config_dir = g_build_filename (g_get_user_config_dir (),
				 "kelp",
				 NULL);
  return config_dir;
}


static gchar *
get_preferences_filename (void)
{
  gchar *config_dir;
  gchar *filename = NULL;

  config_dir = kelp_get_user_config_dir ();

  if (config_dir != NULL)
    {
      filename = g_build_filename (config_dir,
				   KELP_PREFERENCES_FILE_LOCATION,
				   NULL);
      g_free (config_dir);
    }

  return filename;
}


static GKeyFile *
get_kelp_preferences_file (void)
{
  static GKeyFile *state_file = NULL;

  if (state_file == NULL)
    {
      gchar *filename;
      GError *err = NULL;

      state_file = g_key_file_new ();

      filename = get_preferences_filename ();

      if (!g_key_file_load_from_file (state_file,
				      filename,
				      G_KEY_FILE_NONE,
				      &err))
	{
	  if (err->domain != G_FILE_ERROR ||
	      err->code != G_FILE_ERROR_NOENT)
	    {
	      g_warning ("Could not load kelp state file: %s\n",
			 err->message);
	    }

	  g_error_free (err);
	}

      g_free (filename);
    }

  return state_file;
}


void
save_kelp_preferences_file (GKeyFile *file)
{
	gchar *data;
	gsize size;
	gchar *filename;
	GError *p_error = NULL;
	gboolean success;

	data = g_key_file_to_data (file, &size, &p_error);

	if (!data)
		{
			/* Error, exit program. */
			g_warning ("Impossible to convert data to string : %s\n",
					   p_error->message);
			g_error_free (p_error);
			g_key_file_free (file);
			return;
		}
	g_mkdir_with_parents (kelp_get_user_config_dir(), 0755);
	filename = get_preferences_filename ();
	success = g_file_set_contents (filename, data, size,  &p_error);
	g_free (data);

	if (!success)
		{
			g_warning ("Impossible to write config file : %s\n",
					   p_error->message);
			g_error_free (p_error);
			g_key_file_free (file);
			return;
		}

	g_key_file_free (file);
}


void
on_preferences_menuitem_activate (GtkMenuItem *menuitem, Kelp *kelp)
{
  gtk_widget_show (kelp->prefs);
}


void
on_close_button_clicked (GtkButton *button, Kelp *kelp)
{
  gtk_widget_hide (kelp->prefs);
}


void
on_kelp_preferences_show (GtkMenuItem *menuitem, Kelp *kelp)
{
	GtkListStore *computer_types;
	GtkCellRenderer *cell;
	GtkTreeIter   iter;
	int i, length;

	length = sizeof(g_backends) / sizeof(backend_table_t);
	computer_types = gtk_list_store_new (NUM_COLS,
										 G_TYPE_STRING);
	for (i=0; i < length; i++)
		{
			gtk_list_store_append (computer_types, &iter);

			gtk_list_store_set (computer_types, &iter,
								COL_COMPUTER_T, g_backends[i].name,
								-1);

		}
	gtk_combo_box_set_model (kelp->computer_type, (GtkTreeModel*)computer_types);
	g_object_unref (G_OBJECT(computer_types));

	cell = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start (GTK_CELL_LAYOUT(kelp->computer_type), cell, FALSE );
	gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT(kelp->computer_type), cell,
									"text", COL_COMPUTER_T,
									NULL );
}


void
on_prefs_comptype_changed (GtkComboBox *combobox, Kelp *kelp)
{
	GtkTreeIter iter;

	if (gtk_combo_box_get_active_iter (combobox, &iter))
		{
			GKeyFile *file;
			GtkTreeModel *model;
			gchar *computer_type;

			file = get_kelp_preferences_file();
			model = gtk_combo_box_get_model (combobox);
			gtk_tree_model_get (model, &iter,
								COL_COMPUTER_T, &computer_type,
								-1);
			g_key_file_set_string (file, "Computer", "Type", computer_type);
			save_kelp_preferences_file (file);
		}
}


void
on_prefs_port_file_set (GtkFileChooserButton *button, Kelp *kelp)
{
	GFile *portname = NULL;
	GKeyFile *file;

	portname = gtk_file_chooser_get_file (GTK_FILE_CHOOSER(button));
	file = get_kelp_preferences_file();
	g_key_file_set_string (file, "Computer", "Port", g_file_get_path (portname));
	save_kelp_preferences_file (file);
	g_object_unref(portname);
}
