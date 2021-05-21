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

/*
 *    Portions Copyright (c) 1997 Pete Hart, William Lewis
 *    Portions Copyright (c) 2001 Sean Anderson
 *
 */

/**
 *    @file
 *      This file implements an object for sending or receiving data over
 *      a peer-to-peer network connection.
 *
 */

#include <new>

#include "ConnectionBuffer.hpp"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <OpenHLX/Utilities/Assert.hpp>
#include <OpenHLX/Common/Errors.hpp>

namespace HLX
{

namespace Common
{

/**
 *  @brief
 *    This is the class default constructor.
 *
 */
ConnectionBuffer :: ConnectionBuffer(void) :
    mData(nullptr),
    mSize(0),
    mCapacity(0),
    mDataOwner(false)
{
    return;
}

/**
 *  @brief
 *    This is the class destructor.
 *
 *  The buffer and any associated backing store is flushed, destroyed,
 *  and released. While Destroy may be invoked safely prior to
 *  destruction and may be paired with Init for symmetry, since
 *  Destory is invoked by the destructor itself, it is not necessary.
 *
 */
ConnectionBuffer :: ~ConnectionBuffer(void)
{
    Destroy();
}

/**
 *  @brief
 *    This initializes the buffer with defaults.
 *
 *    This initializes the buffer with a dynamically-allocated,
 *    buffer-owned backing store with a system-defined page size worth
 *    of capacity.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EIO             If the system page size cannot be determined.
 *  @retval  -ENOMEM          If the buffer-owned backing store cannot be
 *                            allocated.
 *
 */
Status ConnectionBuffer :: Init(void)
{
    const size_t lPageSize = GetPageSize();
    Status lRetval;

    if (lPageSize == 0)
        lRetval = -EIO;
    else
        lRetval = Init(nullptr, lPageSize);

    return (lRetval);
}

/**
 *  @brief
 *    This initializes the buffer with the specified capacity.
 *
 *    This initializes the buffer with a dynamically-allocated,
 *    buffer-owned backing store with the specified capacity.
 *
 *  @param[in]  inCapacity  The capacity, in bytes, of the buffer-
 *                          owned backing store.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EIO             If the system page size cannot be determined.
 *  @retval  -ENOMEM          If the buffer-owned backing store cannot be
 *                            allocated.
 *
 */
Status ConnectionBuffer :: Init(const size_t &inCapacity)
{
    return (Init(nullptr, inCapacity));
}

/**
 *  @brief
 *    This initializes the buffer with an optional buffer and the
 *    specified capacity.
 *
 *    This initializes the buffer with am optional caller-owned buffer
 *    or, failing that, a dynamically-allocated, buffer-owned backing
 *    store. Regardless, the specified capacity for either is used.
 *
 *  @param[in]  inData      An optional pointer to a caller-owned backing
 *                          store. If null, a dynamically-allocated,
 *                          buffer-owned backing store will be used.
 *  @param[in]  inCapacity  The capacity, in bytes, of the backing store.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -EIO             If the system page size cannot be determined.
 *  @retval  -ENOMEM          If the buffer-owned backing store cannot be
 *                            allocated.
 *
 */
Status ConnectionBuffer :: Init(uint8_t *inData, const size_t &inCapacity)
{
    Status lRetval = kStatus_Success;

    if (inData != nullptr)
    {
        mData      = inData;
        mSize      = 0;
        mCapacity  = inCapacity;
        mDataOwner = false;
    }
    else
    {
        mData      = static_cast<uint8_t *>(malloc(inCapacity));

        if (mData != nullptr)
        {
            mSize      = 0;
            mCapacity  = inCapacity;
            mDataOwner = true;
        }
        else
        {
            lRetval = -ENOMEM;
        }
    }

    return (lRetval);
}

/**
 *  @brief
 *    This attempts to grow the capacity of the buffer.
 *
 *    This attempts to grow the capacity of the buffer for a
 *    buffer-owned backing store such that the total capacity, on
 *    success, is as specified. The contents of the backing store are
 *    preserved up to the prior capacity. The contents thereafter are
 *    undefined.
 *
 *  @note
 *    The location of the underlying backing store may
 *    change. Consequently, previously-cached values of GetHead or
 *    GetTail may be invalidated across this call.
 *
 *  @note
 *    Specifying a new capacity smaller than the current capacity is
 *    ignored.
 *
 *  @param[in]  inCapacity  The new, desired increased capacity, in
 *              bytes, of the backing store.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_BufferNotOwned  If the backing store is caller-
 *                                  rather than buffer-owned.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *
 */
Status ConnectionBuffer :: Reserve(const size_t &inCapacity)
{
    Status lRetval = kStatus_Success;

    nlEXPECT_ACTION(mDataOwner, done, lRetval = kError_BufferNotOwned);

    if (inCapacity > mCapacity)
    {
        mData = static_cast<uint8_t *>(reallocf(mData, inCapacity));
        nlREQUIRE_ACTION(mData != nullptr, done, lRetval = -ENOMEM);

        mCapacity = inCapacity;
    }

 done:
    return (lRetval);
}

/**
 *  @brief
 *    Return the buffer data size, in bytes.
 *
 *  This returns the amount of data that has been placed in the
 *  buffer, either directly through calls to Put or indirectly with
 *  SetSize.
 *
 *  @returns The buffer data size, in bytes.
 *
 *  @sa Put
 *  @sa SetSize
 *
 */
size_t ConnectionBuffer :: GetSize(void) const
{
    return (mSize);
}

/**
 *  @brief
 *    Return the buffer capacity, in bytes.
 *
 *  @returns The buffer capacity, in bytes.
 *
 */
size_t ConnectionBuffer :: GetCapacity(void) const
{
    return (mCapacity);
}

/**
 *  @brief
 *    Return the pointer to the start, or head, of buffer data.
 *
 *  This returns a pointer to the start, or head, of buffer data.
 *
 *  @returns A pointer to the start, or head, of buffer data.
 *
 */
uint8_t *ConnectionBuffer :: GetHead(void) const
{
    return (mData);
}

/**
 *  @brief
 *    Return the pointer to the end, or tail, of buffer data.
 *
 *  This returns a pointer to the end, or tail, of buffer data such
 *  that the pointer difference between the head and tail is
 *  equivalent to the size.
 *
 *  @returns A pointer to the end, or tail, of buffer data.
 *
 */
uint8_t *ConnectionBuffer :: GetTail(void) const
{
    return (mData + mSize);
}

/**
 *  @brief
 *    Put the specified data into the buffer.
 *
 *  This puts (that is, copies in) the specified amount of data into the
 *  buffer, if sufficient space is available.
 *
 *  If the pointer to the data is null, then no data is copied and the
 *  size is simply adjusted to reflect the requested change in data size.
 *
 *  @param[in]  inData  An optional pointer to the data to be put (that
 *                      is, copied) into the buffer.
 *  @param[in]  inSize  The size, in bytes, of data to be put into the
 *                      buffer.
 *
 *  @returns A pointer to the head (that is, the start) of the newly
 *  put data (or previous tail) if successful; otherwise, null is
 *  there was insufficient space for the put data.
 *
 */
uint8_t *ConnectionBuffer :: Put(const uint8_t *inData, const size_t &inSize)
{
    const size_t lHeadroom = mCapacity - mSize;
    uint8_t *    lRetval   = nullptr;

    if (inSize <= (lHeadroom))
    {
        lRetval = GetTail();

        if (inData != nullptr)
        {
            memmove(lRetval, inData, inSize);
        }

        mSize += inSize;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Put the specified data into the buffer.
 *
 *  This puts (that is, increases) the specified amount of data into
 *  the buffer, if sufficient space is available.
 *
 *  It is assumed that the caller has copied the actual data outside
 *  of the buffer interfaces, so the size is simply adjusted to
 *  reflect the requested change in data size.
 *
 *  @param[in]  inSize  The size, in bytes, of data to be put into the
 *                      buffer.
 *
 *  @returns A pointer to the head (that is, the start) of the newly
 *  put data (or previous tail) if successful; otherwise, null is
 *  there was insufficient space for the put data.
 *
 */
uint8_t *ConnectionBuffer :: Put(const size_t &inSize)
{
    return (Put(nullptr, inSize));
}

/**
 *  @brief
 *    Get the specified data out of the buffer.
 *
 *  This gets (that is, copies out) the specified amount of data out
 *  of the buffer, if sufficient data is available.
 *
 *  If the pointer to the data is null, then no data is copied and the
 *  size is simply adjusted to reflect the requested change in data
 *  size.
 *
 *  @param[in]  inData  An optional pointer to the data storage to be
 *                      gotten (that is, copied) out of the buffer.
 *  @param[in]  inSize  The size, in bytes, of data to be gotten out
 *                      of the buffer.
 *
 *  @returns A pointer to the start of the copied data if successful;
 *  otherwise, null is there was insufficient data for the get.
 *
 */
uint8_t *ConnectionBuffer :: Get(uint8_t *inData, const size_t &inSize)
{
    uint8_t *lRetval = nullptr;

    if (inSize <= mSize)
    {
        uint8_t *lPriorHead = GetHead();

        lRetval = inData;

        // Copy out the requested data if a pointer to copy to has
        // been supplied.

        if (inData != nullptr)
        {
            memmove(lRetval, lPriorHead, inSize);
        }

        // If there is any data remaining, move it to the head of the
        // backing store.

        if (inSize < mSize)
        {
            memmove(lPriorHead, lPriorHead + inSize, mSize - inSize);
        }

        mSize -= inSize;
    }

    return (lRetval);
}

/**
 *  @brief
 *    Get the specified data out of the buffer.
 *
 *  This gets (that is, decreases) the specified amount of data out of
 *  the buffer, if sufficient data is available.
 *
 *  It is assumed that the caller has copied the actual data outside
 *  of the buffer interfaces, so the size is simply adjusted to
 *  reflect the requested change in data size.
 *
 *  @param[in]  inSize  The size, in bytes, of data to be gotten out of
 *                      the buffer.
 *
 *  @returns A null pointer.
 *
 */
uint8_t *ConnectionBuffer :: Get(const size_t &inSize)
{
    return (Get(nullptr, inSize));
}

/**
 *  @brief
 *    Change the size of data in the buffer.
 *
 *  This explicitly changes the size of data in the buffer to the
 *  specified value, assuming it is less than or equal to the buffer
 *  capacity.
 *
 *  @note
 *    The Put and Get interfaces should be preferred. This interface
 *    should be reserved for the exceptional case where data and the
 *    buffer backing store are being manipulated out of the scope of
 *    the buffer interfaces.
 *
 *  @param[in]  inSize  The size, in bytes, of data in the buffer.
 *
 *  @retval  kStatus_Success  If successful.
 *  @retval  -ENOSPC          If the requested size exceeds the buffer
 *                            capacity.
 *
 */
Status ConnectionBuffer :: SetSize(const size_t &inSize)
{
    Status lRetval = kStatus_Success;

    if (inSize <= mCapacity)
    {
        mSize = inSize;
    }
    else
    {
        lRetval = -ENOSPC;
    }

    return (lRetval);
}

/**
 *  @brief
 *    This forgets about, or flushes, any data associated with the
 *    buffer.
 *
 *  This flushes any data associated with the buffer such that the
 *  data size is zero following this call.
 *
 *  This call may serve useful between buffer use cycles to reuse the
 *  buffer without otherwise requiring a construct, initialize,
 *  destroy, and destruct lifecycle.
 *
 *  @note
 *    No changes are made to the backing store, either caller- or
 *    buffer-owned.
 *
 */
void ConnectionBuffer :: Flush(void)
{
    mSize = 0;
}

/**
 *  @brief
 *    This destroys the buffer and releases any buffer-owned
 *    resources.
 *
 *  The buffer and any associated buffer-owned backing store are
 *  released. This method is a symmetric companion to the Init family
 *  of methods.
 *
 */
void ConnectionBuffer :: Destroy(void)
{
    if (mDataOwner && (mData != nullptr))
    {
        free(mData);
    }

    mData      = nullptr;
    mSize      = 0;
    mCapacity  = 0;
    mDataOwner = false;
}

/**
 *  @brief
 *    Get the system page size.
 *
 *  This attempts to retrieve the system page size.
 *
 *  @returns The system page size if successful; otherwise, zero (0)
 *  on failure.
 *
 */
size_t ConnectionBuffer :: GetPageSize(void) const
{
    const long  lPageSize = sysconf(_SC_PAGESIZE);
    size_t      lRetval;

    if (lPageSize == -1)
        lRetval = 0;
    else
        lRetval = static_cast<size_t>(lPageSize);

    return (lRetval);
}

namespace Utilities
{

template <typename T>
static T RoundUp(const T &aValue, const T &aFactor)
{
    return ((((aValue) + (aFactor) - 1) / (aFactor)) * (aFactor));
}

static size_t RoundToNextPowerOf2(const size_t &aValue)
{
    size_t lTemp = aValue;

    lTemp--;

    lTemp |= lTemp >>  1;
    lTemp |= lTemp >>  2;
    lTemp |= lTemp >>  4;
    lTemp |= lTemp >>  8;
    lTemp |= lTemp >> 16;

    lTemp++;

    return (lTemp);
}

/**
 *  @brief
 *    Put the specified data into the provided buffer.
 *
 *  This puts (that is, copies in) the specified amount of data into
 *  the provided buffer, if sufficient space is available. If
 *  sufficient space is not available or if the new (that is, current
 *  plus requested) size after would put the buffer at greater than or
 *  equal to 50% utilization, the buffer is first increased to the
 *  next power of two (2) up from twice (2x) the new size, as a
 *  multiple of the current capacity.
 *
 *  As with the in-class Put interface, if the pointer to the data is
 *  null, then no data is copied and the size is simply adjusted to
 *  reflect the requested change in data size.
 *
 *  @param[in]  inBuffer  The buffer to put the specified data into.
 *  @param[in]  inData    An optional pointer to the data to be put (that
 *                        is, copied) into the buffer.
 *  @param[in]  inSize    The size, in bytes, of data to be put into the
 *                        buffer.
 *
 *  @retval  kStatus_Success        If successful.
 *  @retval  kError_BufferNotOwned  If the backing store is caller-
 *                                  rather than buffer-owned.
 *  @retval  -ENOMEM                If the buffer-owned backing store
 *                                  cannot be allocated.
 *  @retval  -ENOSPC                If the requested size exceeds the
 *                                  buffer capacity.
 *
 */
Status Put(ConnectionBuffer & inBuffer,
           const uint8_t *    inData,
           const size_t &     inSize)
{
    const size_t    lCurrentCapacity     = inBuffer.GetCapacity();
    const size_t    lCurrentSize         = inBuffer.GetSize();
    const size_t    lRequestedSize       = lCurrentSize + inSize;
    const size_t    lHalfCurrentCapacity = (lCurrentCapacity >> 1);
    const uint8_t * lTail;
    Status          lRetval              = kStatus_Success;

    if (lRequestedSize >= lHalfCurrentCapacity)
    {
        const size_t lRequestedCapacity = RoundToNextPowerOf2(RoundUp((lRequestedSize << 1) + 1, lCurrentCapacity));

        lRetval = inBuffer.Reserve(lRequestedCapacity);
        nlREQUIRE_SUCCESS(lRetval, done);
    }

    lTail = inBuffer.Put(inData, inSize);
    nlREQUIRE_ACTION(lTail != nullptr, done, lRetval = -ENOSPC);

done:
    return (lRetval);
}

}; // namespace Utilities

}; // namespace Common

}; // namespace HLX
