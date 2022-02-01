#
#    Copyright 2022 Grant Erickson
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
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
#      systemd service makefile.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

EXTRA_DIST                         = \
    $(srcdir)/default                \
    $(srcdir)/hlxproxyd.service.in   \
    $(NULL)

CLEANFILES                         = \
    hlxproxyd.service                \
    $(NULL)

servicedir                         = $(libdir)/systemd/system
service_DATA                       = hlxproxyd.service

#
# We choose to manually transform hlxproxyd.service.in into
# hlxproxyd.service here in the makefile rather than in the configure
# script so that we can take advantage of live, at build time, updating
# of the file system paths.
#

hlxproxyd.service: $(srcdir)/hlxproxyd.service.in Makefile
	$(AM_V_GEN)$(SED)                                     \
	    -e "s,\@sbindir\@,$(sbindir),g"                   \
	    -e "s,\@sysconfdir\@,$(sysconfdir),g"             \
	    < "$(srcdir)/hlxproxyd.service.in" > "$(@)"

#
# We have a bespoke install rule for the defaults file since automake
# does not have any canned way to install a file with a different
# destination name than its source name.
#

install-data-local: $(DESTDIR)$(sysconfdir)/default/hlxproxyd

$(DESTDIR)$(sysconfdir)/default:
	@echo " $(MKDIR_P) '$(@)'"
	$(AM_V_at)$(MKDIR_P) "$(@)"

$(DESTDIR)$(sysconfdir)/default/hlxproxyd: $(srcdir)/default | $(DESTDIR)$(sysconfdir)/default
	@echo " $(INSTALL_DATA) $(<) '$(@)'"
	$(AM_V_at)$(INSTALL_DATA) "$(<)" "$(@)"

uninstall-local:
	@echo " rm -f '$(DESTDIR)$(sysconfdir)/default/hlxproxyd'"
	-$(AM_V_at)rm -f $(DESTDIR)$(sysconfdir)/default/hlxproxyd

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
