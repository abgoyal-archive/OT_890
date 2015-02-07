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
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
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

#ifndef JSClassRef_h
#define JSClassRef_h

#include "JSObjectRef.h"

#include <runtime/JSObject.h>
#include <runtime/Protect.h>
#include <runtime/UString.h>
#include <wtf/HashMap.h>
#include <wtf/RefCounted.h>

struct StaticValueEntry : FastAllocBase {
    StaticValueEntry(JSObjectGetPropertyCallback _getProperty, JSObjectSetPropertyCallback _setProperty, JSPropertyAttributes _attributes)
        : getProperty(_getProperty), setProperty(_setProperty), attributes(_attributes)
    {
    }
    
    JSObjectGetPropertyCallback getProperty;
    JSObjectSetPropertyCallback setProperty;
    JSPropertyAttributes attributes;
};

struct StaticFunctionEntry : FastAllocBase {
    StaticFunctionEntry(JSObjectCallAsFunctionCallback _callAsFunction, JSPropertyAttributes _attributes)
        : callAsFunction(_callAsFunction), attributes(_attributes)
    {
    }

    JSObjectCallAsFunctionCallback callAsFunction;
    JSPropertyAttributes attributes;
};

typedef HashMap<RefPtr<JSC::UString::Rep>, StaticValueEntry*> OpaqueJSClassStaticValuesTable;
typedef HashMap<RefPtr<JSC::UString::Rep>, StaticFunctionEntry*> OpaqueJSClassStaticFunctionsTable;

struct OpaqueJSClass;

// An OpaqueJSClass (JSClass) is created without a context, so it can be used with any context, even across context groups.
// This structure holds data members that vary across context groups.
struct OpaqueJSClassContextData : Noncopyable {
    OpaqueJSClassContextData(OpaqueJSClass*);
    ~OpaqueJSClassContextData();

    // It is necessary to keep OpaqueJSClass alive because of the following rare scenario:
    // 1. A class is created and used, so its context data is stored in JSGlobalData hash map.
    // 2. The class is released, and when all JS objects that use it are collected, OpaqueJSClass
    // is deleted (that's the part prevented by this RefPtr).
    // 3. Another class is created at the same address.
    // 4. When it is used, the old context data is found in JSGlobalData and used.
    RefPtr<OpaqueJSClass> m_class;

    OpaqueJSClassStaticValuesTable* staticValues;
    OpaqueJSClassStaticFunctionsTable* staticFunctions;
    JSC::JSObject* cachedPrototype;
};

struct OpaqueJSClass : public ThreadSafeShared<OpaqueJSClass> {
    static PassRefPtr<OpaqueJSClass> create(const JSClassDefinition*);
    static PassRefPtr<OpaqueJSClass> createNoAutomaticPrototype(const JSClassDefinition*);
    ~OpaqueJSClass();
    
    JSC::UString className();
    OpaqueJSClassStaticValuesTable* staticValues(JSC::ExecState*);
    OpaqueJSClassStaticFunctionsTable* staticFunctions(JSC::ExecState*);
    JSC::JSObject* prototype(JSC::ExecState*);

    OpaqueJSClass* parentClass;
    OpaqueJSClass* prototypeClass;
    
    JSObjectInitializeCallback initialize;
    JSObjectFinalizeCallback finalize;
    JSObjectHasPropertyCallback hasProperty;
    JSObjectGetPropertyCallback getProperty;
    JSObjectSetPropertyCallback setProperty;
    JSObjectDeletePropertyCallback deleteProperty;
    JSObjectGetPropertyNamesCallback getPropertyNames;
    JSObjectCallAsFunctionCallback callAsFunction;
    JSObjectCallAsConstructorCallback callAsConstructor;
    JSObjectHasInstanceCallback hasInstance;
    JSObjectConvertToTypeCallback convertToType;

private:
    friend struct OpaqueJSClassContextData;

    OpaqueJSClass();
    OpaqueJSClass(const OpaqueJSClass&);
    OpaqueJSClass(const JSClassDefinition*, OpaqueJSClass* protoClass);

    OpaqueJSClassContextData& contextData(JSC::ExecState*);

    // UStrings in these data members should not be put into any IdentifierTable.
    JSC::UString m_className;
    OpaqueJSClassStaticValuesTable* m_staticValues;
    OpaqueJSClassStaticFunctionsTable* m_staticFunctions;
};

#endif // JSClassRef_h
