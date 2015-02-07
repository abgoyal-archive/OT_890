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
    Copyright (C) 2008 Nikolas Zimmermann <zimmermann@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef SynchronizableTypeWrapper_h
#define SynchronizableTypeWrapper_h

#if ENABLE(SVG)
#include <wtf/Noncopyable.h>

namespace WebCore {

    template<typename StoredType>
    class SynchronizableTypeWrapperBase : public Noncopyable {
    protected:
        SynchronizableTypeWrapperBase();

        template<typename AssignableType>
        void assign(AssignableType type);

        bool needsSynchronization() const;
        void setSynchronized();

    protected:
        StoredType m_value;
        bool m_needsSynchronization;
    };

    template<typename StoredType>
    class SynchronizableTypeWrapper : private SynchronizableTypeWrapperBase<StoredType> {
    public:
        typedef SynchronizableTypeWrapperBase<StoredType> Base;
        SynchronizableTypeWrapper();
 
        // "Forwarding constructors" for primitive type assignment with more than one argument, for exampe SVGLength
        template<typename T1> SynchronizableTypeWrapper(const T1&);
        template<typename T1, typename T2> SynchronizableTypeWrapper(const T1&, const T2&);
        template<typename T1, typename T2, typename T3> SynchronizableTypeWrapper(const T1&, const T2&, const T3&);

        SynchronizableTypeWrapper& operator=(const StoredType&);
        operator StoredType() const;

        using Base::needsSynchronization;
        using Base::setSynchronized;

    private:
        using Base::m_value;
    };

    template<typename StoredPointerType>
    class SynchronizableTypeWrapper<RefPtr<StoredPointerType> > : private SynchronizableTypeWrapperBase<RefPtr<StoredPointerType> > {
    public:
        typedef SynchronizableTypeWrapperBase<RefPtr<StoredPointerType> > Base;
        SynchronizableTypeWrapper();
        SynchronizableTypeWrapper(const PassRefPtr<StoredPointerType>&);

        SynchronizableTypeWrapper& operator=(StoredPointerType*);
        operator StoredPointerType*() const;

        using Base::needsSynchronization;
        using Base::setSynchronized;

    private:
        using Base::m_value;
    };

    // SynchronizableTypeWrapperBase implementation
    template<typename StoredType>
    inline SynchronizableTypeWrapperBase<StoredType>::SynchronizableTypeWrapperBase()
        : m_value()
        , m_needsSynchronization(false)
    {
    }

    template<typename StoredType> template<typename AssignableType>
    inline void SynchronizableTypeWrapperBase<StoredType>::assign(AssignableType type)
    {
        m_value = type;
        m_needsSynchronization = true;
    }

    template<typename StoredType>
    inline bool SynchronizableTypeWrapperBase<StoredType>::needsSynchronization() const
    {
        return m_needsSynchronization;
    }

    template<typename StoredType>
    inline void SynchronizableTypeWrapperBase<StoredType>::setSynchronized()
    {
        m_needsSynchronization = false;
    }

    // SynchronizableTypeWrapper implementation for primitive types
    template<typename StoredType>
    inline SynchronizableTypeWrapper<StoredType>::SynchronizableTypeWrapper()
        : Base()
    {
    }

    template<typename StoredType> template<typename T1>
    inline SynchronizableTypeWrapper<StoredType>::SynchronizableTypeWrapper(const T1& arg1)
        : Base()
    {
        m_value = StoredType(arg1);
    }

    template<typename StoredType> template<typename T1, typename T2>
    inline SynchronizableTypeWrapper<StoredType>::SynchronizableTypeWrapper(const T1& arg1, const T2& arg2)
        : Base()
    {
        m_value = StoredType(arg1, arg2);
    }

    template<typename StoredType> template<typename T1, typename T2, typename T3>
    inline SynchronizableTypeWrapper<StoredType>::SynchronizableTypeWrapper(const T1& arg1, const T2& arg2, const T3& arg3)
        : Base()
    {
        m_value = StoredType(arg1, arg2, arg3);
    }

    template<typename StoredType>
    inline SynchronizableTypeWrapper<StoredType>& SynchronizableTypeWrapper<StoredType>::operator=(const StoredType& other)
    {
        Base::assign(other);
        return (*this);
    }

    template<typename StoredType>
    inline SynchronizableTypeWrapper<StoredType>::operator StoredType() const
    {
        return m_value;
    }

    // SynchronizableTypeWrapper implementation for refcounted types
    template<typename StoredPointerType>
    inline SynchronizableTypeWrapper<RefPtr<StoredPointerType> >::SynchronizableTypeWrapper()
        : Base()
    {
    }

    template<typename StoredPointerType>
    inline SynchronizableTypeWrapper<RefPtr<StoredPointerType> >::SynchronizableTypeWrapper(const PassRefPtr<StoredPointerType>& type)
        : Base()
    {
        Base::m_value = type;
    }

    template<typename StoredPointerType>
    inline SynchronizableTypeWrapper<RefPtr<StoredPointerType> >& SynchronizableTypeWrapper<RefPtr<StoredPointerType> >::operator=(StoredPointerType* other)
    {
        Base::assign(other);
        return (*this);
    }

    template<typename StoredPointerType>
    inline SynchronizableTypeWrapper<RefPtr<StoredPointerType> >::operator StoredPointerType*() const
    {
        return Base::m_value.get();
    }

};

#endif
#endif
