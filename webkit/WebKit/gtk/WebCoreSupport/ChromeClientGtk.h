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
 * Copyright (C) 2007 Holger Hans Peter Freyther
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
 */

#ifndef ChromeClientGtk_h
#define ChromeClientGtk_h

#include "ChromeClient.h"
#include "KURL.h"

typedef struct _WebKitWebView WebKitWebView;

namespace WebKit {

    class ChromeClient : public WebCore::ChromeClient {
    public:
        ChromeClient(WebKitWebView*);
        WebKitWebView* webView() const { return m_webView; }

        virtual void chromeDestroyed();

        virtual void setWindowRect(const WebCore::FloatRect&);
        virtual WebCore::FloatRect windowRect();

        virtual WebCore::FloatRect pageRect();

        virtual float scaleFactor();

        virtual void focus();
        virtual void unfocus();

        virtual bool canTakeFocus(WebCore::FocusDirection);
        virtual void takeFocus(WebCore::FocusDirection);

        virtual WebCore::Page* createWindow(WebCore::Frame*, const WebCore::FrameLoadRequest&, const WebCore::WindowFeatures&);
        virtual void show();

        virtual bool canRunModal();
        virtual void runModal();

        virtual void setToolbarsVisible(bool);
        virtual bool toolbarsVisible();

        virtual void setStatusbarVisible(bool);
        virtual bool statusbarVisible();

        virtual void setScrollbarsVisible(bool);
        virtual bool scrollbarsVisible();

        virtual void setMenubarVisible(bool);
        virtual bool menubarVisible();

        virtual void setResizable(bool);

        virtual void addMessageToConsole(WebCore::MessageSource source, WebCore::MessageType type,
                                         WebCore::MessageLevel level, const WebCore::String& message,
                                         unsigned int lineNumber, const WebCore::String& sourceID);

        virtual bool canRunBeforeUnloadConfirmPanel();
        virtual bool runBeforeUnloadConfirmPanel(const WebCore::String& message, WebCore::Frame* frame);

        virtual void closeWindowSoon();

        virtual void runJavaScriptAlert(WebCore::Frame*, const WebCore::String&);
        virtual bool runJavaScriptConfirm(WebCore::Frame*, const WebCore::String&);
        virtual bool runJavaScriptPrompt(WebCore::Frame*, const WebCore::String& message, const WebCore::String& defaultValue, WebCore::String& result);
        virtual void setStatusbarText(const WebCore::String&);
        virtual bool shouldInterruptJavaScript();
        virtual bool tabsToLinks() const;

        virtual WebCore::IntRect windowResizerRect() const;

        virtual void repaint(const WebCore::IntRect&, bool contentChanged, bool immediate = false, bool repaintContentOnly = false);
        virtual void scroll(const WebCore::IntSize& scrollDelta, const WebCore::IntRect& rectToScroll, const WebCore::IntRect& clipRect);
        virtual WebCore::IntPoint screenToWindow(const WebCore::IntPoint&) const;
        virtual WebCore::IntRect windowToScreen(const WebCore::IntRect&) const;
        virtual PlatformWidget platformWindow() const;
        virtual void contentsSizeChanged(WebCore::Frame*, const WebCore::IntSize&) const;

        virtual void mouseDidMoveOverElement(const WebCore::HitTestResult&, unsigned modifierFlags);

        virtual void setToolTip(const WebCore::String&, WebCore::TextDirection);

        virtual void print(WebCore::Frame*);
#if ENABLE(DATABASE)
        virtual void exceededDatabaseQuota(WebCore::Frame*, const WebCore::String&);
#endif
#if ENABLE(OFFLINE_WEB_APPLICATIONS)
        virtual void reachedMaxAppCacheSize(int64_t spaceNeeded);
#endif
        virtual void runOpenPanel(WebCore::Frame*, PassRefPtr<WebCore::FileChooser>);

        virtual void formStateDidChange(const WebCore::Node*) { }

        virtual PassOwnPtr<WebCore::HTMLParserQuirks> createHTMLParserQuirks() { return 0; }

        virtual bool setCursor(WebCore::PlatformCursorHandle);

        virtual void scrollRectIntoView(const WebCore::IntRect&, const WebCore::ScrollView*) const {}
        virtual void requestGeolocationPermissionForFrame(WebCore::Frame*, WebCore::Geolocation*);

    private:
        WebKitWebView* m_webView;
        WebCore::KURL m_hoveredLinkURL;
    };
}

#endif // ChromeClient_h
