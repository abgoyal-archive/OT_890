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
 * Copyright (C) 2003 Apple Computer, Inc.  All rights reserved.
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

#ifndef JAVASCRIPTCORE_BINDINGS_JNI_JSOBJECT_H
#define JAVASCRIPTCORE_BINDINGS_JNI_JSOBJECT_H

#if ENABLE(MAC_JAVA_BRIDGE)

#include <CoreFoundation/CoreFoundation.h>
#include <JavaVM/jni.h>
#include <runtime/JSValue.h>
#include <wtf/RefPtr.h>

#define jlong_to_ptr(a) ((void*)(uintptr_t)(a))
#define jlong_to_impptr(a) (static_cast<JSC::JSObject*>(((void*)(uintptr_t)(a))))
#define ptr_to_jlong(a) ((jlong)(uintptr_t)(a))

namespace JSC {

class ArgList;
class ExecState;
class JSObject;
class MarkedArgumentBuffer;

namespace Bindings {

class RootObject;

enum JSObjectCallType {
    CreateNative,
    Call,
    Eval,
    GetMember,
    SetMember,
    RemoveMember,
    GetSlot,
    SetSlot,
    ToString,
    Finalize
};

struct JSObjectCallContext
{
    JSObjectCallType type;
    jlong nativeHandle;
    jstring string;
    jobjectArray args;
    jint index;
    jobject value;
    CFRunLoopRef originatingLoop;
    jvalue result;
};

class JavaJSObject
{
public:
    JavaJSObject(jlong nativeHandle);
    
    static jlong createNative(jlong nativeHandle);
    jobject call(jstring methodName, jobjectArray args) const;
    jobject eval(jstring script) const;
    jobject getMember(jstring memberName) const;
    void setMember(jstring memberName, jobject value) const;
    void removeMember(jstring memberName) const;
    jobject getSlot(jint index) const;
    void setSlot(jint index, jobject value) const;
    jstring toString() const;
    void finalize() const;
    
    static jvalue invoke(JSObjectCallContext*);

    jobject convertValueToJObject(JSValue) const;
    JSValue convertJObjectToValue(ExecState*, jobject) const;
    void getListFromJArray(ExecState*, jobjectArray, MarkedArgumentBuffer&) const;
    
    RootObject* rootObject() const;
    
    // Must be called from the thread that will be used to access JavaScript.
    static void initializeJNIThreading();
private:
    RefPtr<RootObject> _rootObject;
    JSObject* _imp;
};


} // namespace Bindings

} // namespace JSC

extern "C" {

// The Java VM calls these functions to handle calls to methods in Java's JSObject class.
jlong KJS_JSCreateNativeJSObject(JNIEnv*, jclass, jstring jurl, jlong nativeHandle, jboolean ctx);
void KJS_JSObject_JSFinalize(JNIEnv*, jclass, jlong nativeJSObject);
jobject KJS_JSObject_JSObjectCall(JNIEnv*, jclass, jlong nativeJSObject, jstring jurl, jstring methodName, jobjectArray args, jboolean ctx);
jobject KJS_JSObject_JSObjectEval(JNIEnv*, jclass, jlong nativeJSObject, jstring jurl, jstring jscript, jboolean ctx);
jobject KJS_JSObject_JSObjectGetMember(JNIEnv*, jclass, jlong nativeJSObject, jstring jurl, jstring jname, jboolean ctx);
void KJS_JSObject_JSObjectSetMember(JNIEnv*, jclass, jlong nativeJSObject, jstring jurl, jstring jname, jobject value, jboolean ctx);
void KJS_JSObject_JSObjectRemoveMember(JNIEnv*, jclass, jlong nativeJSObject, jstring jurl, jstring jname, jboolean ctx);
jobject KJS_JSObject_JSObjectGetSlot(JNIEnv*, jclass, jlong nativeJSObject, jstring jurl, jint jindex, jboolean ctx);
void KJS_JSObject_JSObjectSetSlot(JNIEnv*, jclass, jlong nativeJSObject, jstring jurl, jint jindex, jobject value, jboolean ctx);
jstring KJS_JSObject_JSObjectToString(JNIEnv*, jclass, jlong nativeJSObject);

}

#endif // ENABLE(MAC_JAVA_BRIDGE)

#endif // JAVASCRIPTCORE_BINDINGS_JNI_JSOBJECT_H
