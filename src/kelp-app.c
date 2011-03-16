/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * kelp-app.c
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

#include "kelp-app.h"

Kelp *GLOBALS;


static GHashTable *dives = NULL;


void
kelp_add_dive(Dive *dive)
{
        if (dives == NULL)
                dives = g_hash_table_new(g_str_hash,g_str_equal);

        g_hash_table_insert(dives, dive->fingerprint, dive);
}

void
kelp_dive_iter_init(GHashTableIter *iter)
{
        if (dives == NULL)
                dives = g_hash_table_new(g_str_hash,g_str_equal);

        g_hash_table_iter_init(iter, dives);
}

gboolean
kelp_dive_iter_next(GHashTableIter *iter, gpointer *key, gpointer *value)
{
        return g_hash_table_iter_next(iter, key, value);
}

Dive*
kelp_lookup_dive(const gchar *fingerprint)
{
        if (dives == NULL)
                dives = g_hash_table_new(g_str_hash,g_str_equal);

        return g_hash_table_lookup(dives, fingerprint);
}


GList*
kelp_get_dives(void)
{
        return g_hash_table_get_values(dives);
}
