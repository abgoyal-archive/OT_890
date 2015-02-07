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

#ifndef V8EventListenerList_h
#define V8EventListenerList_h

#include <v8.h>
#include <wtf/Vector.h>
#include <wtf/HashMap.h>

#include "PassRefPtr.h"

namespace WebCore {
    class Frame;
    class V8EventListener;
    class V8EventListenerListIterator;

    // This is a container for V8EventListener objects that uses the identity hash of the v8::Object to
    // speed up lookups
    class V8EventListenerList {
    public:
        // Because v8::Object identity hashes are not guaranteed to be unique, we unfortunately can't just map
        // an int to V8EventListener. Instead we define a HashMap of int to Vector of V8EventListener
        // called a ListenerMultiMap.
        typedef Vector<V8EventListener*>* Values;
        struct ValuesTraits : HashTraits<Values> {
            static const bool needsDestruction = true;
        };
        typedef HashMap<int, Values, DefaultHash<int>::Hash, HashTraits<int>, ValuesTraits> ListenerMultiMap;

        V8EventListenerList();
        ~V8EventListenerList();

        friend class V8EventListenerListIterator;
        typedef V8EventListenerListIterator iterator;        

        iterator begin();
        iterator end();

        void add(V8EventListener*);
        void remove(V8EventListener*);
        V8EventListener* find(v8::Local<v8::Object>, bool isAttribute);
        void clear();
        size_t size() { return m_table.size(); }

        PassRefPtr<V8EventListener> findWrapper(v8::Local<v8::Value>, bool isAttribute);
        template<typename WrapperType>
        PassRefPtr<V8EventListener> findOrCreateWrapper(Frame*, v8::Local<v8::Value>, bool isAttribute);

    private:
        ListenerMultiMap m_table;

        // we also keep a reverse mapping of V8EventListener to v8::Object identity hash,
        // in order to speed up removal by V8EventListener
        HashMap<V8EventListener*, int> m_reverseTable;
    };

    class V8EventListenerListIterator {
    public:
        ~V8EventListenerListIterator();
        void operator++();
        bool operator==(const V8EventListenerListIterator&);
        bool operator!=(const V8EventListenerListIterator&);
        V8EventListener* operator*();
    private:
        friend class V8EventListenerList;
        explicit V8EventListenerListIterator(V8EventListenerList*);
        V8EventListenerListIterator(V8EventListenerList*, bool shouldSeekToEnd);
        void seekToEnd();

        V8EventListenerList* m_list;
        V8EventListenerList::ListenerMultiMap::iterator m_iter;
        size_t m_vectorIndex;
    };

    template<typename WrapperType>
    PassRefPtr<V8EventListener> V8EventListenerList::findOrCreateWrapper(Frame* frame, v8::Local<v8::Value> object, bool isAttribute)
    {
        ASSERT(v8::Context::InContext());
        if (!object->IsObject())
            return 0;

        // FIXME: Should this be v8::Local<v8::Object>::Cast instead?
        V8EventListener* wrapper = find(object->ToObject(), isAttribute);
        if (wrapper)
            return wrapper;

        // Create a new one, and add to cache.
        RefPtr<WrapperType> newListener = WrapperType::create(frame, v8::Local<v8::Object>::Cast(object), isAttribute);
        add(newListener.get());

        return newListener;
    };

} // namespace WebCore

#endif // V8EventListenerList_h
