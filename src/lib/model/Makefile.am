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
#      This file is the makefile for the OpenHLX data model library.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

lib_LIBRARIES                                               = \
    libopenhlx-model.a                                        \
    $(NULL)

# Ensure that libraries are built before trying to run any recursive
# targets from SUBDIRS that might depend on them.

AM_RECURSIVE_TARGETS: $(LIBRARIES)

SUBDIRS                                                     = \
    tests                                                     \
    $(NULL)

noinst_HEADERS                                              = \
    $(NULL)

# The 'install' target directory transform. Headers in
# libopenhlx_model_a_include_HEADERS will get installed in
# libopenhlx_model_a_includedir.

libopenhlx_model_a_includedir                               = \
    ${includedir}/OpenHLX/Model

# Public library headers to distribute and install.

libopenhlx_model_a_include_HEADERS                          = \
    BalanceModel.hpp                                          \
    CrossoverModel.hpp                                        \
    EqualizerBandModel.hpp                                    \
    EqualizerBandsModel.hpp                                   \
    EqualizerPresetModel.hpp                                  \
    EqualizerPresetsModel.hpp                                 \
    FavoriteModel.hpp                                         \
    FavoritesModel.hpp                                        \
    FrontPanelModel.hpp                                       \
    GroupModel.hpp                                            \
    GroupsModel.hpp                                           \
    IdentifiersCollection.hpp                                 \
    IdentifierModel.hpp                                       \
    InfraredModel.hpp                                         \
    NameModel.hpp                                             \
    NetworkModel.hpp                                          \
    SoundModel.hpp                                            \
    SourceModel.hpp                                           \
    SourcesModel.hpp                                          \
    ToneModel.hpp                                             \
    VolumeModel.hpp                                           \
    ZoneModel.hpp                                             \
    ZonesModel.hpp                                            \
    $(NULL)

libopenhlx_model_a_CPPFLAGS                                 = \
    -I$(top_srcdir)/third_party/LogUtilities/repo/include     \
    -I$(top_srcdir)/src/lib/common                            \
    -I$(top_srcdir)/src/lib/utilities                         \
    $(NULL)

libopenhlx_model_a_SOURCES                                  = \
    BalanceModel.cpp                                          \
    CrossoverModel.cpp                                        \
    EqualizerBandModel.cpp                                    \
    EqualizerBandsModel.cpp                                   \
    EqualizerPresetModel.cpp                                  \
    EqualizerPresetsModel.cpp                                 \
    FavoriteModel.cpp                                         \
    FavoritesModel.cpp                                        \
    FrontPanelModel.cpp                                       \
    GroupModel.cpp                                            \
    GroupsModel.cpp                                           \
    IdentifiersCollection.cpp                                 \
    IdentifierModel.cpp                                       \
    InfraredModel.cpp                                         \
    NameModel.cpp                                             \
    NetworkModel.cpp                                          \
    SoundModel.cpp                                            \
    SourceModel.cpp                                           \
    SourcesModel.cpp                                          \
    ToneModel.cpp                                             \
    VolumeModel.cpp                                           \
    ZoneModel.cpp                                             \
    ZonesModel.cpp                                            \
    $(NULL)

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
