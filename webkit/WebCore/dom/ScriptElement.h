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
 * Copyright (C) 2008 Nikolas Zimmermann <zimmermann@kde.org>
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

#ifndef ScriptElement_h
#define ScriptElement_h

#include "CachedResourceClient.h"
#include "CachedResourceHandle.h"

namespace WebCore {

class CachedScript;
class Element;
class ScriptElementData;
class ScriptSourceCode;

class ScriptElement {
public:
    ScriptElement() { }
    virtual ~ScriptElement() { }

    virtual String scriptContent() const = 0;

    virtual String sourceAttributeValue() const = 0;
    virtual String charsetAttributeValue() const = 0;
    virtual String typeAttributeValue() const = 0;
    virtual String languageAttributeValue() const = 0;
    virtual String forAttributeValue() const = 0;

    virtual void dispatchLoadEvent() = 0;
    virtual void dispatchErrorEvent() = 0;

    // A charset for loading the script (may be overridden by HTTP headers or a BOM).
    virtual String scriptCharset() const = 0;

    virtual bool shouldExecuteAsJavaScript() const = 0;

protected:
    // Helper functions used by our parent classes.
    static void insertedIntoDocument(ScriptElementData&, const String& sourceUrl);
    static void removedFromDocument(ScriptElementData&);
    static void childrenChanged(ScriptElementData&);
    static void finishParsingChildren(ScriptElementData&, const String& sourceUrl);
    static void handleSourceAttribute(ScriptElementData&, const String& sourceUrl);
};

// HTML/SVGScriptElement hold this struct as member variable
// and pass it to the static helper functions in ScriptElement
class ScriptElementData : private CachedResourceClient {
public:
    ScriptElementData(ScriptElement*, Element*);
    virtual ~ScriptElementData();

    bool ignoresLoadRequest() const;
    bool shouldExecuteAsJavaScript() const;

    String scriptContent() const;
    String scriptCharset() const;

    Element* element() const { return m_element; }
    bool createdByParser() const { return m_createdByParser; }
    void setCreatedByParser(bool value) { m_createdByParser = value; }
    bool haveFiredLoadEvent() const { return m_firedLoad; }
    void setHaveFiredLoadEvent(bool firedLoad) { m_firedLoad = firedLoad; }

    void requestScript(const String& sourceUrl);
    void evaluateScript(const ScriptSourceCode&);
    void stopLoadRequest();

    void execute(CachedScript*);

private:
    virtual void notifyFinished(CachedResource*);

private:
    ScriptElement* m_scriptElement;
    Element* m_element;
    CachedResourceHandle<CachedScript> m_cachedScript;
    bool m_createdByParser;
    bool m_requested;
    bool m_evaluated;
    bool m_firedLoad;
};

ScriptElement* toScriptElement(Element*);

}

#endif
