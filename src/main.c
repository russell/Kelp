/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.c
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <config.h>

#include <gtk/gtk.h>



/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif



#include "callbacks.h"
#include "kelp-app.h"

/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/kelp/ui/kelp.ui" */
#define UI_FILE "src/kelp.ui"



/* help menu callback prototypes */
void on_about_menuitem_activate (GtkMenuItem *menuitem, Kelp *kelp);


/*
Called when the user clicks the 'About' menu. We use gtk_show_about_dialog()
which is a convenience function to show a GtkAboutDialog. This dialog will
NOT be modal but will be on top of the main application window.
*/

void on_about_menuitem_activate (GtkMenuItem *menuitem, Kelp *kelp)
{
        static const gchar * const authors[] = {
		"Russell Sim <russell.sim@gmail.com>",
		NULL
	};

	static const gchar copyright[] = \
		"Copyright \xc2\xa9 2011 Russell Sim";

	static const gchar comments[] = "";
	gtk_show_about_dialog (GTK_WINDOW (kelp->window),
			       "authors", authors,
			       "comments", comments,
			       "copyright", copyright,
			       "version", "0.1",
			       "website", "http://russellsim.org",
			       "program-name", "Kelp",
			       "logo-icon-name", GTK_STOCK_EDIT,
			       NULL);
}



GtkWidget*
create_window (Kelp *kelp)
{
	GtkWidget *window;
	GtkBuilder *builder;
	GError* error = NULL;

	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, UI_FILE, &error))
	{
		g_warning ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	/* This is important */
	gtk_builder_connect_signals (builder, kelp);
	window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
	kelp->prefs = GTK_WIDGET (gtk_builder_get_object (builder, "kelp_preferences"));
	kelp->computer_type = GTK_COMBO_BOX (gtk_builder_get_object (builder, "prefs_comptype"));

	kelp->window = window;

	g_object_unref (builder);

	return window;
}


int
main (int argc, char *argv[])
{
 	GtkWidget *window;
	Kelp *kelp;

	/* allocate the memory needed by our TutorialTextEditor struct */
	kelp = g_slice_new (Kelp);

#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	gtk_set_locale ();
	gtk_init (&argc, &argv);

	window = create_window (kelp);
	gtk_widget_show (window);
	gtk_main ();

	/* free memory we allocated for TutorialTextEditor struct */
	g_slice_free (Kelp, kelp);

	return 0;
}
