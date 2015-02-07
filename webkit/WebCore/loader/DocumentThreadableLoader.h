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
 * Copyright (C) 2009 Google Inc. All rights reserved.
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

#ifndef DocumentThreadableLoader_h
#define DocumentThreadableLoader_h

#include "SubresourceLoaderClient.h"
#include "ThreadableLoader.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>

namespace WebCore {
    class Document;
    struct ResourceRequest;
    class ThreadableLoaderClient;

    class DocumentThreadableLoader : public RefCounted<DocumentThreadableLoader>, public ThreadableLoader, private SubresourceLoaderClient  {
    public:
        static void loadResourceSynchronously(Document*, const ResourceRequest&, ThreadableLoaderClient&, StoredCredentials);
        static PassRefPtr<DocumentThreadableLoader> create(Document*, ThreadableLoaderClient*, const ResourceRequest&, LoadCallbacks, ContentSniff, StoredCredentials, CrossOriginRedirectPolicy);
        virtual ~DocumentThreadableLoader();

        virtual void cancel();

        using RefCounted<DocumentThreadableLoader>::ref;
        using RefCounted<DocumentThreadableLoader>::deref;

    protected:
        virtual void refThreadableLoader() { ref(); }
        virtual void derefThreadableLoader() { deref(); }

    private:
        DocumentThreadableLoader(Document*, ThreadableLoaderClient*, const ResourceRequest&, LoadCallbacks, ContentSniff, StoredCredentials, CrossOriginRedirectPolicy);
        virtual void willSendRequest(SubresourceLoader*, ResourceRequest&, const ResourceResponse& redirectResponse);
        virtual void didSendData(SubresourceLoader*, unsigned long long bytesSent, unsigned long long totalBytesToBeSent);

        virtual void didReceiveResponse(SubresourceLoader*, const ResourceResponse&);
        virtual void didReceiveData(SubresourceLoader*, const char*, int lengthReceived);
        virtual void didFinishLoading(SubresourceLoader*);
        virtual void didFail(SubresourceLoader*, const ResourceError&);

        virtual bool getShouldUseCredentialStorage(SubresourceLoader*, bool& shouldUseCredentialStorage);
        virtual void didReceiveAuthenticationChallenge(SubresourceLoader*, const AuthenticationChallenge&);
        virtual void receivedCancellation(SubresourceLoader*, const AuthenticationChallenge&);

        RefPtr<SubresourceLoader> m_loader;
        ThreadableLoaderClient* m_client;
        Document* m_document;
        bool m_allowStoredCredentials;
        bool m_sameOriginRequest;
        bool m_denyCrossOriginRedirect;
    };

} // namespace WebCore

#endif // DocumentThreadableLoader_h
