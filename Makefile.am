#
#    Copyright 2019-2021 Grant Erickson. All Rights Reserved.
#    Copyright 2016-2018 Nest Labs, Inc. All Rights Reserved.
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
#      This file is the GNU automake template for the Open HLX.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

AM_MAKEFLAGS                                     = --no-print-directory

SUBDIRS                                          = \
    third_party                                    \
    src                                            \
    tests                                          \
    doc                                            \
    $(NULL)

EXTRA_DIST                                       = \
    Makefile-bootstrap                             \
    .clang-format                                  \
    .default-version                               \
    .github/workflows/build.yml                    \
    bootstrap                                      \
    bootstrap-configure                            \
    build/clang-format/clang-format-pretty.options \
    build/cstyle/cstyle.options                    \
    repos.conf                                     \
    README.md                                      \
    TODO.md                                        \
    $(NULL)

BUILT_SOURCES                                    = \
    .local-version                                 \
    $(NULL)

dist_doc_DATA                                    = \
    CHANGES.md                                     \
    LICENSE                                        \
    $(NULL)

DISTCLEANFILES                                   = \
    .local-version                                 \
    $(NULL)

#
# Package version files:
#
# .default-version - The default package version. This file is ALWAYS checked
#                    in and should always represent the current baseline
#                    version of the package.
#
# .dist-version    - The distributed package version. This file is NEVER
#                    checked in within the upstream repository, is auto-
#                    generated, and is only found in the package distribution.
#
# .local-version   - The current source code controlled package version. This
#                    file is NEVER checked in within the upstream repository,
#                    is auto-generated, and can always be found in both the
#                    build tree and distribution.
#
# When present, the .local-version file is preferred first, the
# .dist-version second, and the .default-version last.
#

# VERSION_FILE should be and is intentionally an immediate (:=) rather
# than a deferred (=) variable to ensure the value binds once and only once
# for a given MAKELEVEL even as .local-version and .dist-version are created
# during makefile execution.

VERSION_FILE                      := $(if $(wildcard $(builddir)/.local-version),$(builddir)/.local-version,$(if $(wildcard $(srcdir)/.dist-version),$(srcdir)/.dist-version,$(srcdir)/.default-version))

#
# Override autotool's default notion of the package version variables.
# This ensures that when we create a source distribution the
# version is always the current version, not the package bootstrap
# version.
#

OPENHLX_VERSION     ?= $(shell cat $(VERSION_FILE) 2> /dev/null)

PACKAGE_VERSION      = $(OPENHLX_VERSION)
VERSION              = $(PACKAGE_VERSION)

#
# check-file-.local-version
#
# Speculatively regenerate .local-version and check to see if it needs
# to be updated.
#
# If OPENHLX_VERSION has been supplied anywhere other than in this file
# (which is implicitly the contents of .local-version), then use that;
# otherwise, attempt to generate it from the SCM system.
#
# This is called from $(call check-file,.local-version).
#
define check-file-.local-version
if [ "$(origin OPENHLX_VERSION)" != "file" ]; then               \
    echo "$(OPENHLX_VERSION)" > "$(2)";                          \
else                                                             \
    $(abs_top_nlbuild_autotools_dir)/scripts/mkversion           \
        -b "$(OPENHLX_VERSION)" "$(top_srcdir)"                  \
        > "$(2)";                                                \
fi
endef

#
# check-file-.dist-version
#
# Speculatively regenerate .dist-version and check to see if it needs
# to be updated.
#
# This is called from $(call check-file,.dist-version).
#
define check-file-.dist-version
cat "$(1)" > "$(2)"
endef

#
# A convenience target to allow package users to easily rerun the
# package configuration according to the current configuration.
#
.PHONY: reconfigure
reconfigure: $(builddir)/config.status
	$(AM_V_at)$(<) --recheck

#
# Version file regeneration rules.
#
.PHONY: force

$(builddir)/.local-version: $(srcdir)/.default-version force

$(distdir)/.dist-version: $(builddir)/.local-version force

$(distdir)/.dist-version $(builddir)/.local-version:
	$(call check-file,$(@F))

# If you are synchronizing a package on which yours depends using
# 'repos.conf', to 'third_party', uncomment and adapt or delete this,
# as needed. If you choose to use this, please also take a look at
# Makefile.am and third_party/Makefile.am and uncomment the
# appropriate sections there.

#
# When we run 'distcheck' and --with-nlunit_test defaults to 'internal',
# the nlbuild-autotools infrastructure will attempt to create git paths
# to manage the nlunit-test repo. Two directories need to be writable
# to facilitate this.
#
DISTCHECK_CONFIGURE_FLAGS=`chmod u+w .. ../third_party`

dist distcheck: $(BUILT_SOURCES)

dist-hook: $(distdir)/.dist-version

# If you are synchronizing a package on which yours depends using
# 'repos.conf', to 'third_party', uncomment and adapt or delete this,
# as needed. If you choose to use this, please also take a look at
# Makefile.am and third_party/Makefile.am and uncomment the
# appropriate sections there.

#
# Ensure any locally synchronized repositories defined by 'repos.conf'
# are cleaned up.
#
distclean-local:
	$(MAKE) -C $(srcdir) -f Makefile-bootstrap clean-repos

#
# Top-level convenience target for making a documentation-only
# distribution whose results appear at the top level of the build tree
# in the same fashion that the distribution would be for 'make dist'.
#

.PHONY: docdist
docdist: $(BUILT_SOURCES)
	$(MAKE) -C doc docdistdir=$(abs_builddir) $(@)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
