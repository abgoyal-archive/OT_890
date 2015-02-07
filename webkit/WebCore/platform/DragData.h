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
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef DragData_h
#define DragData_h

#include "ClipboardAccessPolicy.h"
#include "Color.h"
#include "DragActions.h"
#include "IntPoint.h"

#include <wtf/Forward.h>
#include <wtf/Vector.h>

#if PLATFORM(MAC)
#ifdef __OBJC__ 
#import <Foundation/Foundation.h>
#import <AppKit/NSDragging.h>
typedef id <NSDraggingInfo> DragDataRef;
#else
typedef void* DragDataRef;
#endif
#elif PLATFORM(QT)
QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE
typedef const QMimeData* DragDataRef;
#elif PLATFORM(WIN)
typedef struct IDataObject* DragDataRef;
#elif PLATFORM(WX)
typedef class wxDataObject* DragDataRef;
#elif PLATFORM(GTK)
// FIXME: this should probably be something gdk-specific
typedef void* DragDataRef;
#elif defined ANDROID
typedef void* DragDataRef;
#elif PLATFORM(CHROMIUM)
#include "DragDataRef.h"
#endif


namespace WebCore {
    
    class Clipboard;
    class Document;
    class DocumentFragment;
    class KURL;
    
#if PLATFORM(MAC)
    class PasteboardHelper;
#endif

    
    class DragData {
    public:
#if PLATFORM(MAC)
        //FIXME: In the future the WebKit functions provided by the helper class should be moved into WebCore, 
        //after which this constructor should be removed
        DragData(DragDataRef data, const IntPoint& clientPosition, const IntPoint& globalPosition,
                 DragOperation operation, PasteboardHelper*);
#else
        //clientPosition is taken to be the position of the drag event within the target window, with (0,0) at the top left
        DragData(DragDataRef data, const IntPoint& clientPosition, const IntPoint& globalPosition, DragOperation operation);
#endif
        const IntPoint& clientPosition() const { return m_clientPosition; }
        const IntPoint& globalPosition() const { return m_globalPosition; }
        DragDataRef platformData() const { return m_platformDragData; }
        DragOperation draggingSourceOperationMask() const { return m_draggingSourceOperationMask; }
        PassRefPtr<Clipboard> createClipboard(ClipboardAccessPolicy) const;
        bool containsURL() const;
        bool containsPlainText() const;
        bool containsCompatibleContent() const;
        String asURL(String* title = 0) const;
        String asPlainText() const;
        void asFilenames(Vector<String>&) const;
        Color asColor() const;
        PassRefPtr<DocumentFragment> asFragment(Document*) const;
        bool canSmartReplace() const;
        bool containsColor() const;
        bool containsFiles() const;
    private:
        IntPoint m_clientPosition;
        IntPoint m_globalPosition;
        DragDataRef m_platformDragData;
        DragOperation m_draggingSourceOperationMask;
#if PLATFORM(MAC)
        PasteboardHelper* m_pasteboardHelper;
#endif
};
    
} //namespace WebCore

#endif //!DragData_h

