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
#      This file is the GNU automake template for the Open HLX public
#      headers.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

#
# Create a hierarchy of linked directories to expose public headers
# under the paths they will be publicly exported as.
#

OpenHLXHeaderDir                  := OpenHLX

OpenHLXHeaderLinks                := \
    $(OpenHLXHeaderDir)/Client       \
    $(OpenHLXHeaderDir)/Common       \
    $(OpenHLXHeaderDir)/Model        \
    $(OpenHLXHeaderDir)/Utilities    \
    $(NULL)

OpenHLXVersionFile                := \
    $(OpenHLXHeaderDir)/Version.h    \
    $(NULL)

BUILT_SOURCES                     := \
    $(OpenHLXHeaderLinks)            \
    $(OpenHLXVersionFile)            \
    $(NULL)

CLEANFILES                        := \
    $(OpenHLXHeaderLinks)            \
    $(OpenHLXVersionFile)            \
    $(NULL)

define check-file-Version.h
$(top_srcdir)/build/scripts/gen-openhlx-version "`cat $(1)`" -o "$(2)"
endef # check-file-Version.h

$(OpenHLXHeaderDir): | $(builddir)
	$(call nl-create-dir,$(@))

$(top_builddir)/.local-version:
	$(NL_V_PROGRESS) "MAKE" "$(@)"
	+$(NL_V_AT)$(MAKE) $(MFLAGS) -C "$(@D)" "$(@F)"

$(OpenHLXVersionFile): $(top_builddir)/.local-version | $(OpenHLXHeaderDir)
	$(call check-file,Version.h)

$(OpenHLXHeaderDir)/Client: $(abs_top_srcdir)/src/lib/client | $(OpenHLXHeaderDir)
	$(call nl-create-link)

$(OpenHLXHeaderDir)/Common: $(abs_top_srcdir)/src/lib/common | $(OpenHLXHeaderDir)
	$(call nl-create-link)

$(OpenHLXHeaderDir)/Model: $(abs_top_srcdir)/src/lib/model | $(OpenHLXHeaderDir)
	$(call nl-create-link)

$(OpenHLXHeaderDir)/Utilities: $(abs_top_srcdir)/src/lib/utilities | $(OpenHLXHeaderDir)
	$(call nl-create-link)

# The 'install' target directory transform. Headers in
# openhlx_include_HEADERS will get installed in openhlx_includedir.

openhlx_includedir                              = \
    ${includedir}/${OpenHLXHeaderDir}

# Public library headers to distribute and install.

openhlx_include_HEADERS                         = \
    Version.h                                     \
    $(NULL)

clean-local:
	-$(NL_V_AT)$(RM) -r -f $(OpenHLXHeaderDir)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
