dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(kelp, 0.1)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

AC_PROG_CC

dnl ***************************************************************************
dnl Internatinalization
dnl ***************************************************************************
GETTEXT_PACKAGE=kelp
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [GETTEXT package name])
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.35.0])

AM_PROG_LIBTOOL

# Check for check libraries.
PKG_CHECK_MODULES([CHECK], [check >= 0.9.4],,
  AC_MSG_WARN([Check not found; cannot run unit tests!]))
AM_CONDITIONAL(HAVE_CHECK, test x"$have_check" = "xyes")

PKG_CHECK_MODULES(KELP, [gtk+-2.0 >= 2.16 ])


AC_OUTPUT([
Makefile
src/Makefile
po/Makefile.in
tests/Makefile
])
