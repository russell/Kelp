
include $(top_srcdir)/flymake.mk


TESTS = check_xml
check_PROGRAMS = check_xml
check_xml_SOURCES = check_xml.c $(top_builddir)/src/kelp-xml.c
check_xml_CFLAGS = -g3 -O0 @CHECK_CFLAGS@ $(KELP_CFLAGS) -I$(top_builddir)/src/
check_xml_LDADD = @CHECK_LIBS@ $(KELP_LIBS)
