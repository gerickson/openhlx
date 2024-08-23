#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#
#    Copyright (c) 2019-2021 Grant Erickson
#    Copyright (c) 2016-2018 Nest Labs, Inc.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

#
#    Description:
#      This file is the GNU autoconf input source file for
#      Open HLX.
#

#
# Declare autoconf version requirements
#
AC_PREREQ([2.68])

#
# Initialize autoconf for the package
#
AC_INIT([openhlx],
        m4_esyscmd([third_party/nlbuild-autotools/repo/scripts/mkversion -b `cat .default-version` .]),
        [@PACKAGE_EMAIL@],
        [openhlx],
        [https://github.com/gerickson/openhlx])

# Tell the rest of the build system the absolute path where the
# nlbuild-autotools repository is rooted at.

AC_SUBST(nlbuild_autotools_stem,[third_party/nlbuild-autotools/repo])
AC_SUBST(abs_top_nlbuild_autotools_dir,[\${abs_top_srcdir}/\${nlbuild_autotools_stem}])

#
# OPENHLX interface current, revision, and age versions.
#
# Maintainters: Please manage these fields as follows:
#
#   Interfaces removed:    CURRENT++, AGE = 0, REVISION = 0
#   Interfaces added:      CURRENT++, AGE++,   REVISION = 0
#   No interfaces changed:                     REVISION++
#
#
AC_SUBST(LIBOPENHLX_VERSION_CURRENT,  [1])
AC_SUBST(LIBOPENHLX_VERSION_AGE,      [0])
AC_SUBST(LIBOPENHLX_VERSION_REVISION, [0])
AC_SUBST(LIBOPENHLX_VERSION_INFO,     [${LIBOPENHLX_VERSION_CURRENT}:${LIBOPENHLX_VERSION_REVISION}:${LIBOPENHLX_VERSION_AGE}])

#
# Check the sanity of the source directory by checking for the
# presence of a key watch file
#
AC_CONFIG_SRCDIR([src/hlxc/hlxc.cpp])

#
# Tell autoconf where to find auxilliary build tools (e.g. config.guess,
# install-sh, missing, etc.)
#
AC_CONFIG_AUX_DIR([third_party/nlbuild-autotools/repo/third_party/autoconf])

#
# Tell autoconf where to find auxilliary M4 macros
# Note that nlbuild-autotools has macros in two subdirs, for local vs third-party scripts
#
AC_CONFIG_MACRO_DIRS([third_party/nlbuild-autotools/repo/third_party/autoconf/m4 third_party/nlbuild-autotools/repo/autoconf/m4 third_party/cycript/repo/m4])

#
# Tell autoconf what file the package is using to aggregate C preprocessor
# defines.
#
AC_CONFIG_HEADERS([src/include/openhlx-config.h])

#
# Figure out what the canonical build, host and target tuples are.
#
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

#
# Mac OS X / Darwin ends up putting some versioning cruft on the end of its
# tuple that we don't care about in this script. Create "clean" variables
# devoid of it.
#

NL_FILTERED_CANONICAL_BUILD
NL_FILTERED_CANONICAL_HOST
NL_FILTERED_CANONICAL_TARGET

#
# Configure automake with the desired options, indicating that this is
# not a native GNU package, that we want "silent" build rules, that we
# want objects built in the same subdirectory as their source rather
# than collapsed together at the top-level directory, that we want
# support for the tar PAX format for 'make dist', that we do not want
# emission of 'PACKAGE' and 'VERSION' since AC_INIT already emits
# 'PACKAGE_NAME' and 'PACKAGE_VERSION', and that we want xz-compressed
# distribution archives in addition to gzip-compressed distribution
# archives.
#
# Disable silent build rules by either passing --disable-silent-rules to
# configure or passing V=1 to make
#
AM_INIT_AUTOMAKE([1.14 foreign silent-rules subdir-objects tar-pax no-define dist-xz])

#
# Silent build rules requires at least automake-1.11. Employ
# techniques for not breaking earlier versions of automake.
#
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AM_SILENT_RULES([yes])

#
# Disable maintainer mode to enable the package to automatically rebuild
# configure, Makefile.in, etc. when the files on which they depend change (for
# example, configure.ac, Makefile.am, etc).
#
# For those that do not desire this behavior, run configure with
# `--enable-maintainer-mode` and run the top-level `bootstrap` script manually.
#
AM_MAINTAINER_MODE([disable])

#
# Checks for build host programs
#

# If we are cross-compiling and we are on an embedded target that
# doesn't support independent, standalone executables, then all
# compiler tests that attempt to create an executable will fail. In
# such circumstances, set AC_NO_EXECUTABLES (see http://sourceware.org/
# ml/newlib/2006/msg00353.html).

AC_MSG_CHECKING([whether to disable executable checking])
if test "$cross_compiling" = yes; then
    AC_NO_EXECUTABLES
    AC_MSG_RESULT([yes])
else
    AC_MSG_RESULT([no])
fi

# Passing -Werror to GCC-based or -compatible compilers breaks some
# autoconf tests (see
# http://lists.gnu.org/archive/html/autoconf-patches/2008-09/msg00014.html).
#
# If -Werror has been passed transform it into -Wno-error. We'll
# transform it back later with NL_RESTORE_WERROR.

NL_SAVE_WERROR

# Check for compilers.
#
# These should be checked BEFORE we check for and, implicitly,
# initialize libtool such that libtool knows what languages it has to
# work with.

AC_PROG_CPP
AC_PROG_CPP_WERROR

AC_PROG_CC
AC_PROG_CC_C_O

AC_PROG_CXXCPP

AC_PROG_CXX
AC_PROG_CXX_C_O

# Check for other compiler toolchain tools.

AC_CHECK_TOOL(AR, ar)
AC_CHECK_TOOL(RANLIB, ranlib)
AC_CHECK_TOOL(OBJCOPY, objcopy)
AC_CHECK_TOOL(STRIP, strip)

# Check for other host tools.

AC_PROG_EGREP
AC_PROG_INSTALL
AC_PROG_LN_S

AC_PATH_PROG(CLANG_FORMAT, clang-format)
AC_PATH_PROG(CMP, cmp)
AC_PATH_PROG(PERL, perl)

#
# Checks for specific compiler characteristics
#

#
# Common compiler flags we would like to have.
#
#   -Wall                        CC, CXX
#   -Wconversion                 CC, CXX
#   -Wextra                      CC, CXX
#   -Wheader-hygiene             CC, CXX
#   -Wshadow                     CC, CXX
#   -Wunused                     CC, CXX
#

PROSPECTIVE_CFLAGS="-Wall -Wconversion -Wextra -Wheader-hygiene -Wshadow -Wunused"
PROSPECTIVE_CXXFLAGS=""

# Check whether the C++ compiler supports C++11. If it is supported,
# enable it. Otherwise, proceed without error or warning.

AX_CXX_COMPILE_STDCXX_11([], [optional])

AX_CHECK_COMPILER_OPTIONS([C],   ${PROSPECTIVE_CFLAGS})
AX_CHECK_COMPILER_OPTIONS([C++], ${PROSPECTIVE_CFLAGS} ${PROSPECTIVE_CXXFLAGS})

# Check for and initialize libtool

LT_INIT

#
# Debug instances
#
AC_MSG_NOTICE([checking whether to build debug instances])

# Debug

NL_ENABLE_DEBUG([no])

AM_CONDITIONAL([OPENHLX_BUILD_DEBUG], [test "${nl_cv_build_debug}" = "yes"])

#
# Code coverage and compiler optimization
#

# Coverage

NL_ENABLE_COVERAGE([no])

AM_CONDITIONAL([OPENHLX_BUILD_COVERAGE], [test "${nl_cv_build_coverage}" = "yes"])

NL_ENABLE_COVERAGE_REPORTS([auto])

AM_CONDITIONAL([OPENHLX_BUILD_COVERAGE_REPORTS], [test "${nl_cv_build_coverage_reports}" = "yes"])

# Optimization

NL_ENABLE_OPTIMIZATION([yes])

AM_CONDITIONAL([OPENHLX_BUILD_OPTIMIZED], [test "${nl_cv_build_optimized}" = "yes"])

#
# Treat Compiler Warnings as Errors
#
NL_ENABLE_WERROR([no])

#
# Tests
#
AC_MSG_NOTICE([checking whether to build tests])

# Tests

NL_ENABLE_TESTS([yes])

AM_CONDITIONAL([OPENHLX_BUILD_TESTS], [test "${nl_cv_build_tests}" = "yes"])

#
# Documentation
#

AC_PATH_PROG(ASCIIDOC, asciidoc)
AC_PATH_PROG(XMLTO, xmlto)

# Determine whether or not documentation (via Doxygen) should be built
# or not, with 'auto' as the default and establish a default support
# value for GraphViz 'dot' support.

NL_ENABLE_DOCS([auto],[NO])

AM_CONDITIONAL(OPENHLX_BUILD_DOCS, [test "${nl_cv_build_docs}" = "yes"])

# Default configuration file path for hlxsimd.

AC_ARG_WITH(default-hlxsimd-configuration-file,
    [AS_HELP_STRING([--with-default-hlxsimd-configuration-file=PATH],
        [Specify the default hlxsimd configuration file path @<:@default=${localstatedir}/hlxsimd/hlxsimd.plist@:>@.])],
    [
    ],
    [with_default_hlxsimd_configuration_file=${localstatedir}/hlxsimd/hlxsimd.plist])

AC_SUBST(HLXSIMD_DEFAULT_CONFIG_PATH, [${with_default_hlxsimd_configuration_file}])

# Check for the source of CoreFoundation, whether from a system
# framework or from [Open]CFLite.

CFUTILITIES_CF_SOURCE=
CFUTILITIES_CF_SOURCE_FRAMEWORK=0
CFUTILITIES_CF_SOURCE_CFLITE=0

AC_MSG_CHECKING([for CoreFoundation source])

AC_ARG_WITH(corefoundation-source,
    [AS_HELP_STRING([--with-corefoundation-source=SOURCE],
        [Specify the CoreFoundation source from one of: auto, framework, or cflite @<:@default=auto@:>@.])],
    [
        case "${with_corefoundation_source}" in

        auto|framework|cflite)
            ;;

        *)
            AC_MSG_ERROR([Invalid value ${with_corefoundation_source} for --with-corefoundation-source])
            ;;

        esac
    ],
    [with_corefoundation_source=auto])

# At this point, the CoreFoundation source is one of the allowed
# values. If it's 'auto' we autodetect it, based on the target OS.

if test "${with_corefoundation_source}" = "auto"; then
    case ${target_os} in

	*darwin*)
            with_corefoundation_source=framework
	    ;;

	*)
            with_corefoundation_source=cflite
	    ;;

    esac
fi

AC_MSG_RESULT(${with_corefoundation_source})

#
# Checks for libraries and packages.
#
# At minimum, the following packages are required:
#
#   * Boost
#   * CFUtilities
#   * LogUtilities
#   * libtelnet
#   * nlassert
#
# The following packages are optional, depending on configuration:
#
#   * libnl (Netlink Protocol Library Suite)
#   * nlunit-test
#   * [Open]CFLite
#   * TRE
#
AC_MSG_NOTICE([checking required package dependencies])

# Check if the build host has pkg-config

AC_PATH_PROG([PKG_CONFIG],[pkg-config])

#
# Boost
#

NL_WITH_REQUIRED_EXTERNAL_PACKAGE([Boost],
[BOOST],
[boost],
[],
[
    # Check for required Boost (C++) headers.

    AC_LANG_PUSH([C++])

        AC_CHECK_HEADERS([boost/filesystem.hpp] [boost/foreach.hpp],
            [],
            [
                AC_MSG_ERROR(The Boost header "$ac_header" is required but cannot be found.)
            ]
            )

    AC_LANG_POP
]
)

#
# CFUtilities
#

NL_WITH_REQUIRED_INTERNAL_PACKAGE(
    [CFUtilities],
    [CFUTILITIES],
    [cfutilities],
    [-lCFUtilities],
    [
        # At this point, the internal CFUtilities package will be neither
        # configured nor built, so the normal checks we undertake for an
        # external package cannot be run here. Simply set the appropriate
        # variables and trust all will be well.

        CFUTILITIES_CPPFLAGS="-I\${abs_top_srcdir}/third_party/CFUtilities/repo"
        CFUTILITIES_LDFLAGS=
        CFUTILITIES_LIBS=
    ],
    [
        # Check for required CFUtilities headers.

        AC_CHECK_HEADERS([CFUtilities/CFString.hpp] [CFUtilities/CFUtilities.h] [CFUtilities/CFUtilities.hpp],
        [],
        [
            AC_MSG_ERROR(The CFUtilities header "$ac_header" is required but cannot be found.)
        ])
    ]
)

# Depending on whether CFUtilities has been configured for an internal
# location, its directory stem within this package needs to be set
# accordingly. In addition, if the location is internal, then we need
# to attempt to pull it down using the bootstrap makefile.

if test "${nl_with_cfutilities}" = "internal"; then
    maybe_cfutilities_dirstem="CFUtilities/repo"
    cfutilities_dirstem="third_party/${maybe_cfutilities_dirstem}"

    AC_MSG_NOTICE([attempting to create internal ${cfutilities_dirstem}])

    ${MAKE-make} --no-print-directory -C ${srcdir} -f Makefile-bootstrap ${cfutilities_dirstem}

    if test $? -ne 0; then
        AC_MSG_ERROR([failed to create ${cfutilities_dirstem}. Please check your network connection or the correctness of 'repos.conf'])
    fi
else
    maybe_cfutilities_dirstem=""
fi

AC_SUBST(CFUTILITIES_SUBDIRS, [${maybe_cfutilities_dirstem}])
AM_CONDITIONAL([OPENHLX_WITH_CFUTILITIES_INTERNAL], [test "${nl_with_cfutilities}" = "internal"])

#
# LogUtilities
#

NL_WITH_REQUIRED_INTERNAL_PACKAGE(
    [LogUtilities],
    [LOGUTILITIES],
    [logutilities],
    [-lLogUtilities],
    [
        # At this point, the internal LogUtilities package will be neither
        # configured nor built, so the normal checks we undertake for an
        # external package cannot be run here. Simply set the appropriate
        # variables and trust all will be well.

        LOGUTILITIES_CPPFLAGS="-I\${abs_top_srcdir}/third_party/LogUtilities/repo"
        LOGUTILITIES_LDFLAGS=
        LOGUTILITIES_LIBS=
    ],
    [
        # Check for required LogUtilities headers.

        AC_CHECK_HEADERS([LogUtilities/LogUtilities.hpp],
        [],
        [
            AC_MSG_ERROR(The LogUtilities header "$ac_header" is required but cannot be found.)
        ])
    ]
)

# Depending on whether LogUtilities has been configured for an internal
# location, its directory stem within this package needs to be set
# accordingly. In addition, if the location is internal, then we need
# to attempt to pull it down using the bootstrap makefile.

if test "${nl_with_logutilities}" = "internal"; then
    maybe_logutilities_dirstem="LogUtilities/repo"
    logutilities_dirstem="third_party/${maybe_logutilities_dirstem}"

    AC_MSG_NOTICE([attempting to create internal ${logutilities_dirstem}])

    ${MAKE-make} --no-print-directory -C ${srcdir} -f Makefile-bootstrap ${logutilities_dirstem}

    if test $? -ne 0; then
        AC_MSG_ERROR([failed to create ${logutilities_dirstem}. Please check your network connection or the correctness of 'repos.conf'])
    fi
else
    maybe_logutilities_dirstem=""
fi

AC_SUBST(LOGUTILITIES_SUBDIRS, [${maybe_logutilities_dirstem}])
AM_CONDITIONAL([OPENHLX_WITH_LOGUTILITIES_INTERNAL], [test "${nl_with_logutilities}" = "internal"])

#
# Libnl
#

case ${nl_filtered_target_os} in

    *linux*)
        NL_WITH_REQUIRED_EXTERNAL_PACKAGE([libnl],
            [LIBNL],
            [libnl],
            [-lnl-3 -lnl-route-3],
            [
                # Check for required libnl and libnl-route library headers.

                AC_LANG_PUSH([C])

                    AC_CHECK_HEADERS([netlink/addr.h] [netlink/cache.h] [netlink/socket.h] [netlink/route/route.h],
                        [],
                        [
                            AC_MSG_ERROR(The libnl header "$ac_header" is required but cannot be found.)
                        ]
                        )

               AC_LANG_POP
            ]
        )
        ;;

    *)
        ;;

esac

#
# Libtelnet
#

NL_WITH_REQUIRED_INTERNAL_PACKAGE(
    [Libtelnet],
    [LIBTELNET],
    [libtelnet],
    [],
    [
        # At this point, the internal libtelnet package will be neither
        # configured nor built, so the normal checks we undertake for an
        # external package cannot be run here. Simply set the appropriate
        # variables and trust all will be well.

        LIBTELNET_CPPFLAGS="-I\${abs_top_srcdir}/third_party/libtelnet/repo"
        LIBTELNET_LDFLAGS=
        LIBTELNET_LIBS=
    ],
    [
        # Check for required Libtelnet headers.

        AC_CHECK_HEADERS([libtelnet.h],
        [],
        [
            AC_MSG_ERROR(The libtelnet header "$ac_header" is required but cannot be found.)
        ])
    ]
)

# Depending on whether libtelnet has been configured for an internal
# location, its directory stem within this package needs to be set
# accordingly. In addition, if the location is internal, then we need
# to attempt to pull it down using the bootstrap makefile.

if test "${nl_with_libtelnet}" = "internal"; then
    maybe_libtelnet_dirstem="libtelnet/repo"
    libtelnet_dirstem="third_party/${maybe_libtelnet_dirstem}"

    AC_MSG_NOTICE([attempting to create internal ${libtelnet_dirstem}])

    ${MAKE-make} --no-print-directory -C ${srcdir} -f Makefile-bootstrap ${libtelnet_dirstem}

    if test $? -ne 0; then
        AC_MSG_ERROR([failed to create ${libtelnet_dirstem}. Please check your network connection or the correctness of 'repos.conf'])
    fi
else
    maybe_libtelnet_dirstem=""
fi

AC_SUBST(LIBTELNET_SUBDIRS, [${maybe_libtelnet_dirstem}])
AM_CONDITIONAL([OPENHLX_WITH_LIBTELNET_INTERNAL], [test "${nl_with_libtelnet}" = "internal"])

#
# Nlassert
#

NL_WITH_REQUIRED_INTERNAL_PACKAGE(
    [Nlassert],
    [NLASSERT],
    [nlassert],
    [],
    [
        # At this point, the internal Nlassert package will be neither
        # configured nor built, so the normal checks we undertake for an
        # external package cannot be run here. Simply set the appropriate
        # variables and trust all will be well.

        NLASSERT_CPPFLAGS="-I\${abs_top_srcdir}/third_party/nlassert/repo/include"
        NLASSERT_LDFLAGS=
        NLASSERT_LIBS=
    ],
    [
        # Check for required Nlassert headers.

        AC_CHECK_HEADERS([nlassert.h],
        [],
        [
            AC_MSG_ERROR(The nlassert header "$ac_header" is required but cannot be found.)
        ])
    ]
)

# Depending on whether nlassert has been configured for an internal
# location, its directory stem within this package needs to be set
# accordingly. In addition, if the location is internal, then we need
# to attempt to pull it down using the bootstrap makefile.

if test "${nl_with_nlassert}" = "internal"; then
    maybe_nlassert_dirstem="nlassert/repo"
    nlassert_dirstem="third_party/${maybe_nlassert_dirstem}"

    AC_MSG_NOTICE([attempting to create internal ${nlassert_dirstem}])

    ${MAKE-make} --no-print-directory -C ${srcdir} -f Makefile-bootstrap ${nlassert_dirstem}

    if test $? -ne 0; then
        AC_MSG_ERROR([failed to create ${nlassert_dirstem}. Please check your network connection or the correctness of 'repos.conf'])
    fi
else
    maybe_nlassert_dirstem=""
fi

AC_SUBST(NLASSERT_SUBDIRS, [${maybe_nlassert_dirstem}])
AM_CONDITIONAL([OPENHLX_WITH_NLASSERT_INTERNAL], [test "${nl_with_nlassert}" = "internal"])

#
# Nlunit-test
#

if test "${nl_cv_build_tests}" = "yes"; then
    NL_WITH_REQUIRED_INTERNAL_PACKAGE(
        [Nlunit-test],
        [NLUNIT_TEST],
        [nlunit_test],
        [-lnlunit-test],
        [
	    # At this point, the internal Nlunit-test package will be neither
	    # configured nor built, so the normal checks we undertake for an
	    # external package cannot be run here. Simply set the appropriate
	    # variables and trust all will be well.

            NLUNIT_TEST_CPPFLAGS="-I\${abs_top_srcdir}/third_party/nlunit-test/repo/src"
            NLUNIT_TEST_LDFLAGS="-L${ac_pwd}/third_party/nlunit-test/repo/src"
            NLUNIT_TEST_LIBS="-lnlunit-test"
        ],
        [
            # Check for required nlunit-test headers.

            AC_CHECK_HEADERS([nlunit-test.h],
            [],
            [
                AC_MSG_ERROR(The nlunit-test header "$ac_header" is required but cannot be found.)
            ])
        ])
fi

# Depending on whether nlunit-test has been configured for an internal
# location, its directory stem within this package needs to be set
# accordingly. In addition, if the location is internal, then we need
# to attempt to pull it down using the bootstrap makefile.

if test "${nl_with_nlunit_test}" = "internal"; then
    maybe_nlunit_test_dirstem="nlunit-test/repo"
    nlunit_test_dirstem="third_party/${maybe_nlunit_test_dirstem}"

    AC_MSG_NOTICE([attempting to create internal ${nlunit_test_dirstem}])

    ${MAKE-make} --no-print-directory -C ${srcdir} -f Makefile-bootstrap ${nlunit_test_dirstem}

    if test $? -ne 0; then
        AC_MSG_ERROR([failed to create ${nlunit_test_dirstem}. Please check your network connection or the correctness of 'repos.conf'])
    fi
else
    maybe_nlunit_test_dirstem=""
fi

AC_SUBST(NLUNIT_TEST_SUBDIRS, [${maybe_nlunit_test_dirstem}])
AM_CONDITIONAL([OPENHLX_WITH_NLUNIT_TEST_INTERNAL], [test "${nl_with_nlunit_test}" = "internal"])

#
# [Open]CFLite
#

if test "${with_corefoundation_source}" == "cflite"; then

    NL_WITH_REQUIRED_EXTERNAL_PACKAGE([CoreFoundation],
    [CF],
    [CoreFoundation],
    [-lCoreFoundation],
    [
        # Check for required CoreFoundation (via [Open]CFLite) headers.

        AC_LANG_PUSH([C])

            AC_CHECK_HEADERS([CoreFoundation/CoreFoundation.h],
                [],
                [
                    AC_MSG_ERROR(The CoreFoundation header "$ac_header" is required but cannot be found.)
                ]
                )

        AC_LANG_POP
    ]
    )

fi

#
# TRE
#
# On Apple-defined and -supported platforms (that is, iOS/iPadOS/macOS/
# tvOS/watchOS), the platform-supplied regex.h defines regnexec as a
# Darwin-specific extension available since macOS 10.8 and iOS 6.0,
# that is needed by this regular expression library to perform
# regular expression matches on the non-null-terminated text-based
# HLX protocol.
#
# Otherwise, regnexec is supplied by the TRE regular expression
# library.
#

if test "${ac_no_link}" != "yes"; then
    AC_CHECK_FUNCS([regnexec])
fi

if test "${ac_cv_func_regnexec}" = "no"; then

    NL_WITH_REQUIRED_EXTERNAL_PACKAGE([TRE],
    [TRE],
    [tre],
    [-ltre],
    [
        # Check for required TRE regular express library headers.

        AC_LANG_PUSH([C])

            AC_CHECK_HEADERS([tre/regex.h],
                [],
                [
                    AC_MSG_ERROR(The TRE header "$ac_header" is required but cannot be found.)
                ]
                )

        AC_LANG_POP
    ]
    )

fi

# At this point, the CoreFoundation source is one of the allowed
# values and, if [Open]CFLite, we have detected the location of a
# package providing it. If it's 'auto' we autodetect it.

if test "${with_corefoundation_source}" = "auto"; then

    AC_CHECK_FRAMEWORK(CoreFoundation,
                       [CFAllocatorGetDefault],
                       [with_corefoundation_source="framework"
                       LIBS="${LIBS} -framework CoreFoundation"],
                       AC_CHECK_LIB(CoreFoundation,
                                    CFAllocatorGetDefault,
                                    [with_corefoundation_source="cflite"],
                                    with_corefoundation_source="none",))

elif test "${with_corefoundation_source}" = "framework"; then

    AC_CHECK_FRAMEWORK(CoreFoundation,
                       [CFAllocatorGetDefault],
                       [LIBS="${LIBS} -framework CoreFoundation"],
                       with_corefoundation_source="none")

elif test "${with_corefoundation_source}" = "cflite"; then

    AC_CHECK_LIB(CoreFoundation,
                 CFAllocatorGetDefault,
                 [],
                 with_corefoundation_source="none",)

fi

if test "${with_corefoundation_source}" = "none"; then

    AC_MSG_ERROR([Cannot determine a source for CoreFoundation.])

fi

OPENHLX_CF_SOURCE=${with_corefoundation_source}

case ${with_corefoundation_source} in

    framework)
        OPENHLX_CF_SOURCE_FRAMEWORK=1
        ;;

    cflite)
        OPENHLX_CF_SOURCE_CFLITE=1
        ;;

esac

AC_SUBST(OPENHLX_CF_SOURCE)
AC_DEFINE_UNQUOTED([OPENHLX_CF_SOURCE],[${OPENHLX_CF_SOURCE}],[CoreFoundation source])

AC_SUBST(OPENHLX_CF_SOURCE_FRAMEWORK)
AM_CONDITIONAL([OPENHLX_CF_SOURCE_FRAMEWORK], [test "${OPENHLX_CF_SOURCE}" = "framework"])
AC_DEFINE_UNQUOTED([OPENHLX_CF_SOURCE_FRAMEWORK],[${OPENHLX_CF_SOURCE_FRAMEWORK}],[Define to 1 if you want to use a system framework implementation source of CoreFoundation for Open HLX])

AC_SUBST(OPENHLX_CF_SOURCE_CFLITE)
AM_CONDITIONAL([OPENHLX_CF_SOURCE_CFLITE], [test "${OPENHLX_CF_SOURCE}" = "cflite"])
AC_DEFINE_UNQUOTED([OPENHLX_CF_SOURCE_CFLITE],[${OPENHLX_CF_SOURCE_CFLITE}],[Define to 1 if you want to use an [Open]CFLite implementation source of CoreFoundation for Open HLX])

#
# Check for headers
#
AC_HEADER_STDBOOL

AC_CHECK_HEADERS([regex.h])
AC_CHECK_HEADERS([stdint.h])
AC_CHECK_HEADERS([string.h])

#
# Check for types and structures
#
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

#
# Checks for library functions
#

if test "${ac_no_link}" != "yes"; then
    AC_CHECK_FUNCS([memcpy])
fi

# Add any Boost CPPFLAGS, LDFLAGS, and LIBS

CPPFLAGS="${CPPFLAGS} ${BOOST_CPPFLAGS}"
LIBS="${LIBS} ${BOOST_LIBS}"
LDFLAGS="${LDFLAGS} ${BOOST_LDFLAGS}"

# Add any CoreFoundation CPPFLAGS, LDFLAGS, and LIBS

CPPFLAGS="${CPPFLAGS} ${CF_CPPFLAGS}"
LIBS="${LIBS} ${CF_LIBS}"
LDFLAGS="${LDFLAGS} ${CF_LDFLAGS}"

# Add any CFUtilities CPPFLAGS, LDFLAGS, and LIBS

CPPFLAGS="${CPPFLAGS} ${CFUTILITIES_CPPFLAGS}"
LDFLAGS="${LDFLAGS} ${CFUTILITIES_LDFLAGS}"
LIBS="${LIBS} ${CFUTILITIES_LIBS}"

# Add any LogUtilities CPPFLAGS, LDFLAGS, and LIBS

CPPFLAGS="${CPPFLAGS} ${LOGUTILITIES_CPPFLAGS}"
LDFLAGS="${LDFLAGS} ${LOGUTILITIES_LDFLAGS}"
LIBS="${LIBS} ${LOGUTILITIES_LIBS}"

# Add any libnl CPPFLAGS, LDFLAGS, and LIBS

CPPFLAGS="${CPPFLAGS} ${LIBNL_CPPFLAGS}"
LDFLAGS="${LDFLAGS} ${LIBNL_LDFLAGS}"
LIBS="${LIBS} ${LIBNL_LIBS}"

# Add any libtelnet CPPFLAGS, LDFLAGS, and LIBS

CPPFLAGS="${CPPFLAGS} ${LIBTELNET_CPPFLAGS}"
LDFLAGS="${LDFLAGS} ${LIBTELNET_LDFLAGS}"
LIBS="${LIBS} ${LIBTELNET_LIBS}"

# Add any nlassert CPPFLAGS, LDFLAGS, and LIBS

CPPFLAGS="${CPPFLAGS} ${NLASSERT_CPPFLAGS}"
LDFLAGS="${LDFLAGS} ${NLASSERT_LDFLAGS}"
LIBS="${LIBS} ${NLASSERT_LIBS}"

# Add any nlunit-test CPPFLAGS, LDFLAGS, and LIBS

CPPFLAGS="${CPPFLAGS} ${NLUNIT_TEST_CPPFLAGS}"
LDFLAGS="${LDFLAGS} ${NLUNIT_TEST_LDFLAGS}"
LIBS="${LIBS} ${NLUNIT_TEST_LIBS}"

# Add any TRE CPPFLAGS, LDFLAGS, and LIBS

CPPFLAGS="${CPPFLAGS} ${TRE_CPPFLAGS}"
LDFLAGS="${LDFLAGS} ${TRE_LDFLAGS}"
LIBS="${LIBS} ${TRE_LIBS}"

# Add any code coverage CPPFLAGS, LDFLAGS, and LIBS

CPPFLAGS="${CPPFLAGS} ${NL_COVERAGE_CPPFLAGS}"
LDFLAGS="${LDFLAGS} ${NL_COVERAGE_LDFLAGS}"
LIBS="${LIBS} ${NL_COVERAGE_LIBS}"

# Fold in the warning-as-errors flag, unless it was already in effect.

if test "${nl_had_CPPFLAGS_werror}" = "no"; then
    CPPFLAGS="${CPPFLAGS} ${NL_WERROR_CPPFLAGS}"
fi

# At this point, we can restore the compiler flags to whatever the
# user passed in, now that we're clear of any -Werror issues by
# transforming -Wno-error back to -Werror.

NL_RESTORE_WERROR

# Coding Style
#
# Establish what tool to use and what arguments to pass for the
# pretty and pretty-check targets.

AC_SUBST(PRETTY, ["\${CLANG_FORMAT}"])
AC_SUBST(PRETTY_ARGS, ["\$(shell cat \${abs_top_srcdir}/build/clang-format/clang-format-pretty.options)"])

AC_SUBST(PRETTY_CHECK, ["\${PERL} \${abs_top_srcdir}/third_party/cstyle/repo/cstyle.pl"])
AC_SUBST(PRETTY_CHECK_ARGS, ["\$(shell cat \${abs_top_srcdir}/build/cstyle/cstyle.options)"])

# Configure any autotools-based subdirectories

if test "${nl_with_cfutilities}" = "internal"; then
    AC_CONFIG_SUBDIRS([third_party/CFUtilities/repo])
fi

if test "${nl_with_logutilities}" = "internal"; then
    AC_CONFIG_SUBDIRS([third_party/LogUtilities/repo])
fi

if test "${nl_with_nlassert}" = "internal"; then
    AC_CONFIG_SUBDIRS([third_party/nlassert/repo])
fi

if test "${nl_with_nlunit_test}" = "internal"; then
    AC_CONFIG_SUBDIRS([third_party/nlunit-test/repo])
fi

#
# Identify the various makefiles and auto-generated files for the package
#
AC_CONFIG_FILES([
Makefile
third_party/Makefile
third_party/NuovationsUtilities/Makefile
third_party/libtelnet/Makefile
src/Makefile
src/hlxc/Makefile
src/hlxproxyd/Makefile
src/hlxsimd/Makefile
src/include/Makefile
src/lib/Makefile
src/lib/client/Makefile
src/lib/client/tests/Makefile
src/lib/common/Makefile
src/lib/common/tests/Makefile
src/lib/model/Makefile
src/lib/model/tests/Makefile
src/lib/server/Makefile
src/lib/server/tests/Makefile
src/lib/utilities/Makefile
src/lib/utilities/tests/Makefile
tests/Makefile
doc/Makefile
doc/man/Makefile
contrib/systemd/Makefile
])

#
# Generate the auto-generated files for the package
#
AC_OUTPUT

#
# Summarize the package configuration
#

AC_MSG_NOTICE([

  Configuration Summary
  ---------------------
  Package                                   : ${PACKAGE_NAME}
  Version                                   : ${PACKAGE_VERSION}
  Interface                                 : ${LIBOPENHLX_VERSION_INFO//:/.}
  Build system                              : ${build}
  Host system                               : ${host}
  Target system                             : ${target}
  Target architecture                       : ${target_cpu}
  Target OS                                 : ${target_os}
  Filtered target system                    : ${nl_filtered_target}
  Filtered target architecture              : ${nl_filtered_target_cpu}
  Filtered target OS                        : ${nl_filtered_target_os}
  Cross compiling                           : ${cross_compiling}
  Build shared libraries                    : ${enable_shared}
  Build static libraries                    : ${enable_static}
  Build debug libraries                     : ${nl_cv_build_debug}
  Build optimized libraries                 : ${nl_cv_build_optimized}
  Build coverage libraries                  : ${nl_cv_build_coverage}
  Build coverage reports                    : ${nl_cv_build_coverage_reports}
  Lcov                                      : ${LCOV:--}
  Genhtml                                   : ${GENHTML:--}
  Build tests                               : ${nl_cv_build_tests}
  Prefix                                    : ${prefix}
  Shadow directory program                  : ${LNDIR}
  Documentation support                     : ${nl_cv_build_docs}
  Asciidoc                                  : ${ASCIIDOC:--}
  Xmlto                                     : ${XMLTO:--}
  Doxygen                                   : ${DOXYGEN:--}
  GraphViz dot                              : ${DOT:--}
  PERL                                      : ${PERL:--}
  Hlxsimd default configuration file        : ${with_default_hlxsimd_configuration_file}
  CFUtilities source                        : ${nl_with_cfutilities:--}
  CFUtilities compile flags                 : ${CFUTILITIES_CPPFLAGS:--}
  CFUtilities link flags                    : ${CFUTILITIES_LDFLAGS:--}
  CFUtilities link libraries                : ${CFUTILITIES_LIBS:--}
  CoreFoundation source                     : ${OPENHLX_CF_SOURCE:--}
  CoreFoundation compile flags              : ${CF_CPPFLAGS:--}
  CoreFoundation link flags                 : ${CF_LDFLAGS:--}
  CoreFoundation link libraries             : ${CF_LIBS:--}
  Libnl compile flags                       : ${LIBNL_CPPFLAGS:--}
  Libnl link flags                          : ${LIBNL_LDFLAGS:--}
  Libnl link libraries                      : ${LIBNL_LIBS:--}
  Libtelnet source                          : ${nl_with_libtelnet:--}
  Libtelnet compile flags                   : ${LIBTELNET_CPPFLAGS:--}
  Libtelnet link flags                      : ${LIBTELNET_LDFLAGS:--}
  Libtelnet link libraries                  : ${LIBTELNET_LIBS:--}
  LogUtilities source                       : ${nl_with_logutilities:--}
  LogUtilities compile flags                : ${LOGUTILITIES_CPPFLAGS:--}
  LogUtilities link flags                   : ${LOGUTILITIES_LDFLAGS:--}
  LogUtilities link libraries               : ${LOGUTILITIES_LIBS:--}
  Nlassert source                           : ${nl_with_nlassert:--}
  Nlassert compile flags                    : ${NLASSERT_CPPFLAGS:--}
  Nlassert link flags                       : ${NLASSERT_LDFLAGS:--}
  Nlassert link libraries                   : ${NLASSERT_LIBS:--}
  Nlunit-test source                        : ${nl_with_nlunit_test:--}
  Nlunit-test compile flags                 : ${NLUNIT_TEST_CPPFLAGS:--}
  Nlunit-test link flags                    : ${NLUNIT_TEST_LDFLAGS:--}
  Nlunit-test link libraries                : ${NLUNIT_TEST_LIBS:--}
  TRE compile flags                         : ${TRE_CPPFLAGS:--}
  TRE link flags                            : ${TRE_LDFLAGS:--}
  TRE link libraries                        : ${TRE_LIBS:--}
  C Preprocessor                            : ${CPP}
  C Compiler                                : ${CC}
  C++ Preprocessor                          : ${CXXCPP}
  C++ Compiler                              : ${CXX}
  Archiver                                  : ${AR}
  Archive Indexer                           : ${RANLIB}
  Symbol Stripper                           : ${STRIP}
  Object Copier                             : ${OBJCOPY}
  C Preprocessor flags                      : ${CPPFLAGS:--}
  C Compile flags                           : ${CFLAGS:--}
  C++ Compile flags                         : ${CXXFLAGS:--}
  Link flags                                : ${LDFLAGS:--}
  Link libraries                            : ${LIBS}

])
