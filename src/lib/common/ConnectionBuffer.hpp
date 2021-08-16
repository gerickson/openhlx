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
 *      This file defines an object for sending or receiving data over
 *      a peer-to-peer network connection.
 *
 */

#ifndef OPENHLXCOMMONCONNECTIONBUFFER_HPP
#define OPENHLXCOMMONCONNECTIONBUFFER_HPP

#include <memory>

#include <stddef.h>

#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    An object for sending or receiving data over a peer-to-peer
 *    network connection.
 *
 *  @ingroup common
 *
 */
class ConnectionBuffer
{
public:
    /**
     *  Smart or shared pointer to a mutable @a ConnectionBuffer in which
     *  the associated memory is released when there are no further
     *  owners of the underlying pointer.
     *
     */
    typedef std::shared_ptr<ConnectionBuffer>       MutableCountedPointer;

    /**
     *  Smart or shared pointer to an immutable @a ConnectionBuffer in
     *  which the associated memory is released when there are no
     *  further owners of the underlying pointer.
     *
     */
    typedef std::shared_ptr<const ConnectionBuffer> ImmutableCountedPointer;

public:
    ConnectionBuffer(void);
    ~ConnectionBuffer(void);

    Status    Init(void);
    Status    Init(const size_t &aCapacity);
    Status    Init(uint8_t *aData, const size_t &aCapacity);

    size_t    GetSize(void) const;
    size_t    GetCapacity(void) const;

    uint8_t * GetHead(void) const;
    uint8_t * GetTail(void) const;

    Status    Reserve(const size_t &aCapacity);

    uint8_t * Put(const uint8_t *aData, const size_t &aSize);
    uint8_t * Put(const size_t &aSize);
    uint8_t * Get(uint8_t *aData, const size_t &aSize);
    uint8_t * Get(const size_t &aSize);
    Status    SetSize(const size_t &aSize);

    void      Flush(void);

    void      Destroy(void);

private:
    size_t    GetPageSize(void) const;

private:
    uint8_t * mData;
    size_t    mSize;
    size_t    mCapacity;
    bool      mDataOwner;
};

namespace Utilities
{

extern Status Put(ConnectionBuffer & inBuffer,
                  const uint8_t *    inData,
                  const size_t &     inSize);

}; // namespace Utilities

}; // namespace Common

}; // namespace HLX

#endif // OPENHLXCOMMONCONNECTIONBUFFER_HPP
