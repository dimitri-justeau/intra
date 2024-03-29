# dnl Process this file with autoconf to produce a configure script.
define([pkgversion], esyscmd([sh -c "grep Version: DESCRIPTION | cut -d' ' -f2 | tr -d '\n'"]))dnl
AC_INIT([rgdal],[pkgversion],[Roger.Bivand@nhh.no])
AC_CONFIG_SRCDIR(src/RasterGraph.cpp)

# AC_PROG_CXX()

# find R home and set correct compiler + flags
: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
  AC_MSG_ERROR([cannot determine R_HOME. Make sure you use R CMD INSTALL!])
fi
AC_MSG_NOTICE([R_HOME: ${R_HOME}])
RBIN="${R_HOME}/bin/R"

# pick all flags for testing from R
: ${CC=`"${RBIN}" CMD config CC`}
: ${CXX=`"${RBIN}" CMD config CXX`}
#: ${CPP=`"${RBIN}" CMD config CPP`}
: ${CFLAGS=`"${RBIN}" CMD config CFLAGS`}
: ${CPPFLAGS=`"${RBIN}" CMD config CPPFLAGS`}
: ${CXXFLAGS=`"${RBIN}" CMD config CXXFLAGS`}
: ${LDFLAGS=`"${RBIN}" CMD config LDFLAGS`}
# AC_SUBST([CC],["clang"])
# AC_SUBST([CXX],["clang++"])
AC_MSG_NOTICE([CC: ${CC}])
AC_MSG_NOTICE([CXX: ${CXX}])
AC_MSG_NOTICE([CFLAGS: ${CFLAGS}])
AC_MSG_NOTICE([CPPFLAGS: ${CPPFLAGS}])
AC_MSG_NOTICE([CXXFLAGS: ${CXXFLAGS}])
AC_MSG_NOTICE([LDFLAGS: ${LDFLAGS}])

AC_ARG_ENABLE([loadflags],
    [AS_HELP_STRING([--enable-loadflags=yes/no],[use value of R CMD config LDFLAGS for configure, default yes])],
    [loadflags=${enableval}],
    [loadflags="yes"])

#echo "loadflags: $loadflags"

if test "${loadflags}" = "no" ; then
    LDFLAGS=""
    AC_MSG_NOTICE(value of R CMD config LDFLAGS not used for configure)
fi
AC_MSG_NOTICE([LDFLAGS: ${LDFLAGS}])

#CXX_11

R_VERSION=`echo 'o <- R.version; cat(paste0(o$major, ".", o$minor))' | ${RBIN} --vanilla --slave`
#AC_MSG_NOTICE([R_VERSION is: ${R_VERSION}])
R_MINOR=`echo $R_VERSION | cut -f2 -d"."`
R_MAJOR=`echo $R_VERSION | cut -f1 -d"."`
#AC_MSG_NOTICE([R_MAJOR is: ${R_MAJOR}, R_MINOR is: ${R_MINOR}])

#if test ${R_MINOR} -ge 4; then
#: ${CXX=`"${RBIN}" CMD config CXX11`}
#else
#: ${CXX=`"${RBIN}" CMD config CXX1X`}
#fi

if test ${R_MAJOR} -ge 4; then
 CXX11=`"${RBIN}" CMD config CXX11`
 CXX11STD=`"${RBIN}" CMD config CXX11STD`
else
# if test ${R_MAJOR} -eq 3 -a ${R_MINOR} -ge 4; then
 if test ${R_MAJOR} -eq 3 && test ${R_MINOR} -ge 4; then
  CXX11=`"${RBIN}" CMD config CXX11`
  CXX11STD=`"${RBIN}" CMD config CXX11STD`
 else
  CXX11=`"${RBIN}" CMD config CXX1X`
  CXX11STD=`"${RBIN}" CMD config CXX1XSTD`
 fi
fi

AC_MSG_NOTICE([CXX11 is: ${CXX11}, CXX11STD is: ${CXX11STD}])

CXX="${CXX11} ${CXX11STD}"

AC_MSG_NOTICE([CXX is: ${CXX}])

if test -n "${CXX11}"; then
HAVE_CXX11=1
else
HAVE_CXX11=0
fi

#HAVE_CXX11=1

#m4_include([inst/m4/ax_cxx_compile_stdcxx.m4])
#AX_CXX_COMPILE_STDCXX([11], [ext], [optional])
if test [${HAVE_CXX11} = 1] ; then
  AC_MSG_NOTICE([C++11 support available])
else
  AC_MSG_NOTICE([C++11 support not available])
fi


AC_MSG_NOTICE([${PACKAGE_NAME}: ${PACKAGE_VERSION}])

AC_CHECK_FILE([/usr/bin/svnversion],
 [SVN_VERSION=`svnversion -n '.'`],
 [SVN_VERSION=""])

#SVN_VERSION=`svnversion -n '.'`
#if test "${SVN_VERSION}" != "exported" -a test -n "${SVN_VERSION}" -a test "${SVN_VERSION}" != "Unversioned directory"; then
if test "${SVN_VERSION}" != "exported" && test -n "${SVN_VERSION}" && test "${SVN_VERSION}" != "Unversioned directory"; then  echo "${SVN_VERSION}" > inst/SVN_VERSION
else
  SVN_VERSION=`cat inst/SVN_VERSION | tr -d '\n'`
fi
AC_MSG_NOTICE([svn revision: ${SVN_VERSION}])

#GDAL

GDAL_CONFIG="gdal-config"

GDAL_CONFIG_SET="no"

AC_ARG_WITH([gdal-config],
    AS_HELP_STRING([--with-gdal-config=GDAL_CONFIG],[the location of gdal-config]),
           [gdal_config=$withval])
if test [ -n "$gdal_config" ] ; then
    GDAL_CONFIG_SET="yes"
    AC_SUBST([GDAL_CONFIG],["${gdal_config}"])
    AC_MSG_NOTICE(gdal-config set to $GDAL_CONFIG)
fi

if test ["$GDAL_CONFIG_SET" = "no"] ; then
  AC_PATH_PROG([GDAL_CONFIG], ["$GDAL_CONFIG"],["no"])
  if test ["$GDAL_CONFIG" = "no"] ; then
    AC_MSG_RESULT(no)
    AC_MSG_ERROR([gdal-config not found or not executable.])
  fi
else
  AC_MSG_CHECKING(gdal-config exists)
  if test -r "${GDAL_CONFIG}"; then
    AC_MSG_RESULT(yes)
  else
    AC_MSG_RESULT(no)
    AC_MSG_ERROR([gdal-config not found - configure argument error.])
  fi
  AC_MSG_CHECKING(gdal-config executable)
  if test -x "${GDAL_CONFIG}"; then
    AC_MSG_RESULT(yes)
  else
    AC_MSG_RESULT(no)
    AC_MSG_ERROR([gdal-config not executable.])
  fi
fi


AC_MSG_CHECKING(gdal-config usability)
if test `${GDAL_CONFIG} --version`;
then
  GDAL_CPPFLAGS=`${GDAL_CONFIG} --cflags`
  GDAL_LIBS=`${GDAL_CONFIG} --libs`
  GDAL_VERSION=`${GDAL_CONFIG} --version`
  GDAL_DEP_LIBS=`${GDAL_CONFIG} --dep-libs`
  GDAL_DATADIR=`${GDAL_CONFIG} --datadir`
  gdalok=yes
  AC_MSG_RESULT(yes)
else
      AC_MSG_RESULT(no)
	echo "Error: gdal-config not found"
	echo "The gdal-config script distributed with GDAL could not be found."
	echo "If you have not installed the GDAL libraries, you can"
	echo "download the source from  http://www.gdal.org/"
	echo "If you have installed the GDAL libraries, then make sure that"
	echo "gdal-config is in your path. Try typing gdal-config at a"
	echo "shell prompt and see if it runs. If not, use:"
	echo " --configure-args='--with-gdal-config=/usr/local/bin/gdal-config'"
	echo "with appropriate values for your installation."
	echo ""

	exit 1
fi

#AC_MSG_NOTICE([GDAL_LIBS is: ${GDAL_LIBS}])
#AC_MSG_NOTICE([GDAL_DEP_LIBS is: ${GDAL_DEP_LIBS}])

AC_MSG_NOTICE([GDAL: ${GDAL_VERSION}])
#GDAL_VER_DOT=`echo $GDAL_VERSION | tr -d "."`
GDAL_MAJOR=`echo $GDAL_VERSION | cut -f1 -d"."`
GDAL_MINOR=`echo $GDAL_VERSION | cut -f2 -d"."`
GDAL_PATCH=`echo $GDAL_VERSION | cut -f3 -d"."`

#if test ${GDAL_MAJOR} = 2 -a test ${GDAL_MINOR} -ge 3 ; then
if test ${GDAL_MAJOR} = 2 && test ${GDAL_MINOR} -ge 3 ; then
  AC_MSG_CHECKING([C++11 support for GDAL >= 2.3.0])
  if test ${HAVE_CXX11} = 1 ; then
    AC_MSG_RESULT(yes)
  else
    AC_MSG_RESULT(no)
    AC_MSG_ERROR([provide at least CXX11 compiler support for GDAL >= 2.3.0])
  fi
fi

AC_MSG_CHECKING([GDAL version >= 1.11.4])
if test ${GDAL_MAJOR} -gt 1 ; then
  AC_MSG_RESULT(yes)
else
#  if test ${GDAL_MAJOR} = 1 -a test ${GDAL_MINOR} = 11 -a test ${GDAL_PATCH} -ge 4 ; then
  if test ${GDAL_MAJOR} = 1 && test ${GDAL_MINOR} = 11 && test ${GDAL_PATCH} -ge 4 ; then
    AC_MSG_RESULT(yes)
  else
    AC_MSG_RESULT(no)
    AC_MSG_ERROR([upgrade GDAL to 1.11.4 or later])
  fi
fi


AC_MSG_CHECKING([GDAL version <= 2.5 or >= 3.0])
if test ${GDAL_MAJOR} -lt 2 || test ${GDAL_MAJOR} -gt 2; then
  AC_MSG_RESULT(yes)
else
  if test ${GDAL_MAJOR} = 2; then
    if test ${GDAL_MINOR} -le 5 ; then
      AC_MSG_RESULT(yes)
    else
      AC_MSG_RESULT(no)
      AC_MSG_ERROR([GDAL version out of bounds])
    fi
  fi
fi


INLIBS="${LIBS}"
INCPPFLAGS="${CPPFLAGS}"
INPKG_CPPFLAGS="${PKG_CPPFLAGS}"
INPKG_LIBS="${PKG_LIBS}"

AC_SUBST([PKG_CPPFLAGS], ["${INPKG_CPPFLAGS} ${GDAL_CPPFLAGS}"])
AC_SUBST([PKG_LIBS], ["${INPKG_LIBS} ${GDAL_LIBS}"])

# honor PKG_xx overrides
# for CPPFLAGS we will superfluously double R's flags
# since we'll set PKG_CPPFLAGS with this, but that shouldn't hurt
CPPFLAGS="${INCPPFLAGS} ${PKG_CPPFLAGS}"


NEED_DEPS=no
LIBS="${INLIBS} ${PKG_LIBS}"
[cat > gdal_test.cc <<_EOCONF
#include <gdal.h>
#ifdef __cplusplus
extern "C" {
#endif
int main() {
GDALAllRegister();
}
#ifdef __cplusplus
}
#endif
_EOCONF]

AC_MSG_CHECKING(GDAL: linking with --libs only)
${CXX} ${CPPFLAGS} -o gdal_test gdal_test.cc ${LIBS} 2> errors.txt
if test `echo $?` -ne 0 ; then
gdalok=no
AC_MSG_RESULT(no)
else
AC_MSG_RESULT(yes)
fi

if test "${gdalok}" = no; then
AC_MSG_CHECKING(GDAL: linking with --libs and --dep-libs)
LIBS="${LIBS} ${GDAL_DEP_LIBS}"
gdalok=yes
${CXX} ${CPPFLAGS} -o gdal_test gdal_test.cc ${LIBS} 2>> errors.txt
if test `echo $?` -ne 0 ; then
gdalok=no
fi
if test "${gdalok}" = yes; then
    NEED_DEPS=yes
    AC_MSG_RESULT(yes)
else
    AC_MSG_RESULT(no)
fi
fi

if test "${gdalok}" = no; then
   cat errors.txt
   AC_MSG_NOTICE([Install failure: compilation and/or linkage problems.])
   AC_MSG_ERROR([GDALAllRegister not found in libgdal.])
fi

rm -f gdal_test errors.txt gdal_test.cc

AC_MSG_CHECKING(GDAL: gdal-config data directory readable)
if test -d "${GDAL_DATADIR}" ; then
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
  AC_MSG_CHECKING(GDAL: GDAL_DATA data directory readable)
  if test -z ${GDAL_DATA} ; then
    AC_MSG_ERROR([GDAL data directory not found.])
  else
    if test -d "${GDAL_DATA}" ; then
      AC_MSG_RESULT(yes)
      AC_SUBST([GDAL_DATADIR], [${GDAL_DATA}])
    else
      AC_MSG_RESULT(no)
      AC_MSG_ERROR([GDAL data directory not found.])
    fi
  fi
fi

GDAL_GE_250="no"
GDAL_MAJ_VER=`echo $GDAL_VERSION | cut -d "." -f1`
GDAL_MOD_VER=`echo $GDAL_VERSION | cut -d "." -f2`
if test "${GDAL_MAJ_VER}" = 2 ; then
  if test "${GDAL_MOD_VER}" -ge 5 ; then
    GDAL_GE_250="yes"
  fi
else
  if test "${GDAL_MAJ_VER}" -ge 3 ; then
    GDAL_GE_250="yes"
  fi
fi

GDAL_DATA_TEST_FILE="${GDAL_DATADIR}/stateplane.csv"
AC_MSG_CHECKING(GDAL: ${GDAL_DATADIR}/stateplane.csv readable)
if test -r "${GDAL_DATA_TEST_FILE}" ; then
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
#  if test "${GDAL_GE_250}" = no; then
    AC_MSG_ERROR([stateplane.csv not found in GDAL data directory.])
#  fi
fi

# PROJ.4

# copied from sf/configure.ac

PROJ_CONFIG="pkg-config proj"

if `$PROJ_CONFIG --exists` ; then # pkg-config proj
  AC_MSG_NOTICE([pkg-config proj exists, will use it])
  proj_config_ok=yes
else # pkg-config proj
  AC_MSG_NOTICE([pkg-config proj not available])
  echo "  set PKG_CONFIG_PATH to the directory containing proj.pc"
  proj_config_ok=no
fi # pkg-config proj

PROJ_VERSION=""
PROJ6=""

if test "${proj_config_ok}" = yes; then # proj_config_ok
  PROJ_VERSION=`${PROJ_CONFIG} --modversion`
  AC_MSG_NOTICE([PROJ version: ${PROJ_VERSION}])

  PROJ_VERSION_DOT=`echo ${PROJ_VERSION} | tr -d "."`
  PROJV1=`echo "${PROJ_VERSION}" | cut -c 1`

  if test "${PROJV1}" -ge 6; then # PROJ >= 6
    PKG_CPPFLAGS="-DPROJ_H_API"
    PROJ6="yes"
  else # PROJ >= 6
    AC_MSG_NOTICE([PROJ version < 6.0.0])
    PKG_CPPFLAGS=""
    PROJ6="no"
  fi # PROJ >= 6

else # proj_config_ok

  AC_MSG_NOTICE([PROJ version not determined using pkg-config proj])

fi # proj_config_ok


AC_ARG_WITH([proj-include],
    AS_HELP_STRING([--with-proj-include=DIR],[location of proj header files]),
    [proj_include_path=$withval])
if test [ -n "$proj_include_path" ] ; then # proj_include_path
   proj_include_path="-I${proj_include_path}"
   AC_SUBST([PKG_CPPFLAGS],["${PKG_CPPFLAGS} ${proj_include_path} ${INPKG_CPPFLAGS}"])
  if test "${proj_config_ok}" = yes; then # proj_config_ok
    AC_MSG_NOTICE([--with-proj-include= overrides pkg-config proj --cflags])
  fi # proj_config_ok
else # proj_include_path
  if test "${proj_config_ok}" = yes; then # proj_config_ok
    PROJ_INCLUDE_PATH=`${PROJ_CONFIG} --cflags`
    proj_include_path=${PROJ_INCLUDE_PATH}
    AC_SUBST([PKG_CPPFLAGS],["${PKG_CPPFLAGS} ${PROJ_INCLUDE_PATH} ${INPKG_CPPFLAGS}"])
  fi # proj_config_ok
fi # proj_include_path

# honor PKG_xx overrides
# for CPPFLAGS we will superfluously double R's flags
# since we'll set PKG_CPPFLAGS with this, but that shouldn't hurt
CPPFLAGS="${INCPPFLAGS} ${PKG_CPPFLAGS}"
AC_MSG_NOTICE([PROJ CPP flags: ${PKG_CPPFLAGS}])
PKG_LIBS=""
PROJ_LIB_PATH=""

# dnl ditto for a library path
AC_ARG_WITH([proj-lib],
    AS_HELP_STRING([--with-proj-lib=LIB_PATH],[the location of proj libraries]),
               [proj_lib_path=$withval])
if test [ -n "$proj_lib_path" ] ; then # proj_lib_path
    AC_SUBST([PKG_LIBS], ["-L${proj_lib_path} ${INPKG_LIBS}"])
  if test "${proj_config_ok}" = yes; then # proj_config_ok
    AC_MSG_NOTICE([--with-proj-lib= overrides pkg-config proj --libs])
  fi # proj_config_ok
else # proj_lib_path
  if test ${proj_config_ok} = "yes"; then # proj_config_ok
    PROJ_LIB_PATH=`${PROJ_CONFIG} --libs`
    AC_SUBST([PKG_LIBS], ["${PROJ_LIB_PATH} ${INPKG_LIBS}"])
  else # proj_config_ok
    AC_SUBST([PKG_LIBS], ["${INPKG_LIBS} -lproj"])
  fi # proj_config_ok
fi # proj_lib_path

LIBS="${PKG_LIBS} ${LIBS}"
AC_MSG_NOTICE([PROJ LIBS: ${PKG_LIBS}])


AC_ARG_WITH([proj_api],
    [  --with-proj_api[=ARG]        legacy proj_api.h while still available, or current proj.h (ARG="proj_api.h", "proj.h"(default))],,)

AC_MSG_CHECKING([PROJ header API:])

if test -z "$with_proj_api" ; then # with_proj_api
  PROJ_API=""
  if test "${PROJ6}" = "yes" ; then # PROJ6
    PROJ_API="proj.h"
  fi
  if test "${PROJ6}" = "no" ; then # PROJ6
    PROJ_API="proj_api.h"
  fi
else # with_proj_api
  PROJ_API="$with_proj_api"
fi # with_proj_api

api_string_ok="no"
if test -z "${PROJ_API}" ; then
  api_string_ok="yes"
fi
if test "${PROJ_API}" = "proj.h" ; then
  api_string_ok="yes"
fi
if test "${PROJ_API}" = "proj_api.h" ; then
  api_string_ok="yes"
fi

if test ${api_string_ok} = "no" ; then # PROJ_API
  AC_MSG_RESULT(no)
  AC_MSG_ERROR([PROJ API string ${PROJ_API} not recognized.])
else
  AC_MSG_RESULT(yes)
fi # PROJ_API

if test -z ${PROJ_API} ; then # PROJ_API = ""
  AC_MSG_NOTICE([API to be used as yet undetermined, searching ...])
  PROJ_VERSION_H=""
  LIBS="${INLIBS} ${PKG_LIBS}"
# proj.h case
[cat > proj_conf_test_proj.c <<_EOCONF
#include <stdio.h>
#include <stdlib.h>
#include <proj.h>

int main() {
    printf("%d.%d.%d\n", PROJ_VERSION_MAJOR, PROJ_VERSION_MINOR, PROJ_VERSION_PATCH);
    exit(0);
}
_EOCONF]

  if test `${CC} ${CFLAGS} ${PKGCPPFLAGS} -o proj_conf_test_proj proj_conf_test_proj.c ${PKG_LIBS} 2> /dev/null; echo $?` = 0 ; then
    PROJ_API="proj.h"
    PROJ6="yes"
    PROJ_VERSION_H=`./proj_conf_test_proj`
    PROJ_VERSION_DOT=`echo ${PROJ_VERSION_H} | tr -d "."`
rm -f proj_conf_test_proj.c proj_conf_test_proj
  else
[cat > proj_conf_test_proj_api.c <<_EOCONF
#include <stdio.h>
#include <stdlib.h>
#include <proj_api.h>

int main() {
    printf("%d\n", PJ_VERSION);
    exit(0);
}
_EOCONF]
    if test `${CC} ${CFLAGS} ${PKG_CPPFLAGS} -o proj_conf_test_proj_api proj_conf_test_proj_api.c ${PKG_LIBS} 2> /dev/null; echo $?` = 0; then
      PROJ_API="proj_api.h"
      PROJ6="no"
      PROJ_VERSION_H=`./proj_conf_test_proj_api`
      PROJ_VERSION_DOT=`echo ${PROJ_VERSION_H} | tr -d "."`
rm -f proj_conf_test_proj_api.c proj_conf_test_proj_api
    fi
  fi
  if test -z "${PROJ_VERSION_H}"; then
    AC_MSG_ERROR([API to be used not found])
  else
    AC_MSG_NOTICE([Using API: ${PROJ_API}])
  fi
fi # PROJ_API = ""

dnl PROJ >= 6 contains C++ code, so AC_CHECK_LIB needs to be
dnl with the C++ runtime (and rgdal is going to use C++).
AC_LANG(C++)

if test "${PROJ_API}" =  "proj.h" ; then # proj.h

  if test ${PROJ6} = "yes"; then # PROJ6
    projhok=yes
    AC_CHECK_HEADER([proj.h],,projhok=no)
    if test "${projhok}" = no; then # libprojok
      AC_MSG_ERROR([proj.h not found in given locations.])
    fi # libprojok
    libprojok=yes
    AC_CHECK_LIB([proj], [proj_context_create],,libprojok=no)
    if test "${libprojok}" = no; then # libprojok
      AC_MSG_ERROR([proj_context_create not found in libproj.])
    fi # libprojok
  fi # PROJ6

fi # proj.h

if test "${PROJ_API}" =  "proj_api.h"; then # proj_api.h

LIBS="${INLIBS} ${PKG_LIBS}"
AC_MSG_CHECKING(proj_api.h presence and usability)
[cat > proj_conf_test1.c <<_EOCONF
#include <stdio.h>
#include <stdlib.h>
#include <proj_api.h>

int main() {
    printf("%d\n", PJ_VERSION);
    exit(0);
}
_EOCONF]

  if test `${CC} ${CFLAGS} ${PKG_CPPFLAGS} -o proj_conf_test1 proj_conf_test1.c ${PKG_LIBS} 2> /dev/null; echo $?` = 0; then
    PROJ_VERSION_H=`./proj_conf_test1`
    AC_MSG_RESULT(yes)
  else
    if test `${CC} ${CFLAGS} ${PKG_CPPFLAGS} -DACCEPT_USE_OF_DEPRECATED_PROJ_API_H -o proj_conf_test1 proj_conf_test1.c ${PKG_LIBS} 2> /dev/null; echo $?` = 0; then
      PROJ6="yes"
      PKG_CPPFLAGS="${PKG_CPPFLAGS} -DACCEPT_USE_OF_DEPRECATED_PROJ_API_H"
      PROJ_VERSION_H=`./proj_conf_test1`
      AC_MSG_RESULT(yes)
    else
      AC_MSG_RESULT(no)
      AC_MSG_ERROR([proj_api.h not found in standard or given locations.])
    fi
  fi

rm -f proj_conf_test1.c proj_conf_test1

#AC_MSG_NOTICE(["|${PROJ_VERSION}| |${PROJ_VERSION_H}|"])

if test -n "${PROJ_VERSION}" ; then
  AC_MSG_CHECKING([PROJ version agreement])
  PROJ_VERSION_DOT=`echo ${PROJ_VERSION} | tr -d "."`
#echo "|${PROJ_VERSION}| |${PROJ_VERSION_DOT}| |${PROJ_VERSION_H}|"
  if test "${PROJ_VERSION_DOT}$" = "${PROJ_VERSION_H}$"; then
    AC_MSG_RESULT(yes)
  else
    AC_MSG_RESULT(no)
    AC_MSG_ERROR([proj_api.h and pkg-config give different versions])
  fi
fi

fi

if test -z "${PROJ_VERSION_DOT}"; then
  PROJ_VERSION_DOT=`echo ${PROJ_VERSION_H} | tr -d "."`
fi
NEW_PROJ_WITH_OLD_GDAL="no"
AC_MSG_CHECKING([Using GDAL < 3 with PROJ >= 6])
#if test ${GDAL_MAJOR} -lt 3 -a test ${PROJ_VERSION_DOT} -ge 600; then
if test ${GDAL_MAJOR} -lt 3 && test ${PROJ_VERSION_DOT} -ge 600; then
    AC_MSG_RESULT(yes)
    if test ${PROJ_API} = "proj.h"; then
        AC_MSG_ERROR([GDAL >= 3 should be used with PROJ >= 6, use --with-proj_api="proj_api.h" for deprecated API])
    else
        NEW_PROJ_WITH_OLD_GDAL="yes"
        AC_MSG_WARN([GDAL >= 3 should be used with PROJ >= 6])
    fi
  else
    AC_MSG_RESULT(no)
fi



if test "${PROJ6}" = "yes" && test "${PROJ_API}" =  "proj_api.h"; then
    PKG_CPPFLAGS="${PKG_CPPFLAGS} -DACCEPT_USE_OF_DEPRECATED_PROJ_API_H"
fi
#AC_MSG_NOTICE([CPP FLAGS: ${PKG_CPPFLAGS}])

AC_MSG_CHECKING([PROJ version >= 4.8.0])
if test ${PROJ_VERSION_DOT} -lt 480 ; then
  AC_MSG_RESULT(no)
  AC_MSG_ERROR([upgrade PROJ to 4.8.0 or later])
else
  AC_MSG_RESULT(yes)
fi

if test ${PROJ_VERSION_DOT} -lt 493; then

if test ${PROJ_VERSION_DOT} = 480 ; then
  ADDED_PKG_INCL_480=`pwd`"/inst/include"
  AC_SUBST([PKG_CPPFLAGS],["${PKG_CPPFLAGS} -I${ADDED_PKG_INCL_480}"])
  proj_include_path="${PKG_CPPFLAGS} -I${ADDED_PKG_INCL_480}"
  CPPFLAGS="${INCPPFLAGS} ${PKG_CPPFLAGS}"
fi

#AC_MSG_NOTICE([${CFLAGS} ${PKG_CPPFLAGS}])

AC_MSG_CHECKING(projects.h presence and usability)
# FIXME re-using names
[cat > proj_conf_test1a.c <<_EOCONF
#include <stdio.h>
#include <stdlib.h>
#include <projects.h>

int main() {
    exit(0);
}
_EOCONF]

if test `${CC} ${CFLAGS} ${PKG_CPPFLAGS} -o proj_conf_test1a proj_conf_test1a.c ${PKG_LIBS} 2> /dev/null; echo $?` = 0; then
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
  STOP="stop"
fi
rm -f proj_conf_test1a.c proj_conf_test1a

if test "$STOP" = "stop" ; then
    echo "projects.h not found in standard or given locations."
    echo "PROJ 4.8.0 was bundled wrongly, and did not provide"
    echo "the file. A copy is provided in init/include in the source"
    echo "package for the users to place in the same directory"
    echo "as proj_api.h manually."
    exit 1
fi

fi # < 493 test for projects.h

#AC_MSG_NOTICE([CPP FLAGS: ${PKG_CPPFLAGS}])


#AC_SUBST([PKG_CPPFLAGS],["-DP4CTX=1 ${PKG_CPPFLAGS}"])
#AC_MSG_NOTICE([CPP FLAGS: ${PKG_CPPFLAGS}])

AC_ARG_WITH([proj-share],
    AS_HELP_STRING([--with-proj-share=SHARE_PATH],[the location of proj metadata files]),
               [proj_share_path=$withval])
if test [ -n "$proj_share_path" ] ; then
    AC_MSG_NOTICE([PROJ_LIB: ${proj_share_path}])
fi

if test ${PROJ6} = "no"; then

#AC_MSG_CHECKING(PROJ.6: )
#AC_MSG_RESULT(no)

[cat > proj_conf_test2.c <<_EOCONF
#include <stdio.h>
#include <stdlib.h>
#include <proj_api.h>
#if PJ_VERSION == 480
FILE *pj_open_lib(projCtx, const char *, const char *);
#endif

int main() {
#if PJ_VERSION == 480
    FILE *fp;
#else
    PAFile fp;
#endif
    projCtx ctx;
    ctx = pj_get_default_ctx();
    fp = pj_open_lib(ctx, "epsg", "rb");
    if (fp == NULL) exit(1);
#if PJ_VERSION == 480
    fclose(fp);
#else
    pj_ctx_fclose(ctx, fp);
#endif
    exit(0);
}
_EOCONF]

${CC} ${CFLAGS} ${PKG_CPPFLAGS} -o proj_conf_test2 proj_conf_test2.c ${PKG_LIBS}
if test [ -n "$proj_share_path" ] ; then
  PROJ_LIB="${proj_share_path}" ./proj_conf_test2
  proj_share=`echo $?`
else
  ./proj_conf_test2
  proj_share=`echo $?`
fi

AC_MSG_CHECKING(PROJ.4: epsg found and readable)
if test ${proj_share} -eq 1 ; then
    AC_MSG_RESULT(no)
    STOP="stop"
else
    AC_MSG_RESULT(yes)
fi

rm -f proj_conf_test2.c proj_conf_test2

if test "$STOP" = "stop" ; then
    echo "Error: proj/epsg not found"
    echo "Either install missing proj support files, for example"
    echo "the proj-nad and proj-epsg RPMs on systems using RPMs,"
    echo "or if installed but not autodetected, set PROJ_LIB to the"
    echo "correct path, and if need be use the --with-proj-share="
    echo "configure argument."
    exit 1
fi

else # test for new files if PROJ6

[cat > proj_conf_test2.cpp <<_EOCONF
#include <stdio.h>
#include <stdlib.h>
#include <proj.h>

int main() {
    PJ_CONTEXT *ctx = proj_context_create();
    if (proj_context_get_database_path(ctx) == NULL) {
        proj_context_destroy(ctx);
        exit(1);
    }
    proj_context_destroy(ctx);
    exit(0);
}
_EOCONF]

dnl PROJ >= 6 needs C++ runtime and libsqlite3 (and perhaps more)
dnl when statically linked.
${CXX} ${CPPFLAGS} ${PKG_CPPFLAGS} -o proj_conf_test2 proj_conf_test2.cpp ${PKG_LIBS} -lproj -lsqlite3
if test [ -n "$proj_share_path" ] ; then
  PROJ_LIB="${proj_share_path}" ./proj_conf_test2
  proj_share=`echo $?`
else
  ./proj_conf_test2
  proj_share=`echo $?`
fi

AC_MSG_CHECKING(PROJ: proj.db found and readable)
if test ${proj_share} -eq 1 ; then
    AC_MSG_RESULT(no)
    STOP="stop"
else
    AC_MSG_RESULT(yes)
fi

rm -f proj_conf_test2.cpp proj_conf_test2

if test "$STOP" = "stop" ; then
    echo "Error: proj/proj.db not found"
    echo "Either install missing proj support files, set PROJ_LIB to the"
    echo "correct path, and if need be use the --with-proj-share="
    echo "configure argument."
    exit 1
fi

fi # don't test for old files if PROJ6


[cat > proj_conf_test3.c <<_EOCONF
#include <stdio.h>
#include <stdlib.h>
#include <proj_api.h>
#if PJ_VERSION == 480
FILE *pj_open_lib(projCtx, const char *, const char *);
#endif

int main() {
#if PJ_VERSION == 480
    FILE *fp;
#else
    PAFile fp;
#endif
    projCtx ctx;
    ctx = pj_get_default_ctx();
    fp = pj_open_lib(ctx, "conus", "rb");
    if (fp == NULL) exit(1);
#if PJ_VERSION == 480
    fclose(fp);
#else
    pj_ctx_fclose(ctx, fp);
#endif
    exit(0);
}
_EOCONF]

if test ${PROJ6} = "no"; then
 ${CC} ${CFLAGS} ${PKG_CPPFLAGS} -o proj_conf_test3 proj_conf_test3.c ${PKG_LIBS}
 if test [ -n "$proj_share_path" ] ; then
   PROJ_LIB="${proj_share_path}" ./proj_conf_test3
   proj_share=`echo $?`
 else
   ./proj_conf_test3
   proj_share=`echo $?`
 fi

 AC_MSG_CHECKING(PROJ.4: conus found and readable)
 if test ${proj_share} -eq 1 ; then
     WARN="warn"
     AC_MSG_RESULT(no)
 else
     AC_MSG_RESULT(yes)
 fi

 if test "$WARN" = "warn" ; then
     echo "Note: proj/conus not found"
     echo "No support available in PROJ4 for NAD grid datum transformations"
     echo "If required, consider re-installing from source with the contents"
     echo "of proj-datumgrid-1.<latest>.zip from http://download.osgeo.org/proj/ in nad/."
 fi
fi

rm -f proj_conf_test3.c proj_conf_test3

# Optional local copy of GDAL datadir and PROJ_LIB

data_copy=no
if test "${PROJ_GDAL_DATA_COPY}" ; then
    data_copy=yes
    AC_MSG_NOTICE(["PROJ_GDAL_DATA_COPY used."])
else
    AC_ARG_WITH([data-copy],
        AS_HELP_STRING([--with-data-copy=yes/no],[local copy of data directories in package, default no]),
               [data_copy=$withval])
fi
if test "${data_copy}" = "yes" ; then
AC_MSG_NOTICE([Copy data for:])
  proj_lib0="${PROJ_LIB}"
  AC_ARG_WITH([proj-data],
    AS_HELP_STRING([--with-proj-data=DIR],[location of PROJ.4 data directory]),
    [proj_lib1=$withval])
  if test -n "${proj_lib0}" ; then
    proj_lib="${proj_lib0}"
  else
    proj_lib="${proj_lib1}"
  fi
  if test -n "${proj_lib}" ; then
    if test -d "${proj_lib}" ; then
      cp -r "${proj_lib}" "${R_PACKAGE_DIR}"
      AC_MSG_NOTICE([  PROJ: ${proj_lib}])
    else
      AC_MSG_ERROR([PROJ.4 data files not found; set environment variable PROJ_LIB=DIR or --with-proj-data=DIR.])
    fi
  else
      AC_MSG_ERROR([PROJ.4 data files not found; set environment variable PROJ_LIB=DIR or --with-proj-data=DIR.])
  fi

  if test -d "${GDAL_DATADIR}" ; then
    cp -r "${GDAL_DATADIR}" "${R_PACKAGE_DIR}"
    AC_MSG_NOTICE([  GDAL: ${GDAL_DATADIR}])
  else
    AC_MSG_ERROR([GDAL data files not found.])
  fi
fi



#AC_MSG_NOTICE([CPP FLAGS: ${PKG_CPPFLAGS}])

# concluding substitution

AC_SUBST([PKG_CPPFLAGS], ["${INPKG_CPPFLAGS} ${GDAL_CPPFLAGS}"])
#AC_MSG_NOTICE([CPP FLAGS: ${PKG_CPPFLAGS}])
if test [ -n "$proj_include_path" ] ; then
#AC_MSG_NOTICE([INC_PATH: ${proj_include_path}])
AC_SUBST([PKG_CPPFLAGS], ["${PKG_CPPFLAGS} ${proj_include_path}"])
fi

if test ${PROJ6} = "yes" ; then
  if test ${PROJ_API} = "proj.h"; then
    AC_SUBST([PKG_CPPFLAGS], ["${PKG_CPPFLAGS} -DPROJ_H_API"])
  else
    AC_SUBST([PKG_CPPFLAGS], ["${PKG_CPPFLAGS} -DACCEPT_USE_OF_DEPRECATED_PROJ_API_H"])
  fi
fi

if test ${NEW_PROJ_WITH_OLD_GDAL} = "yes" ; then
  AC_SUBST([PKG_CPPFLAGS], ["${PKG_CPPFLAGS} -DNEW_PROJ_WITH_OLD_GDAL"])
fi

AC_SUBST([PKG_LIBS], ["${INPKG_LIBS} ${GDAL_LIBS}"])
if test "${NEED_DEPS}" = yes; then
   AC_SUBST([PKG_LIBS], ["${PKG_LIBS} ${GDAL_DEP_LIBS}"])
fi
if test [ -n "$proj_lib_path" ] ; then
AC_SUBST([PKG_LIBS], ["${PKG_LIBS} -L${proj_lib_path}"])
fi
if test [ -n "$PROJ_LIB_PATH" ] ; then
AC_SUBST([PKG_LIBS], ["${PKG_LIBS} ${PROJ_LIB_PATH}"])
else
AC_SUBST([PKG_LIBS], ["${PKG_LIBS} -lproj"])
fi
AC_MSG_NOTICE([Package CPP flags: ${PKG_CPPFLAGS}])
AC_MSG_NOTICE([Package LIBS: ${PKG_LIBS}])

AC_CONFIG_FILES(src/Makevars)
AC_OUTPUT
