
include $(top_srcdir)/flymake.mk


TESTS = check_xml check_app
check_PROGRAMS = check_xml check_app

check_xml_SOURCES = check_xml.c $(top_builddir)/src/kelp-xml.c $(top_builddir)/src/kelp-app.c
check_xml_CFLAGS = -g3 -O0 @CHECK_CFLAGS@ $(KELP_CFLAGS) -I$(top_builddir)/src/
check_xml_LDADD = @CHECK_LIBS@ $(KELP_LIBS)

check_app_SOURCES = check_app.c $(top_builddir)/src/kelp-app.c
check_app_CFLAGS = -g3 -O0 @CHECK_CFLAGS@ $(KELP_CFLAGS) -I$(top_builddir)/src/
check_app_LDADD = @CHECK_LIBS@ $(KELP_LIBS)
