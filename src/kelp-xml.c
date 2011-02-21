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


typedef struct _TagStack
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
        return ps->items[ps->size - 1];
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
        Dive *dive;
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
        Sample *sample = NULL;
        Pressure *pressure = NULL;
        ParseContext *ctx = user_data;
        g_debug("** start element: %s\n", element_name);
        if (!strcmp(element_name, "dives"))
                push_tag(&ctx->tags, TAG_DIVES);
        else if (!strcmp(element_name, "dive"))
                push_tag(&ctx->tags, TAG_DIVE);
        else if (!strcmp(element_name, "sample"))
                push_tag(&ctx->tags, TAG_SAMPLE);
        else if (!strcmp(element_name, "pressure"))
                push_tag(&ctx->tags, TAG_PRESSURE);
        else
                {
                        push_tag(&ctx->tags, TAG_UNKNOWN);
                        g_debug("Unown tag %s\n", element_name);
                }
        /* Find the attributes for the current tag */
        switch (get_tag(&ctx->tags))
                {
                case TAG_DIVES:
                        return;
                case TAG_DIVE:
                        ctx->dive = g_malloc0(sizeof(Dive));
                        for (i = 0; attribute_names[i] != NULL; i++)
                                if (!strcmp (attribute_names[i], "datetime"))
                                        {
                                                struct tm tm;
                                                time_t *time = NULL;
                                                strptime(attribute_values[i], "%Y-%m-%d %H:%M:%S", &tm);
                                                time = g_malloc0(sizeof(time));
                                                *time = mktime(&tm);
                                                ctx->dive->datetime = *time;
                                        }
                                else if (!strcmp (attribute_names[i], "number"))
                                        ctx->dive->number = atol(attribute_values[i]);
                                else if (!strcmp (attribute_names[i], "size"))
                                        ctx->dive->size = atol(attribute_values[i]);
                                else if (!strcmp (attribute_names[i], "fingerprint"))
                                        ctx->dive->fingerprint = g_strdup(attribute_values[i]);
                                else if (!strcmp (attribute_names[i], "maxdepth"))
                                        ctx->dive->maxdepth = strtod(attribute_values[i], NULL);
                                else
                                        g_debug("Unown Dive attribute %s\n",
                                                attribute_names[i]);
                        break;
                case TAG_SAMPLE:
                        sample = g_malloc0(sizeof(Sample));
                        for (i = 0; attribute_names[i] != NULL; i++)
                                if (!strcmp (attribute_names[i], "time"))
                                        {
                                                struct tm tm;
                                                time_t *time = NULL;
                                                strptime(attribute_values[i], "%H:%M:%S", &tm);
                                                time = g_malloc0(sizeof(time));
                                                *time = mktime(&tm);
                                                sample->time = *time;
                                        }
                                else if (!strcmp (attribute_names[i], "depth"))
                                        sample->depth = strtod(attribute_values[i], NULL);
                                else if (!strcmp (attribute_names[i], "temperature"))
                                        sample->temperature = strtof(attribute_values[i], NULL);
                                else
                                        {
                                                g_debug("Unown Sample attribute %s\n",
                                                        attribute_names[i]);
                                        }
                        ctx->dive->samples = g_list_append(ctx->dive->samples, sample);
                        break;
                case TAG_PRESSURE:
                        sample = g_list_last(ctx->dive->samples)->data;
                        pressure = g_malloc0(sizeof(Pressure));
                        for (i = 0; attribute_names[i] != NULL; i++)
                                if (!strcmp (attribute_names[i], "tank"))
                                        pressure->tank = atol(attribute_values[i]);
                                else
                                        {
                                                g_debug("Unown Pressure attribute %s\n",
                                                        attribute_values[i]);
                                        }
                        sample->pressures = g_list_append(sample->pressures, pressure);
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
        Pressure *pressure = NULL;
        Sample *sample = NULL;
        ParseContext *ctx = user_data;
        /* Find the attributes for the current tag */
        switch (get_tag(&ctx->tags))
                {
                case TAG_DIVES:
                        return;
                case TAG_PRESSURE:
                        sample = g_list_last(ctx->dive->samples)->data;
                        pressure = g_list_last(sample->pressures)->data;
                        pressure->value = strtod(text, NULL);
                }
}


static void
end_element_handler (GMarkupParseContext *context,
		       const gchar *element_name,
		       gpointer user_data,
		       GError **error)
{
        ParseContext *ctx = user_data;
        switch (get_tag(&ctx->tags))
                {
                case TAG_DIVE:
                        // TODO Should check there is a finger print here
                        // if not then i should generate one
                        kelp_add_dive(ctx->dive);
                        ctx->dive = NULL;
                }
        pop_tag(&ctx->tags);
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
