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

#define STACKSIZE 10

enum
        {
                TAG_DIVES,
                TAG_DIVE,
                TAG_GASMIX,
                TAG_SAMPLE,
                TAG_EVENT,
                TAG_PRESSURE,
                TAG_UNKNOWN
        };


typedef struct
{
        gint size;
        gint items[STACKSIZE];
} TagStack;


static void
push_tag(TagStack *ps, int x)
{
        if (ps->size == STACKSIZE)
                {
                        g_error("XML tag stack overflow\n");
                        abort();
                }
        else

                ps->items[ps->size++] = x;

}

static int
get_tag(TagStack *ps)
{
        return ps->items[ps->size];
}

static void
pop_tag(TagStack *ps)
{
        if (ps->size == 0)
                {
                        g_error("XML tag stack underflow\n");
                        abort();
                }
        else
                g_debug("size is, %i\n", ps->size);
                --ps->size;
}


typedef struct _ParseContext ParseContext;
struct _ParseContext
{
        TagStack tags;
};

static void
start_element_handler (GMarkupParseContext *context,
		       const gchar *element_name,
		       const gchar **attribute_names,
		       const gchar **attribute_values,
		       gpointer user_data,
		       GError **error)
{
        gint i;
        ParseContext *ctx = user_data;
        g_debug("** start element: %s\n", element_name);
        if (!strcmp(element_name, "dives"))
                push_tag(ctx->tags, TAG_DIVES);
        else if (!strcmp(element_name, "dive"))
                push_tag(ctx->tags, TAG_DIVE);
        else if (!strcmp(element_name, "sample"))
                push_tag(ctx->tags, TAG_SAMPLE);
        else if (!strcmp(element_name, "pressure"))
                push_tag(ctx->tags, TAG_PRESSURE);
        else
                {
                        push_tag(ctx->tags, TAG_UNKNOWN);
                        g_debug("Unown tag %s\n", element_name);
                }
        /* Find the attributes for the current tag */
        switch (get_tag(ctx->tags))
                {
                case TAG_DIVES:
                        return;
                case TAG_DIVE:
                        for (i = 0; attribute_names[i] != NULL; i++)
                                {
                                        if (!strcmp (attribute_names[i], "datetime"))
                                                {
                                                        g_debug("Ignoring time, too complex for now %s\n",
                                                                attribute_values[i]);
                                                }
                                        else if (!strcmp (attribute_names[i], "number"))
                                                {
                                                        double d = strtof(attribute_values[i], NULL);
                                                }
                                        else if (!strcmp (attribute_names[i], "size"))
                                                {
                                                        double d = strtof(attribute_values[i], NULL);
                                                }
                                        else if (!strcmp (attribute_names[i], "fingerprint"))
                                                {
                                                        double d = strtof(attribute_values[i], NULL);
                                                }
                                        else if (!strcmp (attribute_names[i], "maxdepth"))
                                                {
                                                        double d = strtof(attribute_values[i], NULL);
                                                }
                                        else
                                                {
                                                        g_debug("Unown attribute %s\n",
                                                                attribute_values[i]);
                                                }
                                }
                        break;
                case TAG_SAMPLE:
                        for (i = 0; attribute_names[i] != NULL; i++)
                                {
                                        if (!strcmp (attribute_names[i], "time"))
                                                {
                                                        g_debug("Ignoring time, too complex for now %s\n",
                                                                attribute_values[i]);
                                                }
                                        else if (!strcmp (attribute_names[i], "depth"))
                                                {
                                                        double d = strtof(attribute_values[i], NULL);
                                                }
                                        else if (!strcmp (attribute_names[i], "temperature"))
                                                {
                                                        double d = strtof(attribute_values[i], NULL);
                                                }
                                        else if (!strcmp (attribute_names[i], "tank"))
                                                {
                                                        double d = strtof(attribute_values[i], NULL);
                                                }
                                        else
                                                {
                                                        g_debug("Unown attribute %s\n",
                                                                attribute_values[i]);
                                                }
                                }
                        break;
                case TAG_PRESSURE:
                        for (i = 0; attribute_names[i] != NULL; i++)
                                {
                                        if (!strcmp (attribute_names[i], "tank"))
                                                {
                                                        double d = strtof(attribute_values[i], NULL);
                                                }
                                        else
                                                {
                                                        g_debug("Unown attribute %s\n",
                                                                attribute_values[i]);
                                                }
                                }
                        break;
                }
}

static void
text_handler(GMarkupParseContext *context,
             const gchar *text,
             gsize text_len,
             gpointer user_data,
             GError **error)
{
        ParseContext *ctx = user_data;
        /* Find the attributes for the current tag */
        switch (get_tag(ctx->tags))
                {
                case TAG_DIVES:
                        return;
                case TAG_PRESSURE:
                        g_debug("** pressure node: %s\n", text);
                }
}

static void
end_element_handler (GMarkupParseContext *context,
		       const gchar *element_name,
		       gpointer user_data,
		       GError **error)
{
        ParseContext *ctx = user_data;
        pop_tag(ctx->tags);
}

static GMarkupParser kelp_parser = {
        start_element_handler,
        end_element_handler,	//end_element_handler,
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
        ParseContext ctx = { 0 };
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
                                                              0, &ctx, NULL);

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
