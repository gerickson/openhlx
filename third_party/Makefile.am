#
#    Copyright (c) 2021 Grant Erickson. All Rights Reserved.
#    Copyright 2018 Nest Labs Inc. All Rights Reserved.
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
#      This file is the GNU automake template for the Open HLX
#      third-party software directory.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

EXTRA_DIST                        = \
    cstyle                          \
    libtelnet/Makefile.am           \
    nlbuild-autotools               \
    $(NULL)

# Always package (e.g. for 'make dist') these subdirectories.

DIST_SUBDIRS                      = \
    NuovationsUtilities             \
    $(NULL)

# <any of the third-party packages listed in repos.conf>_SUBDIRS are
# not a permanent part of DIST_SUBDIRS since we do not ever want to
# include them in a distribution archive; however, when they've been
# pulled as a 'repo' module, we do want to remove them on invocation
# of the 'distclean' target. Consequently, we conditionally include
# them in DIST_SUBDIRS on invocation of 'distclean-recursive'

distclean-recursive: DIST_SUBDIRS += $(CFUTILITIES_SUBDIRS) $(LIBTELNET_SUBDIRS) $(LOGUTILITIES_SUBDIRS) $(NLASSERT_SUBDIRS) $(NLUNIT_TEST_SUBDIRS)

# Always build (e.g. for 'make all') these subdirectories.
#
# The values of <any of the third-party packages listed in
# repos.conf>_SUBDIRS will be populated by configure if --with-<any of
# the third-party packages listed in repos.conf>=internal

SUBDIRS                           = \
    $(CFUTILITIES_SUBDIRS)          \
    $(LOGUTILITIES_SUBDIRS)         \
    NuovationsUtilities             \
    libtelnet                       \
    $(NLASSERT_SUBDIRS)             \
    $(NLUNIT_TEST_SUBDIRS)          \
    $(NULL)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
