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
#      This file is the GNU automake template for Nuovation's
#      utilities library.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

include_HEADERS                                      = \
    $(NULL)

lib_LIBRARIES                                        = \
    libNuovationsUtilities.a                           \
    $(NULL)

libNuovationsUtilities_a_CPPFLAGS                    = \
    -I$(srcdir)/repo/include                           \
    $(NULL)

libNuovationsUtilities_a_SOURCES                     = \
    repo/GenerateShortOptions.cpp                      \
    $(NULL)

PRETTY_FILES :=

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
