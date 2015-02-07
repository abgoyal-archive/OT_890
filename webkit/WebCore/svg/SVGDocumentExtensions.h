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
    Copyright (C) 2006 Apple Computer, Inc.
                  2006, 2008 Nikolas Zimmermann <zimmermann@kde.org>

    This file is part of the WebKit project

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

#ifndef SVGDocumentExtensions_h
#define SVGDocumentExtensions_h

#if ENABLE(SVG)
#include <memory>

#include <wtf/HashSet.h>
#include <wtf/HashMap.h>

#include "StringHash.h"
#include "StringImpl.h"
#include "SVGAnimatedTemplate.h"

namespace WebCore {

class Document;
class EventListener;
class Node;
class String;
class SVGElementInstance;
class SVGStyledElement;
class SVGSVGElement;

class SVGDocumentExtensions {
public:
    SVGDocumentExtensions(Document*);
    ~SVGDocumentExtensions();
    
    void addTimeContainer(SVGSVGElement*);
    void removeTimeContainer(SVGSVGElement*);
    
    void startAnimations();
    void pauseAnimations();
    void unpauseAnimations();

    void reportWarning(const String&);
    void reportError(const String&);

private:
    Document* m_doc; // weak reference
    HashSet<SVGSVGElement*> m_timeContainers; // For SVG 1.2 support this will need to be made more general.
    HashMap<String, HashSet<SVGStyledElement*>*> m_pendingResources;

    SVGDocumentExtensions(const SVGDocumentExtensions&);
    SVGDocumentExtensions& operator=(const SVGDocumentExtensions&);

    template<typename ValueType>
    HashMap<const SVGElement*, HashMap<StringImpl*, ValueType>*>* baseValueMap() const
    {
        static HashMap<const SVGElement*, HashMap<StringImpl*, ValueType>*>* s_baseValueMap = new HashMap<const SVGElement*, HashMap<StringImpl*, ValueType>*>();
        return s_baseValueMap;
    }

public:
    // This HashMap contains a list of pending resources. Pending resources, are such
    // which are referenced by any object in the SVG document, but do NOT exist yet.
    // For instance, dynamically build gradients / patterns / clippers...
    void addPendingResource(const AtomicString& id, SVGStyledElement*);
    bool isPendingResource(const AtomicString& id) const;
    std::auto_ptr<HashSet<SVGStyledElement*> > removePendingResource(const AtomicString& id);

    // Used by the ANIMATED_PROPERTY_* macros
    template<typename ValueType>
    ValueType baseValue(const SVGElement* element, const AtomicString& propertyName) const
    {
        HashMap<StringImpl*, ValueType>* propertyMap = baseValueMap<ValueType>()->get(element);
        if (propertyMap)
            return propertyMap->get(propertyName.impl());

        return SVGAnimatedTypeValue<ValueType>::null();
    }

    template<typename ValueType>
    void setBaseValue(const SVGElement* element, const AtomicString& propertyName, ValueType newValue)
    {
        HashMap<StringImpl*, ValueType>* propertyMap = baseValueMap<ValueType>()->get(element);
        if (!propertyMap) {
            propertyMap = new HashMap<StringImpl*, ValueType>();
            baseValueMap<ValueType>()->set(element, propertyMap);
        }

        propertyMap->set(propertyName.impl(), newValue);
    }

    template<typename ValueType>
    void removeBaseValue(const SVGElement* element, const AtomicString& propertyName)
    {
        HashMap<StringImpl*, ValueType>* propertyMap = baseValueMap<ValueType>()->get(element);
        if (!propertyMap)
            return;

        propertyMap->remove(propertyName.impl());
    }

    template<typename ValueType>
    bool hasBaseValue(const SVGElement* element, const AtomicString& propertyName) const
    {
        HashMap<StringImpl*, ValueType>* propertyMap = baseValueMap<ValueType>()->get(element);
        if (propertyMap)
            return propertyMap->contains(propertyName.impl());

        return false;
    }
};

}

#endif // ENABLE(SVG)
#endif
