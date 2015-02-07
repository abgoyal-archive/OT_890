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
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller <mueller@kde.org>
    Copyright (C) 2004, 2005, 2006 Apple Computer, Inc.
    Copyright (C) 2009 Torch Mobile Inc. http://www.torchmobile.com/

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

    This class provides all functionality needed for loading images, style sheets and html
    pages from the web. It has a memory cache for these objects.
*/

#ifndef DocLoader_h
#define DocLoader_h

#include "CachedResource.h"
#include "CachedResourceHandle.h"
#include "CachePolicy.h"
#include "StringHash.h"
#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/ListHashSet.h>

namespace WebCore {

class CachedCSSStyleSheet;
class CachedFont;
class CachedImage;
class CachedScript;
class CachedXSLStyleSheet;
class Document;
class Frame;
class ImageLoader;
class KURL;

// The DocLoader manages the loading of scripts/images/stylesheets for a single document.
class DocLoader
{
friend class Cache;
friend class ImageLoader;

public:
    DocLoader(Document*);
    ~DocLoader();

    CachedImage* requestImage(const String& url);
    CachedCSSStyleSheet* requestCSSStyleSheet(const String& url, const String& charset);
    CachedCSSStyleSheet* requestUserCSSStyleSheet(const String& url, const String& charset);
    CachedScript* requestScript(const String& url, const String& charset);
    CachedFont* requestFont(const String& url);

#if ENABLE(XSLT)
    CachedXSLStyleSheet* requestXSLStyleSheet(const String& url);
#endif
#if ENABLE(XBL)
    CachedXBLDocument* requestXBLDocument(const String &url);
#endif

    // Logs an access denied message to the console for the specified URL.
    void printAccessDeniedMessage(const KURL& url) const;

    CachedResource* cachedResource(const String& url) const { return m_documentResources.get(url).get(); }
    
    typedef HashMap<String, CachedResourceHandle<CachedResource> > DocumentResourceMap;
    const DocumentResourceMap& allCachedResources() const { return m_documentResources; }

    bool autoLoadImages() const { return m_autoLoadImages; }
    void setAutoLoadImages(bool);
    
#ifdef ANDROID_BLOCK_NETWORK_IMAGE
    bool blockNetworkImage() const { return m_blockNetworkImage; }
    void setBlockNetworkImage(bool);
    bool shouldBlockNetworkImage(const String& url) const;
#endif

    CachePolicy cachePolicy() const;
    
    Frame* frame() const; // Can be NULL
    Document* doc() const { return m_doc; }

    void removeCachedResource(CachedResource*) const;

    void setLoadInProgress(bool);
    bool loadInProgress() const { return m_loadInProgress; }
    
    void setAllowStaleResources(bool allowStaleResources) { m_allowStaleResources = allowStaleResources; }

    void incrementRequestCount();
    void decrementRequestCount();
    int requestCount();
    
    void clearPreloads();
    void clearPendingPreloads();
    void preload(CachedResource::Type, const String& url, const String& charset, bool referencedFromBody);
    void checkForPendingPreloads();
    void printPreloadStats();
    
private:
    CachedResource* requestResource(CachedResource::Type, const String& url, const String& charset, bool isPreload = false);
    void requestPreload(CachedResource::Type, const String& url, const String& charset);

    void checkForReload(const KURL&);
    void checkCacheObjectStatus(CachedResource*);
    bool canRequest(CachedResource::Type, const KURL&);
    
    Cache* m_cache;
    HashSet<String> m_reloadedURLs;
    mutable DocumentResourceMap m_documentResources;
    Document* m_doc;
    
    int m_requestCount;

    ListHashSet<CachedResource*> m_preloads;
    struct PendingPreload {
        CachedResource::Type m_type;
        String m_url;
        String m_charset;
    };
    Vector<PendingPreload> m_pendingPreloads;
    
    //29 bits left
#ifdef ANDROID_BLOCK_NETWORK_IMAGE
    bool m_blockNetworkImage : 1;
#endif
    bool m_autoLoadImages : 1;
    bool m_loadInProgress : 1;
    bool m_allowStaleResources : 1;
};

}

#endif
