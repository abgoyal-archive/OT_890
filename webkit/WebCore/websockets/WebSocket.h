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
 * Copyright (C) 2009 Google Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WebSocket_h
#define WebSocket_h

#include "ActiveDOMObject.h"
#include "AtomicStringHash.h"
#include "EventListener.h"
#include "EventTarget.h"
#include "KURL.h"
#include <wtf/OwnPtr.h>
#include <wtf/RefCounted.h>

namespace WebCore {

class String;

class WebSocket : public RefCounted<WebSocket>, public EventTarget, public ActiveDOMObject {
public:
    static PassRefPtr<WebSocket> create(ScriptExecutionContext* context) { return adoptRef(new WebSocket(context)); }
    virtual ~WebSocket();

    enum State {
        CONNECTING = 0,
        OPEN = 1,
        CLOSED = 2
    };

    void connect(const KURL& url, ExceptionCode&);
    void connect(const KURL& url, const String& protocol, ExceptionCode&);

    bool send(const String& message, ExceptionCode&);

    void close();

    const KURL& url() const;
    State readyState() const;
    unsigned long bufferedAmount() const;

    void setOnopen(PassRefPtr<EventListener> eventListener) { m_onopen = eventListener; }
    EventListener* onopen() const { return m_onopen.get(); }
    void setOnmessage(PassRefPtr<EventListener> eventListener) { m_onmessage = eventListener; }
    EventListener* onmessage() const { return m_onmessage.get(); }
    void setOnclose(PassRefPtr<EventListener> eventListener) { m_onclose = eventListener; }
    EventListener* onclose() const { return m_onclose.get(); }

    // EventTarget
    virtual WebSocket* toWebSocket() { return this; }

    virtual ScriptExecutionContext* scriptExecutionContext() const;

    virtual void addEventListener(const AtomicString& eventType, PassRefPtr<EventListener>, bool useCapture);
    virtual void removeEventListener(const AtomicString& eventType, EventListener*, bool useCapture);
    virtual bool dispatchEvent(PassRefPtr<Event>, ExceptionCode&);

    // ActiveDOMObject
    // virtual bool hasPendingActivity() const;
    // virtual void contextDestroyed();
    // virtual bool canSuspend() const;
    // virtual void suspend();
    // virtual void resume();
    // virtual void stop();

    using RefCounted<WebSocket>::ref;
    using RefCounted<WebSocket>::deref;

private:
    WebSocket(ScriptExecutionContext*);

    virtual void refEventTarget() { ref(); }
    virtual void derefEventTarget() { deref(); }

    // WebSocketChannelClient
    void didConnect();
    void didReceiveMessage(const String& msg);
    void didClose();

    void dispatchOpenEvent();
    void dispatchMessageEvent(const String& msg);
    void dispatchCloseEvent();

    // FIXME: add WebSocketChannel.

    RefPtr<EventListener> m_onopen;
    RefPtr<EventListener> m_onmessage;
    RefPtr<EventListener> m_onclose;

    State m_state;
    KURL m_url;
    String m_protocol;
};

}  // namespace WebCore

#endif  // WebSocket_h
