/*
 *    Copyright (c) 2007-2010 Nuovation System Designs, LLC
 *    All rights reserved.
 *
 *    IMPORTANT: This software is supplied to you by Nuovation System
 *    Designs, LLC ("Nuovations"). in consideration of your agreement
 *    to the following terms, and your use, installation, modification
 *    or redistribution of this Nuovations software constitutes
 *    acceptance of these terms. If you do not agree with these terms,
 *    please do not use, install, modify or redistribute this
 *    Nuovations software.
 *
 *    In consideration of your agreement to abide by the following
 *    terms, and subject to these terms, Nuovations grants you a
 *    worldwide, non-exclusive license, under Nuovations' copyrights in
 *    this original Nuovations software (the "Nuovations Software"), to
 *    use, reproduce, modify and redistribute the Nuovations Software,
 *    with or without modifications, in source and/or binary forms;
 *    provided that if you redistribute the Nuovations Software in its
 *    entirety and without modifications, you must retain this notice
 *    and the following text and disclaimers in all such
 *    redistributions of the Nuovations Software. Neither the name,
 *    trademarks, service marks or logos of Nuovations may be used to
 *    endorse or promote products derived from the Nuovations Software
 *    without specific prior written permission from Nuovations. Except
 *    as expressly stated in this notice, no other rights or licenses,
 *    express or implied, are granted by Nuovations herein, including
 *    but not limited to any patent rights that may be infringed by
 *    your derivative works or by other works in which the Nuovations
 *    Software may be incorporated.
 *
 *    The Nuovations Software is provided by Nuovations on an "AS IS"
 *    basis. NUOVATIONS MAKES NO WARRANTIES, EXPRESS OR IMPLIED,
 *    INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF
 *    NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *    PURPOSE, REGARDING THE NUOVATIONS SOFTWARE OR ITS USE AND
 *    OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 *
 *    IN NO EVENT SHALL NUOVATIONS BE LIABLE FOR ANY SPECIAL, INDIRECT,
 *    INCIDENTAL OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *    TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY
 *    OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF
 *    THE NUOVATIONS SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY
 *    OF CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR
 *    OTHERWISE, EVEN IF NUOVATIONS HAS BEEN ADVISED OF THE POSSIBILITY
 *    OF SUCH DAMAGE.
 *
 */

/**
 *  @file
 *    This file implements a routine that generates a list of "short"
 *    or single-character command-line options equivalents from a list
 *    of "long" or double-dash options.
 *
 */

#include <NuovationsUtilities/GenerateShortOptions.hpp>

#include <string>

#include <ctype.h>
#include <getopt.h>
#include <stdlib.h>


namespace Nuovations {

namespace Utilities {

/**
 *  @brief
 *    This routine generates a list of "short" or single-character
 *    command-line option equivalents from a list of "long" or
 *    double-dash options.
 *
 *  The caller may specify that the options should be POSIXly correct
 *  by terminating option processing at the first non-option argument.
 *
 *  @param[in]  inPosixlyCorrect  A flag indicating whether or not to
 *                                make the options POSIXly correct.
 *  @param[in]  inOptions         A pointer to long, double-dash options
 *				  to generate short options from.
 *  @param[out] outString         The string to receive the generated
 *                                list of "short" single-character
 *                                option equivalents. On success, the
 *                                string is populated with short
 *                                options in a form required by
 *                                getopt() and friends.
 *
 */
void
GenerateShortOptions(bool inPosixlyCorrect,
                     const struct option *inOptions,
                     std::string &outString)
{
    if (inOptions == NULL) {
        return;
    }

    // Make the options POSIXly correct by preprending a '+' to the
    // short options which will terminate option processing at the
    // first non-option (the command, if present).

    if (inPosixlyCorrect) {
        outString += '+';
    }

    for (const struct option *op = inOptions; op->name != NULL; op++) {
        if (op->flag == NULL	&&
            (op->val != 0	&&
             isascii(op->val)	&&
             isalnum(op->val))) {
            outString += static_cast<char>(op->val);

            if (op->has_arg == required_argument) {
                outString += ':';
            }

            if (op->has_arg == optional_argument) {
                outString += "::";
            }
        }
    }
}

}; // namespace Utilities

}; // namespace Nuovations
