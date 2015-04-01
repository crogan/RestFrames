AC_INIT([RestFrames], [1.0.0])
AC_PREREQ(2.59)
AC_CONFIG_SRCDIR(src/RestFrame.cc)
AM_INIT_AUTOMAKE(tar-ustar)

AM_MAINTAINER_MODE([disable])

AC_CONFIG_HEADERS(inc/RestFrames/RestFrames_config.h:config/config.h.in)

AC_PROG_CXX

dnl make sure there is a C++ compiler
cxx_progname=$(basename $CXX)
cxx_dirname=$(dirname $CXX)
AC_CHECK_PROG([cxx_found],[$cxx_progname],[yes],[no],[$cxx_dirname$PATH_SEPARATOR$PATH])
if test "x$cxx_found" == "xno"; then
   AC_MSG_ERROR([No C++ compiler found])
fi

dnl set CXXFLAGS if user doesn't
if [[ "x$CXXFLAGS" == "x" ]]; then
   CXXFLAGS="-O2 -Wall"
fi

dnl auotomake CXX flags
AM_CXXFLAGS=""

dnl allow for debug option to configure
AC_ARG_ENABLE(debug,
	[  --enable-debug          Turn on debug compiler information [[default=yes]]],
	[ENABLE_DEBUG_FLAG="$enableval"],
	[ENABLE_DEBUG_FLAG="yes"])
if [[ "$ENABLE_DEBUG_FLAG" == "yes" ]] ; then
   AM_CXXFLAGS=${AM_CXXFLAGS}" -g "

   dnl check for -Woverloaded-virtual support
   AC_MSG_CHECKING([if $CXX supports -Woverloaded-virtual flag])
   SAVE_CXXFLAGS="$CXXFLAGS"
   CXXFLAGS="-Woverloaded-virtual"
   AC_LANG_PUSH(C++)
   AC_COMPILE_IFELSE([AC_LANG_SOURCE([int main(void){ return 0;}])],
       [ac_overloaded_virtual_ok=yes],[ac_overloaded_virtual_ok=no])
   AC_LANG_POP(C++)
   CXXFLAGS="$SAVE_CXXFLAGS"
   AC_MSG_RESULT([$ac_overloaded_virtual_ok])
   if [[ "x${ac_overloaded_virtual_ok}" == "xyes" ]]; then
      AM_CXXFLAGS=${AM_CXXFLAGS}" -Woverloaded-virtual "
   fi
fi

dnl check if one wants a high level of warnings
AC_ARG_ENABLE(extra-warnings,
	[  --enable-extra-warnings Turn on additional compiler warnings [[default=no]]],
	[ENABLE_EXTRA_WARNINGS_FLAG="$enableval"],
	[ENABLE_EXTRA_WARNINGS_FLAG="no"])
if [[ "$ENABLE_EXTRA_WARNINGS_FLAG" == "yes" ]] ; then
   AM_CXXFLAGS=${AM_CXXFLAGS}" -ansi -pedantic -Wextra -Wshadow "
fi

AC_SUBST(AM_CXXFLAGS)

dnl flags for restframes-config
AC_CHECK_LIB(m, cos)
CONFIG_CXXFLAGS=""
CONFIG_LIBS="-lm"
AC_SUBST(CONFIG_CXXFLAGS)
AC_SUBST(CONFIG_LIBS)

AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

dnl add ROOT info 
ROOT_PATH(,[
        AC_DEFINE([HAVE_ROOT],,[Root library])
        AC_CHECK_LIB([dl],[dlopen],[],AC_MSG_ERROR([Cannot find complete ROOT installation]))
        ],AC_MSG_RESULT([RestFrames will be not compiled]))
AM_CONDITIONAL(EXISTROOT,test x$no_root = "x")
dnl check for rootcling (ROOT v > 6)
AC_PATH_PROG(ROOTCLING, rootcling, no)
if test ! x"$ROOTCLING" = "xno"; then
   ROOTCINT=$ROOTCLING
fi
AM_CONDITIONAL(EXISTROOTCLING,test ! x"$ROOTCLING" = "xno")

dnl define SOVERSION for shared library labeling
SOVERSION=`echo $VERSION | sed 's/\(.*\)\.\(.*\)\.\(.*\)/\1:\2:0/'`
AC_SUBST(SOVERSION)

AC_CONFIG_FILES( Makefile src/Makefile inc/Makefile inc/RestFrames/Makefile examples/Makefile config/Makefile)

CONFIG_SUMMARY=""
CONFIG_SUMMARY+="Configuration summary:\n"
CONFIG_SUMMARY+="---------------------------------------------------------------\n"
if [[ "x${prefix}" == "xNONE" ]] ; then
CONFIG_SUMMARY+="  Installation directory     "${default_prefix}"\n"
else
CONFIG_SUMMARY+="  Installation directory     "${prefix}"\n"
fi
AC_SUBST(CONFIG_SUMMARY)

CONFIGURE_INVOCATION="$0 $ac_configure_args"
AC_SUBST(CONFIGURE_INVOCATION)

dnl-------------------------------------------------------------------
dnl finish with a nice message
dnl-------------------------------------------------------------------
AC_OUTPUT
printf "$CONFIG_SUMMARY"
cat 1>&2 << 'EOF'
---------------------------------------------------------------
              INFORMATION FOR RestFrames USERS                 
                                                               
 ---           
                                                
 When using the RestFrames library, you can retrieve           
 compilation and link flags using restframes-config.           
 Type 'restframes-config --help' for more information          
 
 ---                                                           
 
 The script setup_RestFrames.sh(csh) is provided to            
 automatically set environemtal variables like PATH,           
 LD_LIBRARY_PATH, etc. in order to find this RestFrames        
 installation. Sourcing this script (from any directory) will
 set the environmental variables to the values matching the
 RestFrames build that created the script. Users can source this 
 script automatically at their terminal login by adding to their                       "
 .bashrc/.profile/.zlogin(.cshrc) file the line:               
    . $(restframes-config --prefix)/libexec/setup_RestFrames.sh 
 for bash-type shells, or                                      
    . $(restframes-config --prefix)/libexec/setup_RestFrames.csh
 for csh-type (and assuming the restframes-config executable is in 
 their default PATH)                                            
---------------------------------------------------------------
EOF
