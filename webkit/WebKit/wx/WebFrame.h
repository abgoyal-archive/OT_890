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
 * Copyright (C) 2007 Kevin Ollivier <kevino@theolliviers.com>
 *
 * All rights reserved.
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
 
#ifndef WXWEBFRAME_H
#define WXWEBFRAME_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

class WebFramePrivate;
class WebViewFrameData;
class wxWebView;

namespace WebCore {
    class ChromeClientWx;
    class FrameLoaderClientWx;
    class EditorClientWx;
    class Frame;
}

#ifndef SWIG

#if !wxCHECK_VERSION(2,9,0) && wxCHECK_GCC_VERSION(4,0)
#define WXDLLIMPEXP_WEBKIT __attribute__ ((visibility("default")))
#elif WXMAKINGDLL_WEBKIT
#define WXDLLIMPEXP_WEBKIT WXEXPORT
#elif defined(WXUSINGDLL_WEBKIT)
#define WXDLLIMPEXP_WEBKIT WXIMPORT
#endif

#else
#define WXDLLIMPEXP_WEBKIT
#endif // SWIG

class WXDLLIMPEXP_WEBKIT wxWebViewDOMElementInfo
{
public:
    wxWebViewDOMElementInfo();

    ~wxWebViewDOMElementInfo() { }

    wxString GetTagName() const { return m_tagName; }
    void SetTagName(const wxString& name) { m_tagName = name; }

    bool IsSelected() const { return m_isSelected; }
    void SetSelected(bool sel) { m_isSelected = sel; }

    wxString GetText() const { return m_text; }
    void SetText(const wxString& text) { m_text = text; }

    wxString GetImageSrc() const { return m_imageSrc; }
    void SetImageSrc(const wxString& src) { m_imageSrc = src; }

    wxString GetLink() const { return m_link; }
    void SetLink(const wxString& link) { m_link = link; }

private:
    void* m_domElement;
    bool m_isSelected;
    wxString m_tagName;
    wxString m_text;
    wxString m_imageSrc;
    wxString m_link;
};

class WXDLLIMPEXP_WEBKIT wxWebFrame
{
    // ChromeClientWx needs to get the Page* stored by the wxWebView
    // for the createWindow function. 
    friend class WebCore::ChromeClientWx;
    friend class WebCore::FrameLoaderClientWx;
    friend class WebCore::EditorClientWx;
    friend class wxWebView;

public:
    wxWebFrame(wxWebView* container, wxWebFrame* parent = NULL, WebViewFrameData* data = NULL);
    
    ~wxWebFrame();
    
    void LoadURL(const wxString& url);
    bool GoBack();
    bool GoForward();
    void Stop();
    void Reload();
    
    bool CanGoBack();
    bool CanGoForward();
    
    bool CanCut();
    bool CanCopy();
    bool CanPaste();
    
    void Cut();
    void Copy();
    void Paste();
    
    bool CanUndo();
    bool CanRedo();
    
    void Undo();
    void Redo();
    
    wxString GetPageSource();
    void SetPageSource(const wxString& source, const wxString& baseUrl = wxEmptyString);
    
    wxString GetInnerText();
    wxString GetAsMarkup();
    wxString GetExternalRepresentation();
    
    wxString RunScript(const wxString& javascript);
    
    bool FindString(const wxString& string, bool forward = true,
        bool caseSensitive = false, bool wrapSelection = true,
        bool startInSelection = true);
    
    bool CanIncreaseTextSize() const;
    void IncreaseTextSize();
    bool CanDecreaseTextSize() const;
    void DecreaseTextSize();
    void ResetTextSize();
    void MakeEditable(bool enable);
    bool IsEditable() const { return m_isEditable; }

    wxString GetPageTitle() const { return m_title; }
    void SetPageTitle(const wxString& title) { m_title = title; }
    
    WebCore::Frame* GetFrame();

    wxWebViewDOMElementInfo HitTest(const wxPoint& post) const;
    
private:
    float m_textMagnifier;
    bool m_isEditable;
    bool m_isInitialized;
    bool m_beingDestroyed;
    WebFramePrivate* m_impl;
    wxString m_title;
    
};

#endif // ifndef WXWEBFRAME_H
