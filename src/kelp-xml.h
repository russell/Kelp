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

#ifndef __KELP_XML_H__
#define __KELP_XML_H__

#include <glib.h>
#include <glib/gstdio.h>

gint kelp_load_xml(gchar *filename);
void kelp_save_xml(gchar *filename);

enum
{
	XML_OK,
	XML_LOAD_ERROR,
	XML_FILE_ERROR,
	XML_VERSION_ERROR,
};


#endif /* __KELP_APP_H__ */
