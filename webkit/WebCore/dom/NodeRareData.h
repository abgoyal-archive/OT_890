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
 * Copyright (C) 2008 David Smith <catfish.man@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef NodeRareData_h
#define NodeRareData_h

#include "DynamicNodeList.h"
#include "EventListener.h"
#include "RegisteredEventListener.h"
#include "StringHash.h"
#include "QualifiedName.h"
#include <wtf/HashSet.h>
#include <wtf/PassOwnPtr.h>
#include <wtf/OwnPtr.h>

namespace WebCore {

struct NodeListsNodeData {
    typedef HashSet<DynamicNodeList*> NodeListSet;
    NodeListSet m_listsWithCaches;
    
    RefPtr<DynamicNodeList::Caches> m_childNodeListCaches;
    
    typedef HashMap<String, RefPtr<DynamicNodeList::Caches> > CacheMap;
    CacheMap m_classNodeListCaches;
    CacheMap m_nameNodeListCaches;
    
    typedef HashMap<QualifiedName, RefPtr<DynamicNodeList::Caches> > TagCacheMap;
    TagCacheMap m_tagNodeListCaches;

    static PassOwnPtr<NodeListsNodeData> create()
    {
        return new NodeListsNodeData;
    }
    
    void invalidateCaches();
    void invalidateCachesThatDependOnAttributes();
    bool isEmpty() const;

private:
    NodeListsNodeData()
        : m_childNodeListCaches(DynamicNodeList::Caches::create())
    {
    }
};
    
class NodeRareData {
public:    
    NodeRareData()
        : m_tabIndex(0)
        , m_tabIndexWasSetExplicitly(false)
        , m_isFocused(false)
        , m_needsFocusAppearanceUpdateSoonAfterAttach(false)
    {
    }

    typedef HashMap<const Node*, NodeRareData*> NodeRareDataMap;
    
    static NodeRareDataMap& rareDataMap()
    {
        static NodeRareDataMap* dataMap = new NodeRareDataMap;
        return *dataMap;
    }
    
    static NodeRareData* rareDataFromMap(const Node* node)
    {
        return rareDataMap().get(node);
    }
    
    void clearNodeLists() { m_nodeLists.clear(); }
    void setNodeLists(PassOwnPtr<NodeListsNodeData> lists) { m_nodeLists = lists; }
    NodeListsNodeData* nodeLists() const { return m_nodeLists.get(); }
    
    short tabIndex() const { return m_tabIndex; }
    void setTabIndexExplicitly(short index) { m_tabIndex = index; m_tabIndexWasSetExplicitly = true; }
    bool tabIndexSetExplicitly() const { return m_tabIndexWasSetExplicitly; }

    RegisteredEventListenerVector* listeners() { return m_eventListeners.get(); }
    RegisteredEventListenerVector& ensureListeners()
    {
        if (!m_eventListeners)
            m_eventListeners.set(new RegisteredEventListenerVector);
        return *m_eventListeners;
    }

    bool isFocused() const { return m_isFocused; }
    void setFocused(bool focused) { m_isFocused = focused; }

protected:
    // for ElementRareData
    bool needsFocusAppearanceUpdateSoonAfterAttach() const { return m_needsFocusAppearanceUpdateSoonAfterAttach; }
    void setNeedsFocusAppearanceUpdateSoonAfterAttach(bool needs) { m_needsFocusAppearanceUpdateSoonAfterAttach = needs; }

private:
    OwnPtr<NodeListsNodeData> m_nodeLists;
    OwnPtr<RegisteredEventListenerVector > m_eventListeners;
    short m_tabIndex;
    bool m_tabIndexWasSetExplicitly : 1;
    bool m_isFocused : 1;
    bool m_needsFocusAppearanceUpdateSoonAfterAttach : 1;
};

} //namespace

#endif
