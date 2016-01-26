#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([OpenVPNLDAPGroups],[0.1],[oliver.d@prodege.com])

# Debug option
AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug], [enable debug compilation, default: no]),
[case "${enableval}" in
    yes) debug=true ;;
    no)  debug=false ;;
    *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac], [debug=false])
AS_IF([test x$debug = xtrue],[
: ${CXXFLAGS="-flto -Wall -g"}
], [
: ${CXXFLAGS="-Ofast -flto -Wall"}
])

AC_CONFIG_SRCDIR([src/VPNPlugin.cpp])
AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_MACRO_DIR([m4/])
AC_CONFIG_FILES([Makefile src/Makefile pch/Makefile])
AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])
LT_INIT

AC_PATH_PROG([GCC_AR], [gcc-ar], [no])
AC_PATH_PROG([GCC_NM], [gcc-nm], [no])
AC_PATH_PROG([GCC_RL], [gcc-ranlib], [no])

AS_IF([test "$GCC_AR" = "no" || test "$GCC_NM" = "no" || test "$GCC_RL" = "no"], ,[
AR="$GCC_AR"
NM="$GCC_NM"
RANLIB="$GCC_RL"
AC_SUBST([AR])
AC_SUBST([NM])
AC_SUBST([RANLIB])])

# Checks for programs.
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11(, [mandatory])
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS(ldap.h openvpn-plugin.h,,AC_MSG_ERROR([required header missing]))
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS(yaml-cpp/yaml.h iostream fstream string stdexcept type_traits unordered_map list forward_list functional,,AC_MSG_ERROR([required header missing]))
AC_LANG_POP([C++])

# Checks for libraries.
AC_SEARCH_LIBS([ldap_initialize], [ldap], , AC_MSG_ERROR([libldap missing]))
AC_LANG_PUSH([C++])
AX_CXX_CHECK_LIB(yaml-cpp, [main], ,AC_MSG_ERROR([libyaml-cpp missing]))
AC_LANG_POP([C++])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strdup])

AC_OUTPUT
