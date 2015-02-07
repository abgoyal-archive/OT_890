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
 *  Copyright (C) 2006, 2008 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef PropertyNameArray_h
#define PropertyNameArray_h

#include "CallFrame.h"
#include "Identifier.h"
#include "Structure.h"
#include <wtf/HashSet.h>
#include <wtf/Vector.h>

namespace JSC {

    class PropertyNameArrayData : public RefCounted<PropertyNameArrayData> {
    public:
        typedef Vector<Identifier, 20> PropertyNameVector;
        typedef PropertyNameVector::const_iterator const_iterator;

        static PassRefPtr<PropertyNameArrayData> create() { return adoptRef(new PropertyNameArrayData); }

        const_iterator begin() const { return m_propertyNameVector.begin(); }
        const_iterator end() const { return m_propertyNameVector.end(); }

        PropertyNameVector& propertyNameVector() { return m_propertyNameVector; }

        void setCachedStructure(Structure* structure) { m_cachedStructure = structure; }
        Structure* cachedStructure() const { return m_cachedStructure; }

        void setCachedPrototypeChain(PassRefPtr<StructureChain> cachedPrototypeChain) { m_cachedPrototypeChain = cachedPrototypeChain; }
        StructureChain* cachedPrototypeChain() { return m_cachedPrototypeChain.get(); }

    private:
        PropertyNameArrayData()
            : m_cachedStructure(0)
        {
        }

        PropertyNameVector m_propertyNameVector;
        Structure* m_cachedStructure;
        RefPtr<StructureChain> m_cachedPrototypeChain;
    };

    class PropertyNameArray {
    public:
        typedef PropertyNameArrayData::const_iterator const_iterator;

        PropertyNameArray(JSGlobalData* globalData)
            : m_data(PropertyNameArrayData::create())
            , m_globalData(globalData)
            , m_shouldCache(true)
        {
        }

        PropertyNameArray(ExecState* exec)
            : m_data(PropertyNameArrayData::create())
            , m_globalData(&exec->globalData())
            , m_shouldCache(true)
        {
        }

        JSGlobalData* globalData() { return m_globalData; }

        void add(const Identifier& identifier) { add(identifier.ustring().rep()); }
        void add(UString::Rep*);
        void addKnownUnique(UString::Rep* identifier) { m_data->propertyNameVector().append(Identifier(m_globalData, identifier)); }

        size_t size() const { return m_data->propertyNameVector().size(); }

        Identifier& operator[](unsigned i) { return m_data->propertyNameVector()[i]; }
        const Identifier& operator[](unsigned i) const { return m_data->propertyNameVector()[i]; }

        const_iterator begin() const { return m_data->begin(); }
        const_iterator end() const { return m_data->end(); }

        void setData(PassRefPtr<PropertyNameArrayData> data) { m_data = data; }
        PropertyNameArrayData* data() { return m_data.get(); }

        PassRefPtr<PropertyNameArrayData> releaseData() { return m_data.release(); }

        void setShouldCache(bool shouldCache) { m_shouldCache = shouldCache; }
        bool shouldCache() const { return m_shouldCache; }

    private:
        typedef HashSet<UString::Rep*, PtrHash<UString::Rep*> > IdentifierSet;

        RefPtr<PropertyNameArrayData> m_data;
        IdentifierSet m_set;
        JSGlobalData* m_globalData;
        bool m_shouldCache;
    };

} // namespace JSC

#endif // PropertyNameArray_h
