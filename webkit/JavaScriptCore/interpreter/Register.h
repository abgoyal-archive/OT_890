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
 * Copyright (C) 2008, 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef Register_h
#define Register_h

#include "JSValue.h"
#include <wtf/Assertions.h>
#include <wtf/FastAllocBase.h>
#include <wtf/VectorTraits.h>

namespace JSC {

    class Arguments;
    class CodeBlock;
    class ExecState;
    class JSActivation;
    class JSFunction;
    class JSPropertyNameIterator;
    class ScopeChainNode;

    struct Instruction;

    typedef ExecState CallFrame;

    class Register : public WTF::FastAllocBase {
    public:
        Register();
        Register(JSValue);

        JSValue jsValue() const;

        bool marked() const;
        void markChildren(MarkStack&);
        
        Register(JSActivation*);
        Register(CallFrame*);
        Register(CodeBlock*);
        Register(JSFunction*);
        Register(JSPropertyNameIterator*);
        Register(ScopeChainNode*);
        Register(Instruction*);

        int32_t i() const;
        JSActivation* activation() const;
        Arguments* arguments() const;
        CallFrame* callFrame() const;
        CodeBlock* codeBlock() const;
        JSFunction* function() const;
        JSPropertyNameIterator* propertyNameIterator() const;
        ScopeChainNode* scopeChain() const;
        Instruction* vPC() const;

        static Register withInt(int32_t i)
        {
            return Register(i);
        }

    private:
        Register(int32_t);

        union {
            int32_t i;
            EncodedJSValue value;

            JSActivation* activation;
            CallFrame* callFrame;
            CodeBlock* codeBlock;
            JSFunction* function;
            JSPropertyNameIterator* propertyNameIterator;
            ScopeChainNode* scopeChain;
            Instruction* vPC;
        } u;
    };

    ALWAYS_INLINE Register::Register()
    {
#ifndef NDEBUG
        u.value = JSValue::encode(JSValue());
#endif
    }

    ALWAYS_INLINE Register::Register(JSValue v)
    {
        u.value = JSValue::encode(v);
    }

    ALWAYS_INLINE JSValue Register::jsValue() const
    {
        return JSValue::decode(u.value);
    }
    
    ALWAYS_INLINE bool Register::marked() const
    {
        return jsValue().marked();
    }

    // Interpreter functions

    ALWAYS_INLINE Register::Register(JSActivation* activation)
    {
        u.activation = activation;
    }

    ALWAYS_INLINE Register::Register(CallFrame* callFrame)
    {
        u.callFrame = callFrame;
    }

    ALWAYS_INLINE Register::Register(CodeBlock* codeBlock)
    {
        u.codeBlock = codeBlock;
    }

    ALWAYS_INLINE Register::Register(JSFunction* function)
    {
        u.function = function;
    }

    ALWAYS_INLINE Register::Register(Instruction* vPC)
    {
        u.vPC = vPC;
    }

    ALWAYS_INLINE Register::Register(ScopeChainNode* scopeChain)
    {
        u.scopeChain = scopeChain;
    }

    ALWAYS_INLINE Register::Register(JSPropertyNameIterator* propertyNameIterator)
    {
        u.propertyNameIterator = propertyNameIterator;
    }

    ALWAYS_INLINE Register::Register(int32_t i)
    {
        u.i = i;
    }

    ALWAYS_INLINE int32_t Register::i() const
    {
        return u.i;
    }
    
    ALWAYS_INLINE JSActivation* Register::activation() const
    {
        return u.activation;
    }
    
    ALWAYS_INLINE CallFrame* Register::callFrame() const
    {
        return u.callFrame;
    }
    
    ALWAYS_INLINE CodeBlock* Register::codeBlock() const
    {
        return u.codeBlock;
    }
    
    ALWAYS_INLINE JSFunction* Register::function() const
    {
        return u.function;
    }
    
    ALWAYS_INLINE JSPropertyNameIterator* Register::propertyNameIterator() const
    {
        return u.propertyNameIterator;
    }
    
    ALWAYS_INLINE ScopeChainNode* Register::scopeChain() const
    {
        return u.scopeChain;
    }
    
    ALWAYS_INLINE Instruction* Register::vPC() const
    {
        return u.vPC;
    }

} // namespace JSC

namespace WTF {

    template<> struct VectorTraits<JSC::Register> : VectorTraitsBase<true, JSC::Register> { };

} // namespace WTF

#endif // Register_h
