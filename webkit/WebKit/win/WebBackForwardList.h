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
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
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

#ifndef WebBackForwardList_H
#define WebBackForwardList_H

#include "WebKit.h"

#include "WebHistoryItem.h"

#include <WTF/PassRefPtr.h>
#include <WTF/RefPtr.h>

namespace WebCore {
    class BackForwardList;
}

class WebBackForwardList : public IWebBackForwardList, IWebBackForwardListPrivate
{
public:
    static WebBackForwardList* createInstance(PassRefPtr<WebCore::BackForwardList>);
protected:
    WebBackForwardList(PassRefPtr<WebCore::BackForwardList>);
    ~WebBackForwardList();

public:
    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);

    // IWebBackForwardList
    virtual HRESULT STDMETHODCALLTYPE addItem( 
        /* [in] */ IWebHistoryItem *item);
    
    virtual HRESULT STDMETHODCALLTYPE goBack( void);
    
    virtual HRESULT STDMETHODCALLTYPE goForward( void);
    
    virtual HRESULT STDMETHODCALLTYPE goToItem( 
        /* [in] */ IWebHistoryItem *item);
    
    virtual HRESULT STDMETHODCALLTYPE backItem( 
        /* [retval][out] */ IWebHistoryItem **item);
    
    virtual HRESULT STDMETHODCALLTYPE currentItem( 
        /* [retval][out] */ IWebHistoryItem **item);
    
    virtual HRESULT STDMETHODCALLTYPE forwardItem( 
        /* [retval][out] */ IWebHistoryItem **item);
    
    virtual HRESULT STDMETHODCALLTYPE backListWithLimit( 
        /* [in] */ int limit,
        /* [out] */ int *listCount,
        /* [in] */ IWebHistoryItem **list);
    
    virtual HRESULT STDMETHODCALLTYPE forwardListWithLimit( 
        /* [in] */ int limit,
        /* [out] */ int *listCount,
        /* [in] */ IWebHistoryItem **list);
    
    virtual HRESULT STDMETHODCALLTYPE capacity( 
        /* [retval][out] */ int *result);
    
    virtual HRESULT STDMETHODCALLTYPE setCapacity( 
        /* [in] */ int size);
    
    virtual HRESULT STDMETHODCALLTYPE backListCount( 
        /* [retval][out] */ int *count);
    
    virtual HRESULT STDMETHODCALLTYPE forwardListCount( 
        /* [retval][out] */ int *sizecount);
    
    virtual HRESULT STDMETHODCALLTYPE containsItem( 
        /* [in] */ IWebHistoryItem *item,
        /* [retval][out] */ BOOL *result);
    
    virtual HRESULT STDMETHODCALLTYPE itemAtIndex( 
        /* [in] */ int index,
        /* [retval][out] */ IWebHistoryItem **item);
    
    // IWebBackForwardListPrivate
    virtual HRESULT STDMETHODCALLTYPE removeItem( 
        /* [in] */ IWebHistoryItem* item);

    // WebBackForwardList

protected:
    ULONG m_refCount;
    RefPtr<WebCore::BackForwardList> m_backForwardList;
};

#endif
