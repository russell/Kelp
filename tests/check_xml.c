/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * check_xml.c
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

#include <check.h>
#include <glib.h>
#include <stdlib.h>

#include "kelp-xml.h"

START_TEST (test_xml_import)
{
        char filename[1024];
        getcwd(filename, sizeof(filename));
        strcat(filename, "/test.xml");
        gint ret = kelp_load_xml (filename);
        fail_unless (ret == XML_OK,
                     "Parsed file successfully.");
}
END_TEST


Suite *
xml_suite (void)
{
        Suite *s = suite_create ("XML");

        /* Core test case */
        TCase *tc_core = tcase_create ("Core");
        tcase_add_test (tc_core, test_xml_import);
        suite_add_tcase (s, tc_core);

        return s;
}


int
main(void)
{
        int nf;
        Suite *s = xml_suite();
        SRunner *sr = srunner_create(s);
        srunner_set_log(sr, "check_xml.log");
        srunner_run_all(sr, CK_NORMAL);
        nf = srunner_ntests_failed(sr);
        srunner_free(sr);
        //suite_free(s);
        return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
