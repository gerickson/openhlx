#
#    Copyright (c) 2018-2021 Grant Erickson. All Rights Reserved.
#    Copyright 2016 Nest Labs Inc. All Rights Reserved.
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
#      unit tests.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

#
# Local headers to build against and distribute but not to install
# since they are not part of the package.
#
noinst_HEADERS                                 = \
    $(NULL)

#
# Other files we do want to distribute with the package.
#
EXTRA_DIST                                     = \
    $(NULL)

if OPENHLX_BUILD_TESTS
# C preprocessor option flags that will apply to all compiled objects in this
# makefile.

AM_CPPFLAGS                                                            = \
    -I$(top_srcdir)/src/lib/common                                       \
    -I$(top_srcdir)/src/lib/model                                        \
    -I$(top_srcdir)/src/lib/utilities                                    \
    -I$(top_srcdir)/third_party/LogUtilities/repo/include                \
    $(NULL)

COMMON_LDADD                                                           = \
    $(top_builddir)/src/lib/model/libopenhlx-model.a                     \
    $(top_builddir)/src/lib/utilities/libopenhlx-utilities.a             \
    $(top_builddir)/third_party/LogUtilities/repo/src/libLogUtilities.la \
    $(NULL)

# Test applications that should be run when the 'check' target is run.

check_PROGRAMS                                                         = \
    TestBalanceModel                                                     \
    TestCrossoverModel                                                   \
    TestEqualizerBandModel                                               \
    TestEqualizerBandsModel                                              \
    TestEqualizerPresetModel                                             \
    TestEqualizerPresetsModel                                            \
    TestFavoriteModel                                                    \
    TestFavoritesModel                                                   \
    TestFrontPanelModel                                                  \
    TestGroupModel                                                       \
    TestGroupsModel                                                      \
    TestIdentifierModel                                                  \
    TestIdentifiersCollection                                            \
    TestInfraredModel                                                    \
    TestNameModel                                                        \
    TestSoundModel                                                       \
    TestSourceModel                                                      \
    TestSourcesModel                                                     \
    TestToneModel                                                        \
    TestVolumeModel                                                      \
    TestZoneModel                                                        \
    TestZonesModel                                                       \
    $(NULL)

# Test applications and scripts that should be built and run when the
# 'check' target is run.

TESTS                                                                  = \
    $(check_PROGRAMS)                                                    \
    $(NULL)

# The additional environment variables and their values that will be
# made available to all programs and scripts in TESTS.

TESTS_ENVIRONMENT                                                      = \
    $(NULL)

# Source, compiler, and linker options for test programs.

TestBalanceModel_SOURCES                       = TestBalanceModel.cpp
TestBalanceModel_LDADD                         = $(COMMON_LDADD)

TestCrossoverModel_SOURCES                     = TestCrossoverModel.cpp
TestCrossoverModel_LDADD                       = $(COMMON_LDADD)

TestEqualizerBandModel_SOURCES                 = TestEqualizerBandModel.cpp
TestEqualizerBandModel_LDADD                   = $(COMMON_LDADD)

TestEqualizerBandsModel_SOURCES                = TestEqualizerBandsModel.cpp
TestEqualizerBandsModel_LDADD                  = $(COMMON_LDADD)

TestEqualizerPresetModel_SOURCES               = TestEqualizerPresetModel.cpp
TestEqualizerPresetModel_LDADD                 = $(COMMON_LDADD)

TestEqualizerPresetsModel_SOURCES              = TestEqualizerPresetsModel.cpp
TestEqualizerPresetsModel_LDADD                = $(COMMON_LDADD)

TestFavoriteModel_SOURCES                      = TestFavoriteModel.cpp
TestFavoriteModel_LDADD                        = $(COMMON_LDADD)

TestFavoritesModel_SOURCES                     = TestFavoritesModel.cpp
TestFavoritesModel_LDADD                       = $(COMMON_LDADD)

TestFrontPanelModel_SOURCES                    = TestFrontPanelModel.cpp
TestFrontPanelModel_LDADD                      = $(COMMON_LDADD)

TestGroupModel_SOURCES                         = TestGroupModel.cpp
TestGroupModel_LDADD                           = $(COMMON_LDADD)

TestGroupsModel_SOURCES                        = TestGroupsModel.cpp
TestGroupsModel_LDADD                          = $(COMMON_LDADD)

TestIdentifierModel_SOURCES                    = TestIdentifierModel.cpp
TestIdentifierModel_LDADD                      = $(COMMON_LDADD)

TestIdentifiersCollection_SOURCES              = TestIdentifiersCollection.cpp
TestIdentifiersCollection_LDADD                = $(COMMON_LDADD)

TestInfraredModel_SOURCES                      = TestInfraredModel.cpp
TestInfraredModel_LDADD                        = $(COMMON_LDADD)

TestNameModel_SOURCES                          = TestNameModel.cpp
TestNameModel_LDADD                            = $(COMMON_LDADD)

TestSoundModel_SOURCES                         = TestSoundModel.cpp
TestSoundModel_LDADD                           = $(COMMON_LDADD)

TestSourceModel_SOURCES                        = TestSourceModel.cpp
TestSourceModel_LDADD                          = $(COMMON_LDADD)

TestSourcesModel_SOURCES                       = TestSourcesModel.cpp
TestSourcesModel_LDADD                         = $(COMMON_LDADD)

TestToneModel_SOURCES                          = TestToneModel.cpp
TestToneModel_LDADD                            = $(COMMON_LDADD)

TestVolumeModel_SOURCES                        = TestVolumeModel.cpp
TestVolumeModel_LDADD                          = $(COMMON_LDADD)

TestZoneModel_SOURCES                          = TestZoneModel.cpp
TestZoneModel_LDADD                            = $(COMMON_LDADD)

TestZonesModel_SOURCES                         = TestZonesModel.cpp
TestZonesModel_LDADD                           = $(COMMON_LDADD)

if OPENHLX_BUILD_COVERAGE
CLEANFILES                                     = $(wildcard *.gcda *.gcno)

if OPENHLX_BUILD_COVERAGE_REPORTS
# The bundle should positively be qualified with the absolute build
# path. Otherwise, VPATH will get auto-prefixed to it if there is
# already such a directory in the non-colocated source tree.

OPENHLX_COVERAGE_BUNDLE                        = ${abs_builddir}/${PACKAGE}${NL_COVERAGE_BUNDLE_SUFFIX}
OPENHLX_COVERAGE_INFO                          = ${OPENHLX_COVERAGE_BUNDLE}/${PACKAGE}${NL_COVERAGE_INFO_SUFFIX}

$(OPENHLX_COVERAGE_BUNDLE):
	$(call create-directory)

# Generate the coverage report, filtering out platform and system
# directories and this test directory.

INCLUDE_PATHS    := $(subst -I,,$(sort $(BOOST_CPPFLAGS) $(CPPUNIT_CPPFLAGS)))
INCLUDE_PATTERNS := $(if $(INCLUDE_PATHS),$(addsuffix *,$(INCLUDE_PATHS)),)
XCODE_PATH       := $(shell xcode-select -p)
XCODE_PATTERN    := $(if $(XCODE_PATH),$(addsuffix *,$(XCODE_PATH)),)

$(OPENHLX_COVERAGE_INFO): check | $(OPENHLX_COVERAGE_BUNDLE)
	$(call generate-coverage-report-with-filter,${top_builddir},${INCLUDE_PATTERNS} ${XCODE_PATTERN} *${subdir}*)

coverage: $(OPENHLX_COVERAGE_INFO)

clean-local: clean-local-coverage

.PHONY: clean-local-coverage
clean-local-coverage:
	-$(AM_V_at)rm -rf $(OPENHLX_COVERAGE_BUNDLE)
endif # OPENHLX_BUILD_COVERAGE_REPORTS
endif # OPENHLX_BUILD_COVERAGE
endif # OPENHLX_BUILD_TESTS

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
