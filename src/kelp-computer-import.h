/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * kelp_computer_import.h
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

#include <suunto.h>
#include <reefnet.h>
#include <uwatec.h>
#include <oceanic.h>
#include <mares.h>
#include <hw.h>
#include <cressi.h>
#include <zeagle.h>

static const char *g_cachedir = NULL;
static int g_cachedir_read = 1;

typedef struct device_data_t {
	device_type_t backend;
	device_devinfo_t devinfo;
	device_clock_t clock;
} device_data_t;

typedef struct dive_data_t {
	device_data_t *devdata;
	FILE* fp;
	unsigned int number;
	dc_buffer_t *fingerprint;
} dive_data_t;

typedef struct sample_data_t {
	FILE* fp;
	unsigned int nsamples;
} sample_data_t;

typedef struct backend_table_t {
	const char *name;
	device_type_t type;
} backend_table_t;

static const backend_table_t g_backends[] = {
	{"Suunto Solution",	DEVICE_TYPE_SUUNTO_SOLUTION},
	{"Suunto Eon",			DEVICE_TYPE_SUUNTO_EON},
	{"Suunto Vyper",		DEVICE_TYPE_SUUNTO_VYPER},
	{"Suunto Vyper2",		DEVICE_TYPE_SUUNTO_VYPER2},
	{"Suunto D9",			DEVICE_TYPE_SUUNTO_D9},
	{"Uwatec Aladin",		DEVICE_TYPE_UWATEC_ALADIN},
	{"Uwatec Memo Mouse",	DEVICE_TYPE_UWATEC_MEMOMOUSE},
	{"Uwatec Smart",		DEVICE_TYPE_UWATEC_SMART},
	{"Reefnet Sensus",		DEVICE_TYPE_REEFNET_SENSUS},
	{"Reefnet Sensus Pro",	DEVICE_TYPE_REEFNET_SENSUSPRO},
	{"Reefnet Sensus Ultra",	DEVICE_TYPE_REEFNET_SENSUSULTRA},
	{"Oceanic VT Pro",		DEVICE_TYPE_OCEANIC_VTPRO},
	{"Oceanic Veo 250",		DEVICE_TYPE_OCEANIC_VEO250},
	{"Oceanic Atom 2",		DEVICE_TYPE_OCEANIC_ATOM2},
	{"Mares Nemo",		DEVICE_TYPE_MARES_NEMO},
	{"Mares Puck",		DEVICE_TYPE_MARES_PUCK},
	{"Mares Icon HD",		DEVICE_TYPE_MARES_ICONHD},
	{"OSTC",		DEVICE_TYPE_HW_OSTC},
	{"Cressi Edy",			DEVICE_TYPE_CRESSI_EDY},
	{"Zeagle N2iTion3",	DEVICE_TYPE_ZEAGLE_N2ITION3}
};
