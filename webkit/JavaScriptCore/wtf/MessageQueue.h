/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MessageQueue_h
#define MessageQueue_h

#include <limits>
#include <wtf/Assertions.h>
#include <wtf/Deque.h>
#include <wtf/Noncopyable.h>
#include <wtf/Threading.h>

namespace WTF {

    enum MessageQueueWaitResult {
        MessageQueueTerminated,       // Queue was destroyed while waiting for message.
        MessageQueueTimeout,          // Timeout was specified and it expired.
        MessageQueueMessageReceived,  // A message was successfully received and returned.
    };

    template<typename DataType>
    class MessageQueue : public Noncopyable {
    public:
        MessageQueue() : m_killed(false) { }
        
        void append(const DataType&);
        bool appendAndCheckEmpty(const DataType&);
        void prepend(const DataType&);
        bool waitForMessage(DataType&);
        template<typename Predicate>
        MessageQueueWaitResult waitForMessageFilteredWithTimeout(DataType&, Predicate&, double absoluteTime);
        void kill();

        bool tryGetMessage(DataType&);
        bool killed() const;

        // The result of isEmpty() is only valid if no other thread is manipulating the queue at the same time.
        bool isEmpty();

        static double infiniteTime() { return std::numeric_limits<double>::max(); }

    private:
        static bool alwaysTruePredicate(DataType&) { return true; }

        mutable Mutex m_mutex;
        ThreadCondition m_condition;
        Deque<DataType> m_queue;
        bool m_killed;
    };

    template<typename DataType>
    inline void MessageQueue<DataType>::append(const DataType& message)
    {
        MutexLocker lock(m_mutex);
        m_queue.append(message);
        m_condition.signal();
    }

    // Returns true if the queue was empty before the item was added.
    template<typename DataType>
    inline bool MessageQueue<DataType>::appendAndCheckEmpty(const DataType& message)
    {
        MutexLocker lock(m_mutex);
        bool wasEmpty = m_queue.isEmpty();
        m_queue.append(message);
        m_condition.signal();
        return wasEmpty;
    }

    template<typename DataType>
    inline void MessageQueue<DataType>::prepend(const DataType& message)
    {
        MutexLocker lock(m_mutex);
        m_queue.prepend(message);
        m_condition.signal();
    }

    template<typename DataType>
    inline bool MessageQueue<DataType>::waitForMessage(DataType& result)
    {
        MessageQueueWaitResult exitReason = waitForMessageFilteredWithTimeout(result, MessageQueue<DataType>::alwaysTruePredicate, infiniteTime());
        ASSERT(exitReason == MessageQueueTerminated || exitReason == MessageQueueMessageReceived);
        return exitReason == MessageQueueMessageReceived;
    }

    template<typename DataType>
    template<typename Predicate>
    inline MessageQueueWaitResult MessageQueue<DataType>::waitForMessageFilteredWithTimeout(DataType& result, Predicate& predicate, double absoluteTime)
    {
        MutexLocker lock(m_mutex);
        bool timedOut = false;

        DequeConstIterator<DataType> found = m_queue.end();
        while (!m_killed && !timedOut && (found = m_queue.findIf(predicate)) == m_queue.end())
            timedOut = !m_condition.timedWait(m_mutex, absoluteTime);

        ASSERT(!timedOut || absoluteTime != infiniteTime());

        if (m_killed)
            return MessageQueueTerminated;

        if (timedOut)
            return MessageQueueTimeout;

        ASSERT(found != m_queue.end());
        result = *found;
        m_queue.remove(found);
        return MessageQueueMessageReceived;
    }

    template<typename DataType>
    inline bool MessageQueue<DataType>::tryGetMessage(DataType& result)
    {
        MutexLocker lock(m_mutex);
        if (m_killed)
            return false;
        if (m_queue.isEmpty())
            return false;

        result = m_queue.first();
        m_queue.removeFirst();
        return true;
    }

    template<typename DataType>
    inline bool MessageQueue<DataType>::isEmpty()
    {
        MutexLocker lock(m_mutex);
        if (m_killed)
            return true;
        return m_queue.isEmpty();
    }

    template<typename DataType>
    inline void MessageQueue<DataType>::kill()
    {
        MutexLocker lock(m_mutex);
        m_killed = true;
        m_condition.broadcast();
    }

    template<typename DataType>
    inline bool MessageQueue<DataType>::killed() const
    {
        MutexLocker lock(m_mutex);
        return m_killed;
    }
} // namespace WTF

using WTF::MessageQueue;
// MessageQueueWaitResult enum and all its values.
using WTF::MessageQueueWaitResult;
using WTF::MessageQueueTerminated;
using WTF::MessageQueueTimeout;
using WTF::MessageQueueMessageReceived;

#endif // MessageQueue_h
