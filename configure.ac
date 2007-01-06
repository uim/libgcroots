# Copyright (c) 1999-2001 by Red Hat, Inc. All rights reserved.
# 
# THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
# OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
# 
# Permission is hereby granted to use or copy this program
# for any purpose,  provided the above notices are retained on all copies.
# Permission to modify the code and to distribute modified code is granted,
# provided the above notices are retained, and a notice that the code was
# modified is included with the above copyright notice.
#
# Original author: Tom Tromey
# Modified by: Grzegorz Jakacki <jakacki at acm dot org>
# Modified by: YAMAMOTO Kengo <yamaken AT bp.iij4u.or.jp> (2006-12, libgcroots)

dnl Process this file with autoconf to produce configure.

# Initialization
# ==============

dnl AC_INIT(gc,7.0alpha7,Hans.Boehm@hp.com) 
dnl     ## version must conform to [0-9]+[.][0-9]+(alpha[0-9]+)?
dnl AC_CONFIG_SRCDIR(gcj_mlc.c)
AC_INIT([gcroots], [0.1.3], [uim@freedesktop.org], [libgcroots]) 
AC_CONFIG_SRCDIR(include/private/gcroots_priv.h)
AC_CANONICAL_TARGET 
AC_PREREQ(2.60b)
dnl AC_REVISION($Revision: 1.14 $)
dnl GC_SET_VERSION
AM_INIT_AUTOMAKE([foreign dist-bzip2 subdir-objects nostdinc])
AM_MAINTAINER_MODE

AC_SUBST(PACKAGE)
dnl AC_SUBST(GC_VERSION)

AM_PROG_CC_C_O
dnl AC_PROG_CXX

AM_PROG_AS
## FIXME: really needed? (AC_LIBTOOL already provides this)
AC_CHECK_TOOL(AR, ar)
AC_CHECK_TOOL(RANLIB, ranlib, :)  # :)

AC_PROG_INSTALL
AC_PATH_PROGS(MD5, md5 md5sum)
AC_PATH_PROGS(SHA1, sha1 sha1sum)

. ${srcdir}/configure.host

GC_CFLAGS=${gc_cflags}
AC_SUBST(GC_CFLAGS)

# FIXME: depending on the internal variable name
AC_DEFUN([AX_LANG_WNOERROR], [ac_[]_AC_LANG_ABBREV[]_werror_flag=])

AC_LANG_WERROR  # Turn warning-only unknown options into error.

AX_CFLAGS_GCC_OPTION([-pedantic])
AX_CFLAGS_GCC_OPTION([-Wall])
AX_CFLAGS_GCC_OPTION([-Wchar-subscripts])
AX_CFLAGS_GCC_OPTION([-Wmissing-declarations])
AX_CFLAGS_GCC_OPTION([-Wredundant-decls])
AX_CFLAGS_GCC_OPTION([-Wmissing-prototypes])
AX_CFLAGS_GCC_OPTION([-Wnested-externs])
AX_CFLAGS_GCC_OPTION([-Wpointer-arith])
AX_CFLAGS_GCC_OPTION([-Wcast-align])
AX_CFLAGS_GCC_OPTION([-Wsign-compare])

# Disable the problematic preprocessing on Mac OS X
AX_CFLAGS_GCC_OPTION([-no-cpp-precomp])

AX_LANG_WNOERROR  # end AC_LANG_WERROR

dnl AC_ARG_ENABLE(threads,
dnl   [AC_HELP_STRING([--enable-threads=TYPE], [choose threading package])],
dnl   THREADS=$enableval,
dnl   [ AC_MSG_CHECKING([for thread model used by GCC])
dnl     THREADS=`$CC -v 2>&1 | sed -n 's/^Thread model: //p'`
dnl     if test -z "$THREADS"; then
dnl       THREADS=no
dnl     fi
dnl     AC_MSG_RESULT([$THREADS]) ])
enable_threads=no
THREADS=no

dnl AC_ARG_ENABLE(parallel-mark,
dnl    [AC_HELP_STRING([--enable-parallel-mark],
dnl 	[parallelize marking and free list construction])],
dnl    [case "$THREADS" in
dnl       no | none | single)
dnl 	AC_MSG_ERROR([Parallel mark requires --enable-threads=x spec])
dnl 	;;
dnl     esac ]
dnl )
enable_parallel_mark=no

dnl AC_ARG_ENABLE(cplusplus,
dnl     [AC_HELP_STRING([--enable-cplusplus], [install C++ support])])
enable_cplusplus=no

INCLUDES=-I${srcdir}/include
THREADDLLIBS=
need_atomic_ops_asm=false
## Libraries needed to support dynamic loading and/or threads.
case "$THREADS" in
 no | none | single)
    THREADS=none
    ;;
 posix | pthreads)
    THREADS=posix
    THREADDLLIBS=-lpthread
    case "$host" in
     x86-*-linux* | ia64-*-linux* | i586-*-linux* | i686-*-linux* | x86_64-*-linux* | alpha-*-linux*)
	AC_DEFINE(GC_LINUX_THREADS)
	AC_DEFINE(_REENTRANT)
        if test "${enable_parallel_mark}" = yes; then
	  AC_DEFINE(PARALLEL_MARK)
	fi
	AC_DEFINE(THREAD_LOCAL_ALLOC)
	AC_MSG_WARN("Explict GC_INIT() calls may be required.");
	;;
     *-*-linux*)
	AC_DEFINE(GC_LINUX_THREADS)
	AC_DEFINE(_REENTRANT)
	;;
     *-*-aix*)
	AC_DEFINE(GC_AIX_THREADS)
	AC_DEFINE(_REENTRANT)
	;;
     *-*-hpux11*)
	AC_MSG_WARN("Only HP/UX 11 POSIX threads are supported.")
	AC_DEFINE(GC_HPUX_THREADS)
	AC_DEFINE(_POSIX_C_SOURCE,199506L)
	if test "${enable_parallel_mark}" = yes; then
	  AC_DEFINE(PARALLEL_MARK)
	fi
	AC_DEFINE(THREAD_LOCAL_ALLOC)
	AC_MSG_WARN("Explict GC_INIT() calls may be required.");
	THREADDLLIBS="-lpthread -lrt"
	# HPUX needs REENTRANT for the _r calls.
	AC_DEFINE(_REENTRANT, 1, [Required define if using POSIX threads])
	;;
     *-*-hpux10*)
	AC_MSG_WARN("Only HP-UX 11 POSIX threads are supported.")
	;;
     *-*-freebsd*)
	AC_MSG_WARN("FreeBSD does not yet fully support threads with Boehm GC.")
	AC_DEFINE(GC_FREEBSD_THREADS)
	INCLUDES="$INCLUDES -pthread"
      	;;
     *-*-kfreebsd*-gnu)
	AC_DEFINE(GC_FREEBSD_THREADS)
	INCLUDES="$INCLUDES -pthread"
	THREADDLLIBS=-pthread
	AC_DEFINE(_REENTRANT)
        if test "${enable_parallel_mark}" = yes; then
	  AC_DEFINE(PARALLEL_MARK)
	fi
	AC_DEFINE(THREAD_LOCAL_ALLOC)
	AC_DEFINE(USE_COMPILER_TLS)
      	;;
     *-*-netbsd*)
	AC_MSG_WARN("Only on NetBSD 2.0 or later.")
	AC_DEFINE(GC_NETBSD_THREADS)
	AC_DEFINE(_REENTRANT)
	AC_DEFINE(_PTHREADS)
	THREADDLLIBS="-lpthread -lrt"
	;;
     *-*-solaris*)
	AC_DEFINE(GC_SOLARIS_THREADS)
	AC_DEFINE(THREAD_LOCAL_ALLOC)
	THREADDLLIBS="-lpthread -lrt"
	if test "$GCC" != yes; then
          CFLAGS="$CFLAGS -O"
          need_atomic_ops_asm=true
        fi
	;;
     *-*-irix*)
	AC_DEFINE(GC_IRIX_THREADS)
	;;
     *-*-cygwin*)
	AC_DEFINE(GC_WIN32_THREADS)
	win32_threads=true
	;;
     *-*-darwin*)
	AC_DEFINE(GC_DARWIN_THREADS)
	AC_DEFINE(THREAD_LOCAL_ALLOC)
	AC_MSG_WARN("Explict GC_INIT() calls may be required.");
	if test "${enable_parallel_mark}" = yes; then
	  AC_DEFINE(PARALLEL_MARK)
	fi
	darwin_threads=true
	;;
     *-*-osf*)
	AC_DEFINE(GC_OSF1_THREADS)
        if test "${enable_parallel_mark}" = yes; then
	  AC_DEFINE(PARALLEL_MARK)
	  AC_DEFINE(THREAD_LOCAL_ALLOC)
	  AC_MSG_WARN("Explict GC_INIT() calls may be required.");
	  # May want to enable it in other cases, too.
	  # Measurements havent yet been done.
	fi
	INCLUDES="$INCLUDES -pthread"
	THREADDLLIBS="-lpthread -lrt"
	;;
      *)
	AC_MSG_ERROR("Pthreads not supported by the GC on this platform.")
	;;
    esac
    ;;
 win32)
    AC_DEFINE(GC_WIN32_THREADS)
    dnl Wine getenv may not return NULL for missing entry
    AC_DEFINE(NO_GETENV)
    ;;
 dgux386)
    THREADS=dgux386
    AC_MSG_RESULT($THREADDLLIBS)
    # Use pthread GCC  switch
    THREADDLLIBS=-pthread
    if test "${enable_parallel_mark}" = yes; then
        AC_DEFINE(PARALLEL_MARK)
    fi
    AC_DEFINE(THREAD_LOCAL_ALLOC)
    AC_MSG_WARN("Explict GC_INIT() calls may be required.");
    AC_DEFINE(GC_DGUX386_THREADS)
    AC_DEFINE(DGUX_THREADS)
    # Enable _POSIX4A_DRAFT10_SOURCE with flag -pthread
    INCLUDES="-pthread $INCLUDES"
    ;;
 aix)
    THREADS=posix
    THREADDLLIBS=-lpthread
    AC_DEFINE(GC_AIX_THREADS)
    AC_DEFINE(_REENTRANT)
    ;;
 decosf1 | irix | mach | os2 | solaris | dce | vxworks)
    AC_MSG_ERROR(thread package $THREADS not yet supported)
    ;;
 *)
    AC_MSG_ERROR($THREADS is an unknown thread package)
    ;;
esac
AC_SUBST(THREADDLLIBS)
AM_CONDITIONAL(THREADS, test x$THREADS != xnone)
AM_CONDITIONAL(PTHREADS, test x$THREADS = xposix)
AM_CONDITIONAL(DARWIN_THREADS, test x$darwin_threads = xtrue)
AM_CONDITIONAL(WIN32_THREADS, test x$win32_threads = xtrue)

case "$host" in 
   powerpc-*-darwin*)
      powerpc_darwin=true
      ;;
esac

AC_MSG_CHECKING(for xlc)
AC_TRY_COMPILE([],[
 #ifndef __xlC__
 # error
 #endif
], [compiler_xlc=yes], [compiler_xlc=no])
AC_MSG_RESULT($compiler_xlc)
AM_CONDITIONAL(COMPILER_XLC,test $compiler_xlc = yes)
if test $compiler_xlc = yes -a "$powerpc_darwin" = true; then
  # the darwin stack-frame-walking code is completely broken on xlc
  AC_DEFINE(DARWIN_DONT_PARSE_STACK)
fi

# We never want libdl on darwin. It is a fake libdl that just ends up making
# dyld calls anyway
case "$host" in
  *-*-darwin*) ;;
  *) 
    AC_CHECK_LIB(dl, dlopen, THREADDLLIBS="$THREADDLLIBS -ldl")
    ;;
esac

AC_SUBST(EXTRA_TEST_LIBS)

dnl target_all=libgc.la
target_all=libgcroots.la
AC_SUBST(target_all)

dnl If the target is an eCos system, use the appropriate eCos
dnl I/O routines.
dnl FIXME: this should not be a local option but a global target
dnl system; at present there is no eCos target.
TARGET_ECOS="no"
AC_ARG_WITH(ecos,
[  --with-ecos             enable runtime eCos target support],
TARGET_ECOS="$with_ecos"
)

addobjs=
addlibs=
CXXINCLUDES=
case "$TARGET_ECOS" in
   no)
      ;;
   *)
      AC_DEFINE(ECOS)
      CXXINCLUDES="-I${TARGET_ECOS}/include"
      addobjs="$addobjs ecos.lo"
      ;;
esac

AM_CONDITIONAL(CPLUSPLUS, test "${enable_cplusplus}" = yes)

AC_SUBST(CXX)

AC_SUBST(INCLUDES)
AC_SUBST(CXXINCLUDES)

AX_FUNC_GETCONTEXT

# Configuration of shared libraries
#
AC_MSG_CHECKING(whether to build shared libraries)
AC_ENABLE_SHARED

case "$host" in
 alpha-*-openbsd*)
     enable_shared=no
     AC_MSG_RESULT(no)
     ;;
 *)
     AC_MSG_RESULT(yes)
     ;;
esac

# Configuration of machine-dependent code
#
AC_MSG_CHECKING(which machine-dependent code should be used) 
machdep=
case "$host" in
 alpha-*-openbsd*)
    machdep="mach_dep.lo"
    if test x"${ac_cv_lib_dl_dlopen}" != xyes ; then
       AC_MSG_WARN(OpenBSD/Alpha without dlopen(). Shared library support is disabled)
    fi
    ;;
 alpha*-*-linux*)
    machdep="mach_dep.lo"
    ;;
 i?86-*-solaris2.[[89]] | i?86-*-solaris2.1?)
    AC_DEFINE(SOLARIS25_PROC_VDB_BUG_FIXED)
    ;;
 mipstx39-*-elf*)
    machdep="mach_dep.lo"
    AC_DEFINE(STACKBASE, __stackbase)
    AC_DEFINE(DATASTART_IS_ETEXT)
    ;;
 mips-dec-ultrix*)
    machdep="mach-dep.lo"
    ;;
 mips-nec-sysv*|mips-unknown-sysv*)
    ;;
 mips*-*-linux*) 
    ;; 
 mips-*-*)
    machdep="mach_dep.lo"
    AC_DEFINE(NO_EXECUTE_PERMISSION)
    ;;
 # should be replaced with sparc_mach_dep.lo?
 sparc-*-netbsd*)
    machdep="mach_dep.lo sparc_netbsd_mach_dep.lo"
    ;;
 sparc-sun-solaris2.3)
    machdep="mach_dep.lo sparc_mach_dep.lo"
    AC_DEFINE(SUNOS53_SHARED_LIB)
    ;;
 sparc*-sun-solaris2.*)
    machdep="mach_dep.lo sparc_mach_dep.lo"
    ;;
 sparc*-*-linux*)
    machdep="mach_dep.lo sparc_mach_dep.lo"
    ;;
 sparc*-*-*)
    machdep="mach_dep.lo sparc_mach_dep.lo"
    ;;
 ia64-*-*)
    machdep="mach_dep.lo ia64_save_regs_in_stack.lo"
    ;;
esac
if test x"$machdep" = x; then
AC_MSG_RESULT($machdep)
   machdep="mach_dep.lo"
fi
addobjs="$addobjs $machdep"
AC_SUBST(addobjs)
AC_SUBST(addlibs)

AC_PROG_LIBTOOL

#
# Check for AViiON Machines running DGUX
#
ac_is_dgux=no
AC_CHECK_HEADER(sys/dg_sys_info.h,
[ac_is_dgux=yes;])

    ## :GOTCHA: we do not check anything but sys/dg_sys_info.h
if test $ac_is_dgux = yes; then
    if test "$enable_full_debug" = "yes"; then
      CFLAGS="-g -mstandard -DDGUX -D_DGUX_SOURCE -Di386 -mno-legend -O2"
      CXXFLAGS="-g -mstandard -DDGUX -D_DGUX_SOURCE -Di386 -mno-legend -O2"
    else
      CFLAGS="-DDGUX -D_DGUX_SOURCE -Di386 -mno-legend -O2"
      CXXFLAGS="-DDGUX -D_DGUX_SOURCE -Di386 -mno-legend -O2"
    fi
    AC_SUBST(CFLAGS)
    AC_SUBST(CXXFLAGS)
fi

dnl We use these options to decide which functions to include.
AC_ARG_WITH(target-subdir,
[  --with-target-subdir=SUBDIR
                          configuring with a cross compiler])
AC_ARG_WITH(cross-host,
[  --with-cross-host=HOST  configuring with a cross compiler])

# automake wants to see AC_EXEEXT.  But we don't need it.  And having
# it is actually a problem, because the compiler we're passed can't
# necessarily do a full link.  So we fool automake here.
if false; then
  # autoconf 2.50 runs AC_EXEEXT by default, and the macro expands
  # to nothing, so nothing would remain between `then' and `fi' if it
  # were not for the `:' below.
  :
  AC_EXEEXT
fi

dnl As of 4.13a2, the collector will not properly work on Solaris when
dnl built with gcc and -O.  So we remove -O in the appropriate case.
dnl Not needed anymore on Solaris.
AC_MSG_CHECKING(whether Solaris gcc optimization fix is necessary)
case "$host" in
 *aix*)
    if test "$GCC" = yes; then
       AC_MSG_RESULT(yes)
       new_CFLAGS=
       for i in $CFLAGS; do
	  case "$i" in
	   -O*)
	      ;;
	   *)
	      new_CFLAGS="$new_CFLAGS $i"
	      ;;
	  esac
       done
       CFLAGS="$new_CFLAGS"
    else
       AC_MSG_RESULT(no)
    fi
    ;;
 *) AC_MSG_RESULT(no) ;;
esac

dnl Include defines that have become de facto standard.
dnl ALL_INTERIOR_POINTERS can be overridden in startup code.
AC_DEFINE(NO_EXECUTE_PERMISSION)
AC_DEFINE(ALL_INTERIOR_POINTERS)


dnl Interface Selection
dnl -------------------
dnl
dnl By default, make the library as general as possible.
dnl enable_gcj_support=no
dnl AC_ARG_ENABLE(gcj-support,
dnl     [AC_HELP_STRING([--disable-gcj-support],
dnl 	[Disable support for gcj.])])
enable_gcj_support=no
AM_CONDITIONAL(ENABLE_GCJ_SUPPORT,
    [test x"$enable_gcj_support" != xno])
if test x"$enable_gcj_support" != xno; then
    AC_DEFINE(GC_GCJ_SUPPORT, 1, [Define to include support for gcj])
fi

dnl AC_ARG_ENABLE(java-finalization,
dnl     [AC_HELP_STRING([--disable-java-finalization],
dnl 	[Disable support for java finalization.])])
enable_java_finalization=no
if test x"$enable_java_finalization" != xno; then
    AC_DEFINE(JAVA_FINALIZATION)
fi

dnl AC_ARG_ENABLE(atomic-uncollectable,
dnl     [AC_HELP_STRING([--disable-atomic-uncollectible],
dnl 	[Disable support for atomic uncollectible allocation.])])
enable_atomic_uncollectible=no
if test x"$enable_atomic_uncollectible" != x"no"; then
    AC_DEFINE(ATOMIC_UNCOLLECTABLE, 1,
	[Define to enable atomic uncollectible allocation.])
fi

dnl AC_ARG_ENABLE(redirect-malloc,
dnl     [AC_HELP_STRING([--enable-redirect-malloc],
dnl 	[Redirect malloc and friends to GC routines])])
enable_redirect_malloc=no

if test "${enable_redirect_malloc}" = yes; then
    if test "${enable_full_debug}" = yes; then
	AC_DEFINE(REDIRECT_MALLOC, GC_debug_malloc_replacement)
	AC_DEFINE(REDIRECT_REALLOC, GC_debug_realloc_replacement)
	AC_DEFINE(REDIRECT_FREE, GC_debug_free)
    else
	AC_DEFINE(REDIRECT_MALLOC, GC_malloc)
    fi
fi

dnl AC_ARG_ENABLE(large-config,
dnl     [AC_HELP_STRING([--enable-large-config],
dnl 	[Optimize for large (> 100 MB) heap or root set])])
enable_large_config=no

if test "${enable_large_config}" = yes; then
    AC_DEFINE(LARGE_CONFIG, 1, [Define to optimize for large heaps or root sets])
fi

dnl This is something of a hack.  When cross-compiling we turn off
dnl some functionality.  We also enable the "small" configuration.
dnl These is only correct when targetting an embedded system.  FIXME.
if test -n "${with_cross_host}"; then
   AC_DEFINE(NO_CLOCK)
   AC_DEFINE(SMALL_CONFIG)
   AC_DEFINE(NO_DEBUGGING)
fi


dnl Debugging
dnl ---------

UNWINDLIBS=
dnl AC_ARG_ENABLE(gc-debug,
dnl [AC_HELP_STRING([--enable-gc-debug],
dnl     [include full support for pointer backtracing etc.])],
dnl [ if test "$enable_gc_debug" = "yes"; then
dnl     AC_MSG_WARN("Should define GC_DEBUG and use debug alloc. in clients.")
dnl     AC_DEFINE(KEEP_BACK_PTRS)
dnl     AC_DEFINE(DBG_HDRS_ALL)
dnl     case $host in
dnl       ia64-*-linux* )
dnl 	AC_DEFINE(MAKE_BACK_GRAPH)
dnl 	AC_DEFINE(SAVE_CALL_COUNT, 8)
dnl         AC_CHECK_LIB(unwind, backtrace, [
dnl 	  AC_DEFINE(GC_HAVE_BUILTIN_BACKTRACE)
dnl 	  UNWINDLIBS=-lunwind
dnl 	  AC_MSG_WARN("Client code may need to link against libunwind.")
dnl 	])
dnl       ;;
dnl       x86-*-linux* | i586-*-linux* | i686-*-linux* | x86_64-*-linux* )
dnl 	AC_DEFINE(MAKE_BACK_GRAPH)
dnl 	AC_MSG_WARN("Client must not use -fomit-frame-pointer.")
dnl 	AC_DEFINE(SAVE_CALL_COUNT, 8)
dnl       ;;
dnl       i[3456]86-*-dgux*)
dnl 	AC_DEFINE(MAKE_BACK_GRAPH)
dnl       ;;
dnl     esac ]
dnl   fi)
enable_gc_debug=no

AC_SUBST(UNWINDLIBS)

AC_ARG_ENABLE(gc-assertions,
    [AC_HELP_STRING([--enable-gc-assertions],
	[collector-internal assertion checking])])
if test "${enable_gc_assertions}" = yes; then
    AC_DEFINE(GC_ASSERTIONS)
fi

AM_CONDITIONAL(USE_LIBDIR, test -z "$with_cross_host")


dnl Atomic Ops
dnl ----------

atomic_ops_libs=-latomic_ops
dnl AC_CHECK_HEADER(atomic_ops.h,
dnl   [ AC_MSG_NOTICE([Using pre-installed libatomic_ops]) ],
dnl   [ ao_dir=
dnl     for candidate in ${srcdir}/libatomic_ops*; do
dnl 	case $candidate in
dnl 	    *.tar.gz)
dnl 		;;
dnl 	    *install)
dnl 	    	dnl generated by alternate Makefile.
dnl 		;;
dnl 	    *)
dnl 		if test -e "$candidate"; then
dnl 		    ao_dir="$candidate"
dnl 		fi
dnl 		;;
dnl 	esac
dnl     done
dnl     if test -z "$ao_dir"; then
dnl 	AC_MSG_ERROR([Missig libatomic_ops.])
dnl     fi
dnl     ao_version="${ao_dir#*libatomic_ops-}"
dnl     AC_MSG_NOTICE([Using internal version of libatomic_ops])
dnl 
dnl     dnl Automake does not accept shell variables in AC_CONFIG_SUBDIRS
dnl     test -e ${srcdir}/libatomic_ops \
dnl 	|| ln -s ${ao_dir} ${srcdir}/libatomic_ops
dnl     AC_CONFIG_SUBDIRS(libatomic_ops)
dnl 
dnl     dnl Also copy the source files to be linked in.
dnl     test -e ${srcdir}/atomic_ops.c \
dnl     	|| ln -s ${srcdir}/libatomic_ops/src/atomic_ops.c \
dnl 		 ${srcdir}/atomic_ops.c
dnl 
dnl     test -e ${srcdir}/atomic_ops_sysdeps.S \
dnl     	|| ln -s ${srcdir}/libatomic_ops/src/atomic_ops_sysdeps.S \
dnl 		 ${srcdir}/atomic_ops_sysdeps.S
dnl 
dnl     dnl This gets the source include files, which is often close enough.
dnl     dnl It also makes atomic_ops_sysdeps.S assemble.
dnl     GC_CFLAGS="${GC_CFLAGS} -I \$(top_srcdir)/libatomic_ops/src"
dnl     maybe_libatomic_ops="libatomic_ops"
dnl   ])

AM_CONDITIONAL(USE_INTERNAL_LIBATOMIC_OPS,
		test -n "$maybe_libatomic_ops" -a "$THREADS" != "none")
AM_CONDITIONAL(NEED_ATOMIC_OPS_ASM, test -n "$maybe_libatomic_ops" -a x$need_atomic_ops_asm = xtrue)
AC_SUBST(atomic_ops_libs)

dnl Produce the Files
dnl -----------------

dnl AC_CONFIG_FILES([Makefile bdw-gc.pc])
AC_CONFIG_FILES([Makefile gcroots.pc m4/Makefile])

AC_CONFIG_COMMANDS([default],,
  [ srcdir=${srcdir}
    host=${host}
    CONFIG_SHELL=${CONFIG_SHELL-/bin/sh}
    CC="${CC}"
    DEFS="$DEFS" ])

AC_OUTPUT
