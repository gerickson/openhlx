/*
 *    Copyright (c) 2018-2021 Grant Erickson
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing,
 *    software distributed under the License is distributed on an "AS
 *    IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 *    express or implied.  See the License for the specific language
 *    governing permissions and limitations under the License.
 *
 */

/**
 *    @file
 *      This file defines base, common (that is, independent of
 *      reqestor or responder) command regular expression objects for
 *      HLX zone commands.
 *
 */

#ifndef HLXCOMMONCOMMANDZONESREGULAREXPRESSIONBASES_HPP
#define HLXCOMMONCOMMANDZONESREGULAREXPRESSIONBASES_HPP

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>


namespace HLX
{

namespace Common
{

namespace Command
{

class RegularExpressionBasis;

namespace Zones
{

/**
 *  @brief
 *    Base regular expression object for HLX zone stereophonic sound
 *    mode channel balance command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone stereophonic
 *  sound mode channel balance command.
 *
 *  @ingroup balance
 *  @ingroup common
 *  @ingroup command
 *  @ingroup zone
 *
 */
class BalanceRegularExpressionBasis
{
protected:
    BalanceRegularExpressionBasis(void) = default;
    virtual ~BalanceRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX zone DSP zone equalizer
 *    band level sound mode command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone DSP zone
 *  equalizer band level sound mode command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup zone
 *
 */
class EqualizerBandLevelRegularExpressionBasis
{
protected:
    EqualizerBandLevelRegularExpressionBasis(void) = default;
    virtual ~EqualizerBandLevelRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX zone DSP equalizer preset
 *    sound mode command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone DSP equalizer
 *  preset sound mode command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup zone
 *
 */
class EqualizerPresetRegularExpressionBasis
{
protected:
    EqualizerPresetRegularExpressionBasis(void) = default;
    virtual ~EqualizerPresetRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX zone DSP highpass
 *    crossover sound mode frequency command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone DSP highpass
 *  crossover sound mode frequency command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup zone
 *
 */
class HighpassCrossoverRegularExpressionBasis
{
protected:
    HighpassCrossoverRegularExpressionBasis(void) = default;
    virtual ~HighpassCrossoverRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX zone DSP lowpass
 *    crossover sound mode frequency command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone DSP lowpass
 *  crossover sound mode frequency command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup zone
 *
 */
class LowpassCrossoverRegularExpressionBasis
{
protected:
    LowpassCrossoverRegularExpressionBasis(void) = default;
    virtual ~LowpassCrossoverRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX zone volume mute
 *    command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone volume mute
 *  command.
 *
 *  @sa ToggleMuteRegularExpressionBasis
 *  @sa VolumeAllRegularExpressionBasis
 *  @sa VolumeFixedRegularExpressionBasis
 *  @sa VolumeRegularExpressionBasis
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class MuteRegularExpressionBasis
{
protected:
    MuteRegularExpressionBasis(void) = default;
    virtual ~MuteRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX zone name command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone name command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup zone
 *
 */
class NameRegularExpressionBasis
{
protected:
    NameRegularExpressionBasis(void) = default;
    virtual ~NameRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX zone query command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone query command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup zone
 *
 */
class QueryRegularExpressionBasis
{
protected:
    QueryRegularExpressionBasis(void) = default;
    virtual ~QueryRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX zone DSP sound mode
 *    command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone DSP sound mode
 *  command.
 *
 *  @sa EqualizerBandLevelRegularExpressionBasis
 *  @sa EqualizerPresetRegularExpressionBasis
 *  @sa HighpassCrossoverRegularExpressionBasis
 *  @sa LowpassCrossoverRegularExpressionBasis
 *  @sa ToneRegularExpressionBasis
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SoundModeRegularExpressionBasis
{
protected:
    SoundModeRegularExpressionBasis(void) = default;
    virtual ~SoundModeRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX zone source (input)
 *    command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone source (input)
 *  command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SourceRegularExpressionBasis
{
protected:
    SourceRegularExpressionBasis(void) = default;
    virtual ~SourceRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX all-zones source (input)
 *    command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX all-zones source (input)
 *  command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup zone
 *
 */
class SourceAllRegularExpressionBasis
{
protected:
    SourceAllRegularExpressionBasis(void) = default;
    virtual ~SourceAllRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX zone DSP tone sound mode
 *    command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone DSP tone (that is
 *  bass / treble) sound mode command.
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup zone
 *
 */
class ToneRegularExpressionBasis
{
protected:
    ToneRegularExpressionBasis(void) = default;
    virtual ~ToneRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX zone volume mute
 *    command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone volume mute
 *  command.
 *
 *  @sa VolumeAllRegularExpressionBasis
 *  @sa VolumeFixedRegularExpressionBasis
 *  @sa VolumeRegularExpressionBasis
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class ToggleMuteRegularExpressionBasis
{
protected:
    ToggleMuteRegularExpressionBasis(void) = default;
    virtual ~ToggleMuteRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX zone volume level
 *    command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone volume level
 *  command.
 *
 *  @sa MuteRegularExpressionBasis
 *  @sa ToggleMuteRegularExpressionBasis
 *  @sa VolumeAllRegularExpressionBasis
 *  @sa VolumeFixedRegularExpressionBasis
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class VolumeRegularExpressionBasis
{
protected:
    VolumeRegularExpressionBasis(void) = default;
    virtual ~VolumeRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for the HLX all-zones volume
 *    level command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for the HLX all-zones volume
 *  level command.
 *
 *  @sa MuteRegularExpressionBasis
 *  @sa ToggleMuteRegularExpressionBasis
 *  @sa VolumeFixedRegularExpressionBasis
 *  @sa VolumeRegularExpressionBasis
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class VolumeAllRegularExpressionBasis
{
protected:
    VolumeAllRegularExpressionBasis(void) = default;
    virtual ~VolumeAllRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

/**
 *  @brief
 *    Base regular expression object for HLX zone volume fixed/lock
 *    command.
 *
 *  This defines a base, common (that is, independent of requestor or
 *  responder) regular expression object for HLX zone volume fixed/locked
 *  command.
 *
 *  @sa MuteRegularExpressionBasis
 *  @sa ToggleMuteRegularExpressionBasis
 *  @sa VolumeAllRegularExpressionBasis
 *  @sa VolumeRegularExpressionBasis
 *
 *  @ingroup common
 *  @ingroup command
 *  @ingroup volume
 *  @ingroup zone
 *
 */
class VolumeFixedRegularExpressionBasis
{
protected:
    VolumeFixedRegularExpressionBasis(void) = default;
    virtual ~VolumeFixedRegularExpressionBasis(void) = default;

    static Common::Status Init(RegularExpressionBasis &aRegularExpression);

public:
    static const size_t       kExpectedMatches;

private:
    static const char * const kRegexp;
};

}; // namespace Zones

}; // namespace Command

}; // namespace Common

}; // namespace HLX

#endif // HLXCOMMONCOMMANDZONESREGULAREXPRESSIONBASES_HPP
