#
#    Copyright (c) 2019-2021 Grant Erickson
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
#      This file is the GNU automake template for the HLX command line
#      interface (CLI) control client program executable.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

bin_PROGRAMS                                                               = \
    hlxc                                                                     \
    $(NULL)

hlxc_CPPFLAGS                                                              = \
    -I$(top_srcdir)/third_party/CFUtilities/repo/include                     \
    -I$(top_srcdir)/third_party/LogUtilities/repo/include                    \
    -I$(top_srcdir)/third_party/NuovationsUtilities/repo/include             \
    -I$(top_srcdir)/src/lib/client                                           \
    -I$(top_srcdir)/src/lib/common                                           \
    -I$(top_srcdir)/src/lib/model                                            \
    -I$(top_srcdir)/src/lib/utilities                                        \
    $(NULL)

hlxc_LDADD                                                                 = \
    $(top_builddir)/src/lib/client/libopenhlx-client.a                       \
    $(top_builddir)/src/lib/common/libopenhlx-common.a                       \
    $(top_builddir)/src/lib/model/libopenhlx-model.a                         \
    $(top_builddir)/src/lib/utilities/libopenhlx-utilities.a                 \
    $(top_builddir)/third_party/CFUtilities/repo/src/libCFUtilities.la       \
    $(top_builddir)/third_party/LogUtilities/repo/src/libLogUtilities.la     \
    $(top_builddir)/third_party/NuovationsUtilities/libNuovationsUtilities.a \
    $(top_builddir)/third_party/libtelnet/libtelnet.a                        \
    $(NULL)

hlxc_LDFLAGS                                                               = \
    -framework CoreFoundation                                                \
    -lboost_system                                                           \
    -lboost_filesystem                                                       \
    $(NULL)

hlxc_SOURCES                                                               = \
    hlxc.cpp                                                                 \
    $(NULL)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
