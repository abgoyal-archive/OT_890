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
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CrossThreadRefCounted_h
#define CrossThreadRefCounted_h

#include <wtf/Noncopyable.h>
#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <wtf/Threading.h>

namespace WTF {

    // Used to allowing sharing data across classes and threads (like ThreadedSafeShared).
    //
    // Why not just use ThreadSafeShared?
    // ThreadSafeShared can have a significant perf impact when used in low level classes
    // (like UString) that get ref/deref'ed a lot. This class has the benefit of doing fast ref
    // counts like RefPtr whenever possible, but it has the downside that you need to copy it
    // to use it on another thread.
    //
    // Is this class threadsafe?
    // While each instance of the class is not threadsafe, the copied instance is threadsafe
    // with respect to the original and any other copies.  The underlying m_data is jointly
    // owned by the original instance and all copies.
    template<class T>
    class CrossThreadRefCounted : public Noncopyable {
    public:
        static PassRefPtr<CrossThreadRefCounted<T> > create(T* data)
        {
            return adoptRef(new CrossThreadRefCounted<T>(data, 0));
        }

        // Used to make an instance that can be used on another thread.
        PassRefPtr<CrossThreadRefCounted<T> > crossThreadCopy();

        void ref();
        void deref();
        T* release();

        bool isShared() const
        {
            return !m_refCounter.hasOneRef() || (m_threadSafeRefCounter && !m_threadSafeRefCounter->hasOneRef());
        }

#ifndef NDEBUG
        bool mayBePassedToAnotherThread() const { ASSERT(!m_threadId); return m_refCounter.hasOneRef(); }
#endif

    private:
        CrossThreadRefCounted(T* data, ThreadSafeSharedBase* threadedCounter)
            : m_threadSafeRefCounter(threadedCounter)
            , m_data(data)
#ifndef NDEBUG
            , m_threadId(0)
#endif
        {
        }

        ~CrossThreadRefCounted()
        {
            if (!m_threadSafeRefCounter)
                delete m_data;
        }

        void threadSafeDeref();

        RefCountedBase m_refCounter;
        ThreadSafeSharedBase* m_threadSafeRefCounter;
        T* m_data;
#ifndef NDEBUG
        ThreadIdentifier m_threadId;
#endif
    };

    template<class T>
    void CrossThreadRefCounted<T>::ref()
    {
        ASSERT(!m_threadId || m_threadId == currentThread());
        m_refCounter.ref();
#ifndef NDEBUG
        // Store the threadId as soon as the ref count gets to 2.
        // The class gets created with a ref count of 1 and then passed
        // to another thread where to ref count get increased.  This
        // is a heuristic but it seems to always work and has helped
        // find some bugs.
        if (!m_threadId && m_refCounter.refCount() == 2)
            m_threadId = currentThread();
#endif
    }

    template<class T>
    void CrossThreadRefCounted<T>::deref()
    {
        ASSERT(!m_threadId || m_threadId == currentThread());
        if (m_refCounter.derefBase()) {
            threadSafeDeref();
            delete this;
        } else {
#ifndef NDEBUG
            // Clear the threadId when the ref goes to 1 because it
            // is safe to be passed to another thread at this point.
            if (m_threadId && m_refCounter.refCount() == 1)
                m_threadId = 0;
#endif
        }
    }

    template<class T>
    T* CrossThreadRefCounted<T>::release()
    {
        ASSERT(!isShared());

        T* data = m_data;
        m_data = 0;
        return data;
    }

    template<class T>
    PassRefPtr<CrossThreadRefCounted<T> > CrossThreadRefCounted<T>::crossThreadCopy()
    {
        if (m_threadSafeRefCounter)
            m_threadSafeRefCounter->ref();
        else
            m_threadSafeRefCounter = new ThreadSafeSharedBase(2);
        return adoptRef(new CrossThreadRefCounted<T>(m_data, m_threadSafeRefCounter));
    }


    template<class T>
    void CrossThreadRefCounted<T>::threadSafeDeref()
    {
        if (m_threadSafeRefCounter && m_threadSafeRefCounter->derefBase()) {
            delete m_threadSafeRefCounter;
            m_threadSafeRefCounter = 0;
        }
    }
} // namespace WTF

using WTF::CrossThreadRefCounted;

#endif // CrossThreadRefCounted_h
