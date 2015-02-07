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
    Copyright (C) 2004, 2005, 2006, 2008 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>

    This file is part of the KDE project

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

#ifndef SVGList_h
#define SVGList_h

#if ENABLE(SVG)
#include "ExceptionCode.h"
#include "SVGListTraits.h"

#include <wtf/RefCounted.h>
#include <wtf/PassRefPtr.h>
#include <wtf/Vector.h>

namespace WebCore {

    class QualifiedName;

    template<typename Item>
    struct SVGListTypeOperations {
        static Item nullItem()
        {
            return SVGListTraits<UsesDefaultInitializer<Item>::value, Item>::nullItem();
        }
    };

    template<typename Item>
    class SVGList : public RefCounted<SVGList<Item> > {
    private:
        typedef SVGListTypeOperations<Item> TypeOperations;

    public:
        virtual ~SVGList() { }

        const QualifiedName& associatedAttributeName() const { return m_associatedAttributeName; }

        unsigned int numberOfItems() const { return m_vector.size(); }
        void clear(ExceptionCode &) { m_vector.clear(); }

        Item initialize(Item newItem, ExceptionCode& ec)
        {
            clear(ec);
            return appendItem(newItem, ec);
        }

        Item getFirst() const
        {
            ExceptionCode ec = 0;
            return getItem(0, ec);
        }

        Item getLast() const
        {
            ExceptionCode ec = 0;
            return getItem(m_vector.size() - 1, ec);
        }

        Item getItem(unsigned int index, ExceptionCode& ec)
        {
            if (index >= m_vector.size()) {
                ec = INDEX_SIZE_ERR;
                return TypeOperations::nullItem();
            }

            return m_vector.at(index);
        }

        const Item getItem(unsigned int index, ExceptionCode& ec) const
        {
            if (index >= m_vector.size()) {
                ec = INDEX_SIZE_ERR;
                return TypeOperations::nullItem();
            }

            return m_vector[index];
        }

        Item insertItemBefore(Item newItem, unsigned int index, ExceptionCode&)
        {
            if (index < m_vector.size()) {
                m_vector.insert(index, newItem);
            } else {
                m_vector.append(newItem);
            }
            return newItem;
        }

        Item replaceItem(Item newItem, unsigned int index, ExceptionCode& ec)
        {
            if (index >= m_vector.size()) {
                ec = INDEX_SIZE_ERR;
                return TypeOperations::nullItem();
            }

            m_vector[index] = newItem;
            return newItem;
        }

        Item removeItem(unsigned int index, ExceptionCode& ec)
        {
            if (index >= m_vector.size()) {
                ec = INDEX_SIZE_ERR;
                return TypeOperations::nullItem();
            }

            Item item = m_vector[index];
            m_vector.remove(index);
            return item;
        }

        Item appendItem(Item newItem, ExceptionCode&)
        {
            m_vector.append(newItem);
            return newItem;
        }

    protected:
        SVGList(const QualifiedName& attributeName) 
            : m_associatedAttributeName(attributeName)
        {
        }

    private:
        Vector<Item> m_vector;
        const QualifiedName& m_associatedAttributeName;
    };

    template<typename Item>
    class SVGPODListItem : public RefCounted<SVGPODListItem<Item> > {
    public:
        static PassRefPtr<SVGPODListItem> create() { return adoptRef(new SVGPODListItem); }
        static PassRefPtr<SVGPODListItem> copy(const Item& item) { return adoptRef(new SVGPODListItem(item)); }

        operator Item&() { return m_item; }
        operator const Item&() const { return m_item; }

        // Updating facilities, used by JSSVGPODTypeWrapperCreatorForList
        Item value() const { return m_item; }
        void setValue(Item newItem) { m_item = newItem; }

    private:
        SVGPODListItem() : m_item() { }
        SVGPODListItem(const Item& item) : RefCounted<SVGPODListItem<Item> >(), m_item(item) { }
        
        Item m_item;
    };

    template<typename Item>
    class SVGPODList : public SVGList<RefPtr<SVGPODListItem<Item> > > {
    public:
        Item initialize(Item newItem, ExceptionCode& ec)
        {
            SVGPODListItem<Item>* ptr(SVGList<RefPtr<SVGPODListItem<Item> > >::initialize(SVGPODListItem<Item>::copy(newItem), ec).get());
            if (!ptr)
                return Item();

            return static_cast<const Item&>(*ptr); 
        }

        Item getFirst() const
        {
            SVGPODListItem<Item>* ptr(SVGList<RefPtr<SVGPODListItem<Item> > >::getFirst().get());
            if (!ptr)
                return Item();

            return static_cast<const Item&>(*ptr);
        }

        Item getLast() const
        {
            SVGPODListItem<Item>* ptr(SVGList<RefPtr<SVGPODListItem<Item> > >::getLast().get());
            if (!ptr)
                return Item();

            return static_cast<const Item&>(*ptr); 
        }

        Item getItem(unsigned int index, ExceptionCode& ec)
        {
            SVGPODListItem<Item>* ptr(SVGList<RefPtr<SVGPODListItem<Item> > >::getItem(index, ec).get());
            if (!ptr)
                return Item();

            return static_cast<const Item&>(*ptr);
        }

        const Item getItem(unsigned int index, ExceptionCode& ec) const
        {
            SVGPODListItem<Item>* ptr(SVGList<RefPtr<SVGPODListItem<Item> > >::getItem(index, ec).get());
             if (!ptr)
                return Item();

            return static_cast<const Item&>(*ptr);
        }

        Item insertItemBefore(Item newItem, unsigned int index, ExceptionCode& ec)
        {
            SVGPODListItem<Item>* ptr(SVGList<RefPtr<SVGPODListItem<Item> > >::insertItemBefore(SVGPODListItem<Item>::copy(newItem), index, ec).get());
            if (!ptr)
                return Item();

            return static_cast<const Item&>(*ptr); 
        }

        Item replaceItem(Item newItem, unsigned int index, ExceptionCode& ec)
        {
            SVGPODListItem<Item>* ptr(SVGList<RefPtr<SVGPODListItem<Item> > >::replaceItem(SVGPODListItem<Item>::copy(newItem), index, ec).get());
            if (!ptr)
                return Item();

            return static_cast<const Item&>(*ptr);
        }

        Item removeItem(unsigned int index, ExceptionCode& ec)
        {
            SVGPODListItem<Item>* ptr(SVGList<RefPtr<SVGPODListItem<Item> > >::removeItem(index, ec).get());
            if (!ptr)
                return Item();

            return static_cast<const Item&>(*ptr); 
        }

        Item appendItem(Item newItem, ExceptionCode& ec)
        {
            SVGPODListItem<Item>* ptr(SVGList<RefPtr<SVGPODListItem<Item> > >::appendItem(SVGPODListItem<Item>::copy(newItem), ec).get());
            if (!ptr)
                return Item();

            return static_cast<const Item&>(*ptr); 
        }
        
    protected:
        SVGPODList(const QualifiedName& attributeName) 
            : SVGList<RefPtr<SVGPODListItem<Item> > >(attributeName) { }
    };

} // namespace WebCore

#endif // ENABLE(SVG)
#endif // SVGList_h
