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
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef BINDINGS_QT_INSTANCE_H_
#define BINDINGS_QT_INSTANCE_H_

#include <QtScript/qscriptengine.h>
#include "runtime.h"
#include "runtime_root.h"
#include <qpointer.h>
#include <qhash.h>
#include <qset.h>

namespace JSC {

namespace Bindings {

class QtClass;
class QtField;
class QtRuntimeMetaMethod;

class QtInstance : public Instance {
public:
    ~QtInstance();

    virtual Class* getClass() const;
    virtual RuntimeObjectImp* createRuntimeObject(ExecState*);

    virtual void begin();
    virtual void end();

    virtual JSValue valueOf(ExecState*) const;
    virtual JSValue defaultValue(ExecState*, PreferredPrimitiveType) const;

    virtual void mark(); // This isn't inherited

    virtual JSValue invokeMethod(ExecState*, const MethodList&, const ArgList&);

    virtual void getPropertyNames(ExecState*, PropertyNameArray&);

    JSValue stringValue(ExecState* exec) const;
    JSValue numberValue(ExecState* exec) const;
    JSValue booleanValue() const;

    QObject* getObject() const { return m_object; }

    static PassRefPtr<QtInstance> getQtInstance(QObject*, PassRefPtr<RootObject>, QScriptEngine::ValueOwnership ownership);

    virtual bool getOwnPropertySlot(JSObject*, ExecState*, const Identifier&, PropertySlot&);
    virtual void put(JSObject*, ExecState*, const Identifier&, JSValue, PutPropertySlot&);

    void removeCachedMethod(JSObject*);

    static QtInstance* getInstance(JSObject*);

private:
    static PassRefPtr<QtInstance> create(QObject *instance, PassRefPtr<RootObject> rootObject, QScriptEngine::ValueOwnership ownership)
    {
        return adoptRef(new QtInstance(instance, rootObject, ownership));
    }

    friend class QtClass;
    friend class QtField;
    QtInstance(QObject*, PassRefPtr<RootObject>, QScriptEngine::ValueOwnership ownership); // Factory produced only..
    mutable QtClass* m_class;
    QPointer<QObject> m_object;
    QObject* m_hashkey;
    mutable QHash<QByteArray, JSObject*> m_methods;
    mutable QHash<QString, QtField*> m_fields;
    mutable QtRuntimeMetaMethod* m_defaultMethod;
    QScriptEngine::ValueOwnership m_ownership;
};

} // namespace Bindings

} // namespace JSC

#endif
