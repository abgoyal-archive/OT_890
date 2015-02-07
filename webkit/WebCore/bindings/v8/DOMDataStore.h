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

#ifndef DOMDataStore_h
#define DOMDataStore_h

#include "DOMObjectsInclude.h"

#include <v8.h>
#include <wtf/HashMap.h>
#include <wtf/MainThread.h>
#include <wtf/Noncopyable.h>
#include <wtf/OwnPtr.h>
#include <wtf/StdLibExtras.h>
#include <wtf/Threading.h>
#include <wtf/ThreadSpecific.h>
#include <wtf/Vector.h>

namespace WebCore {

    class DOMData;

    typedef WTF::Vector<DOMDataStore*> DOMDataList;

    // DOMDataStore
    //
    // DOMDataStore is the backing store that holds the maps between DOM objects
    // and JavaScript objects.  In general, each thread can have multiple backing
    // stores, one per isolated world.
    //
    // This class doesn't manage the lifetime of the store.  The data store
    // lifetime is managed by subclasses.
    //
    class DOMDataStore : public Noncopyable {
    public:
        enum DOMWrapperMapType {
            DOMNodeMap,
            DOMObjectMap,
            ActiveDOMObjectMap,
#if ENABLE(SVG)
            DOMSVGElementInstanceMap,
            DOMSVGObjectWithContextMap
#endif
        };

        template <class KeyType>
        class InternalDOMWrapperMap : public DOMWrapperMap<KeyType> {
        public:
            InternalDOMWrapperMap(DOMData* domData, v8::WeakReferenceCallback callback)
                : DOMWrapperMap<KeyType>(callback), m_domData(domData) { }

            virtual void forget(KeyType* object)
            {
                DOMWrapperMap<KeyType>::forget(object);
                forgetDelayedObject(m_domData, object);
            }

            void forgetOnly(KeyType* object) { DOMWrapperMap<KeyType>::forget(object); }

        private:
            DOMData* m_domData;
        };

        DOMDataStore(DOMData*);
        virtual ~DOMDataStore();

        // A list of all DOMDataStore objects.  Traversed during GC to find a thread-specific map that
        // contains the object - so we can schedule the object to be deleted on the thread which created it.
        static DOMDataList& allStores();
        // Mutex to protect against concurrent access of DOMDataList.
        static WTF::Mutex& allStoresMutex();

        // Helper function to avoid circular includes.
        static void forgetDelayedObject(DOMData*, void* object);

        DOMData* domData() const { return m_domData; }

        void* getDOMWrapperMap(DOMWrapperMapType);

        InternalDOMWrapperMap<Node>& domNodeMap() { return *m_domNodeMap; }
        InternalDOMWrapperMap<void>& domObjectMap() { return *m_domObjectMap; }
        InternalDOMWrapperMap<void>& activeDomObjectMap() { return *m_activeDomObjectMap; }
#if ENABLE(SVG)
        InternalDOMWrapperMap<SVGElementInstance>& domSvgElementInstanceMap() { return *m_domSvgElementInstanceMap; }
        InternalDOMWrapperMap<void>& domSvgObjectWithContextMap() { return *m_domSvgObjectWithContextMap; }
#endif

        // Need by V8GCController.
        static void weakActiveDOMObjectCallback(v8::Persistent<v8::Value> v8Object, void* domObject);

    protected:
        static void weakNodeCallback(v8::Persistent<v8::Value> v8Object, void* domObject);
        static void weakDOMObjectCallback(v8::Persistent<v8::Value> v8Object, void* domObject);
#if ENABLE(SVG)
        static void weakSVGElementInstanceCallback(v8::Persistent<v8::Value> v8Object, void* domObject);
        // SVG non-node elements may have a reference to a context node which should be notified when the element is change.
        static void weakSVGObjectWithContextCallback(v8::Persistent<v8::Value> v8Object, void* domObject);
#endif
        
        InternalDOMWrapperMap<Node>* m_domNodeMap;
        InternalDOMWrapperMap<void>* m_domObjectMap;
        InternalDOMWrapperMap<void>* m_activeDomObjectMap;
#if ENABLE(SVG)
        InternalDOMWrapperMap<SVGElementInstance>* m_domSvgElementInstanceMap;
        InternalDOMWrapperMap<void>* m_domSvgObjectWithContextMap;
#endif

    private:
        // A back-pointer to the DOMData to which we belong.
        DOMData* m_domData;
    };

} // namespace WebCore

#endif // DOMDataStore_h
