#
#    Copyright (c) 2021 Grant Erickson
#    All rights reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing,
#    software distributed under the License is distributed on an "AS
#    IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
#    express or implied.  See the License for the specific language
#    governing permissions and limitations under the License.
#

#
#    Description:
#      This file is the makefile for the OpenHLX server library.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

lib_LIBRARIES                                               = \
    libopenhlx-server.a                                       \
    $(NULL)

# Ensure that libraries are built before trying to run any recursive
# targets from SUBDIRS that might depend on them.

AM_RECURSIVE_TARGETS: $(LIBRARIES)

noinst_HEADERS                                              = \
    $(NULL)

# The 'install' target directory transform. Headers in
# libopenhlx_server_a_include_HEADERS will get installed in
# libopenhlx_server_a_includedir.

libopenhlx_server_a_includedir                              = \
    ${includedir}/OpenHLX/Server

# Public library headers to distribute and install.

libopenhlx_server_a_include_HEADERS                         = \
    $(NULL)

libopenhlx_server_a_CPPFLAGS                                = \
    -I$(top_srcdir)/third_party/CFUtilities/repo/include      \
    -I$(top_srcdir)/third_party/LogUtilities/repo/include     \
    -I$(top_srcdir)/third_party/libtelnet/repo                \
    -I$(top_srcdir)/src/lib/common                            \
    -I$(top_srcdir)/src/lib/utilities                         \
    $(NULL)

libopenhlx_server_a_SOURCES                                 = \
    $(NULL)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
