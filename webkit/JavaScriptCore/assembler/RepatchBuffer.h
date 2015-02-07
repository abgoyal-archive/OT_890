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
 * Copyright (C) 2009 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef RepatchBuffer_h
#define RepatchBuffer_h

#include <wtf/Platform.h>

#if ENABLE(ASSEMBLER)

#include <MacroAssembler.h>
#include <wtf/Noncopyable.h>

namespace JSC {

// RepatchBuffer:
//
// This class is used to modify code after code generation has been completed,
// and after the code has potentially already been executed.  This mechanism is
// used to apply optimizations to the code.
//
class RepatchBuffer {
    typedef MacroAssemblerCodePtr CodePtr;

public:
    RepatchBuffer(CodeBlock* codeBlock)
    {
        JITCode& code = codeBlock->getJITCode();
        m_start = code.start();
        m_size = code.size();

        ExecutableAllocator::makeWritable(m_start, m_size);
    }

    ~RepatchBuffer()
    {
        ExecutableAllocator::makeExecutable(m_start, m_size);
    }

    void relink(CodeLocationJump jump, CodeLocationLabel destination)
    {
        MacroAssembler::repatchJump(jump, destination);
    }

    void relink(CodeLocationCall call, CodeLocationLabel destination)
    {
        MacroAssembler::repatchCall(call, destination);
    }

    void relink(CodeLocationCall call, FunctionPtr destination)
    {
        MacroAssembler::repatchCall(call, destination);
    }

    void relink(CodeLocationNearCall nearCall, CodePtr destination)
    {
        MacroAssembler::repatchNearCall(nearCall, CodeLocationLabel(destination));
    }

    void relink(CodeLocationNearCall nearCall, CodeLocationLabel destination)
    {
        MacroAssembler::repatchNearCall(nearCall, destination);
    }

    void repatch(CodeLocationDataLabel32 dataLabel32, int32_t value)
    {
        MacroAssembler::repatchInt32(dataLabel32, value);
    }

    void repatch(CodeLocationDataLabelPtr dataLabelPtr, void* value)
    {
        MacroAssembler::repatchPointer(dataLabelPtr, value);
    }

    void repatchLoadPtrToLEA(CodeLocationInstruction instruction)
    {
        MacroAssembler::repatchLoadPtrToLEA(instruction);
    }

    void relinkCallerToTrampoline(ReturnAddressPtr returnAddress, CodeLocationLabel label)
    {
        relink(CodeLocationCall(CodePtr(returnAddress)), label);
    }
    
    void relinkCallerToTrampoline(ReturnAddressPtr returnAddress, CodePtr newCalleeFunction)
    {
        relinkCallerToTrampoline(returnAddress, CodeLocationLabel(newCalleeFunction));
    }

    void relinkCallerToFunction(ReturnAddressPtr returnAddress, FunctionPtr function)
    {
        relink(CodeLocationCall(CodePtr(returnAddress)), function);
    }
    
    void relinkNearCallerToTrampoline(ReturnAddressPtr returnAddress, CodeLocationLabel label)
    {
        relink(CodeLocationNearCall(CodePtr(returnAddress)), label);
    }
    
    void relinkNearCallerToTrampoline(ReturnAddressPtr returnAddress, CodePtr newCalleeFunction)
    {
        relinkNearCallerToTrampoline(returnAddress, CodeLocationLabel(newCalleeFunction));
    }

private:
    void* m_start;
    size_t m_size;
};

} // namespace JSC

#endif // ENABLE(ASSEMBLER)

#endif // RepatchBuffer_h
