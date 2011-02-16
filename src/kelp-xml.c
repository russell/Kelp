/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * kelp-xml.c
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

#include "kelp-xml.h"

static void
start_element_handler (GMarkupParseContext *context,
		       const gchar *element_name,
		       const gchar **attribute_names,
		       const gchar **attribute_values,
		       gpointer user_data,
		       GError **error)
{
        g_debug("** start element: %s\n", element_name);

}

static void
text_handler(GMarkupParseContext *context,
             const gchar *text,
             gsize text_len,
             gpointer user_data,
             GError **error)
{
        g_debug("** start element: %s\n", text);
}

static GMarkupParser kelp_parser = {
        start_element_handler,
        NULL,	//end_element_handler,
        text_handler, //text_handler,
        NULL,
        NULL  //cleanup
};


gint
kelp_load_xml(gchar *filename)
{
        gint retval;
        gchar *buffer;
        gsize length;
        GError *error = NULL;
        GMarkupParseContext *context;
        gboolean rc;

        retval = XML_OK;
        if (!g_file_get_contents (filename, &buffer, &length, &error))
                {
                        g_message ("%s", error->message);
                        retval = XML_LOAD_ERROR;
                        g_error_free (error);
                }
        else
                {
                        /* then process the buffer */
                        context = g_markup_parse_context_new (&kelp_parser,
                                                              0, NULL, NULL);

                        error = NULL;
                        rc = g_markup_parse_context_parse (context, buffer, length, &error);

                        if( error )
                                g_print("failed: %s\n", error->message);

                        if( rc == FALSE )
                                {
                                        error = NULL;
                                        g_markup_parse_context_end_parse(context, &error);

                                        if( error )
                                                g_print("failed: %s\n", error->message);
                                }

                        g_markup_parse_context_free (context);
                        g_free (buffer);

                }

	return retval;
}


void
kelp_save_xml(gchar *filename)
{
}
