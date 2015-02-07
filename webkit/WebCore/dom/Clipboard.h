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
 * Copyright (C) 2001 Peter Kelly (pmk@post.com)
 * Copyright (C) 2001 Tobias Anton (anton@stud.fbi.fh-darmstadt.de)
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 * Copyright (C) 2003, 2004, 2005, 2006, 2008 Apple Inc. All rights reserved.
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

#ifndef Clipboard_h
#define Clipboard_h

#include "CachedResourceHandle.h"
#include "ClipboardAccessPolicy.h"
#include "DragActions.h"
#include "DragImage.h"
#include "IntPoint.h"
#include "Node.h"

namespace WebCore {

    class FileList;

    // State available during IE's events for drag and drop and copy/paste
    class Clipboard : public RefCounted<Clipboard> {
    public:
        virtual ~Clipboard() { }

        // Is this operation a drag-drop or a copy-paste?
        bool isForDragging() const { return m_forDragging; }

        String dropEffect() const { return m_dropEffect; }
        void setDropEffect(const String&);
        String effectAllowed() const { return m_effectAllowed; }
        void setEffectAllowed(const String&);
    
        virtual void clearData(const String& type) = 0;
        virtual void clearAllData() = 0;
        virtual String getData(const String& type, bool& success) const = 0;
        virtual bool setData(const String& type, const String& data) = 0;
    
        // extensions beyond IE's API
        virtual HashSet<String> types() const = 0;
        virtual PassRefPtr<FileList> files() const = 0;

        IntPoint dragLocation() const { return m_dragLoc; }
        CachedImage* dragImage() const { return m_dragImage.get(); }
        virtual void setDragImage(CachedImage*, const IntPoint&) = 0;
        Node* dragImageElement() const { return m_dragImageElement.get(); }
        virtual void setDragImageElement(Node*, const IntPoint&) = 0;
        
        virtual DragImageRef createDragImage(IntPoint& dragLocation) const = 0;
        virtual void declareAndWriteDragImage(Element*, const KURL&, const String& title, Frame*) = 0;
        virtual void writeURL(const KURL&, const String&, Frame*) = 0;
        virtual void writeRange(Range*, Frame*) = 0;

        virtual bool hasData() = 0;
        
        void setAccessPolicy(ClipboardAccessPolicy);

        bool sourceOperation(DragOperation&) const;
        bool destinationOperation(DragOperation&) const;
        void setSourceOperation(DragOperation);
        void setDestinationOperation(DragOperation);
        
        void setDragHasStarted() { m_dragStarted = true; }
        
    protected:
        Clipboard(ClipboardAccessPolicy, bool isForDragging);

        ClipboardAccessPolicy policy() const { return m_policy; }
        bool dragStarted() const { return m_dragStarted; }
        
    private:
        ClipboardAccessPolicy m_policy;
        String m_dropEffect;
        String m_effectAllowed;
        bool m_dragStarted;
        
    protected:
        bool m_forDragging;
        IntPoint m_dragLoc;
        CachedResourceHandle<CachedImage> m_dragImage;
        RefPtr<Node> m_dragImageElement;
    };

} // namespace WebCore

#endif // Clipboard_h
