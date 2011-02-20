/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * kelp-app.h
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

#ifndef __KELP_APP_H__
#define __KELP_APP_H__

#include <gtk/gtk.h>


/* store the widgets which may need to be accessed in a typedef struct */
struct _Kelp
{
        GtkWidget *window;
	GtkWidget *prefs;
	GtkComboBox *computer_type;
	GtkFileChooserButton *computer_port;
	gchar *computer;
};
typedef struct _Kelp Kelp;


extern Kelp *GLOBALS;



typedef struct gasmix_t {
	double helium;
	double oxygen;
	double nitrogen;
} gasmix_t;

typedef struct pressure_t{
        unsigned int tank;
        double value;
} pressure_t;

typedef struct sample_t {
        double depth;
        time_t time;
	double temperature;
	unsigned int rbt;
	unsigned int heartbeat;
	unsigned int bearing;
        GList *pressures;
} sample_t;

typedef struct _Dive
{
        gchar *fingerprint;
	time_t *time;
	unsigned int number;
        unsigned int size;
        time_t date;
        GTimeVal divetime; // might change this later...
	double maxdepth;
        GList *gasmixes;
        GList *samples;
} Dive;

void kelp_add_dive(Dive *dive);

Dive* kelp_lookup_dive(const gchar *fingerprint);

#endif /* __KELP_APP_H__ */
