dnl Process this file with autoconf 2.60 or later
dnl to produce a configure script.
AC_INIT(epix, 1.2.19)
AC_PREREQ(2.60)
AC_CONFIG_SRCDIR(utils.h)
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_MAKE_SET
AC_PROG_INSTALL

dnl User may request epix.el non-installation
AC_ARG_ENABLE(epix-el,
              AS_HELP_STRING([--enable-epix-el],
                             [Install emacs mode for ePiX files]),
              enable_epix_el=${enableval-yes},
              enable_epix_el=yes)
AM_CONDITIONAL(ENABLE_EPIX_EL, [test "$enable_epix_el" = yes])

dnl For all shell scripts
AC_PATH_PROG(BASH_PATH, bash, /bin/bash)
AC_ARG_WITH(bash,
            AS_HELP_STRING([--with-bash=<bash>],
                           [The path to bash]),
            BASH_PATH=$withval)

dnl For epix and flix
AC_PATH_PROG(RUNTIME_COMPILER, g++, g++)
AC_ARG_WITH(runtime,
            AS_HELP_STRING([--with-runtime=<g++>],
                           [The runtime compiler]),
            RUNTIME_COMPILER=$withval)

dnl AC_ARG_WITH(contrib,
dnl             AS_HELP_STRING([--with-contrib],
dnl                            [Build contributed code and docs]))
dnl AM_CONDITIONAL(BUILD_CONTRIB, [test "$with_contrib" = yes])

dnl Checks for libraries.

dnl Checks for header files.
AC_HEADER_STDC

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE

dnl Checks for library functions.
AC_CHECK_FUNCS(strtod)

AC_CONFIG_FILES([Makefile samples/Makefile doc/Makefile])
dnl                contrib/Makefile contrib/doc/Makefile])

AC_OUTPUT
