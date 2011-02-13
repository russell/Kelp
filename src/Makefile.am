## Process this file with automake to produce Makefile.in

## Created by Anjuta
include $(top_srcdir)/flymake.mk

uidir = $(datadir)/kelp/ui
ui_DATA = kelp.ui

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(KELP_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = kelp

kelp_SOURCES = \
	kelp-app.h \
	callbacks.c \
	callbacks.h \
	kelp-computer-import.h \
	kelp-computer-import.c \
	kelp-prefs.c \
	kelp-prefs.h \
	main.c

kelp_LDFLAGS = \
	-Wl,--export-dynamic

kelp_LDADD = $(KELP_LIBS)

EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(datadir)/kelp
