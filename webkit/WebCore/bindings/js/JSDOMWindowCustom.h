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
 *  Copyright (C) 2008, 2009 Apple Inc. All rights reseved.
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
 */

#ifndef JSDOMWindowCustom_h
#define JSDOMWindowCustom_h

#include "JSDOMWindow.h"
#include "JSDOMWindowShell.h"
#include <wtf/AlwaysInline.h>

namespace WebCore {

inline JSDOMWindow* asJSDOMWindow(JSC::JSGlobalObject* globalObject)
{
    return static_cast<JSDOMWindow*>(globalObject);
}
 
inline const JSDOMWindow* asJSDOMWindow(const JSC::JSGlobalObject* globalObject)
{
    return static_cast<const JSDOMWindow*>(globalObject);
}

inline bool JSDOMWindowBase::allowsAccessFrom(const JSGlobalObject* other) const
{
    if (allowsAccessFromPrivate(other))
        return true;
    printErrorMessage(crossDomainAccessErrorMessage(other));
    return false;
}

inline bool JSDOMWindowBase::allowsAccessFrom(JSC::ExecState* exec) const
{
    if (allowsAccessFromPrivate(exec->lexicalGlobalObject()))
        return true;
    printErrorMessage(crossDomainAccessErrorMessage(exec->lexicalGlobalObject()));
    return false;
}
    
inline bool JSDOMWindowBase::allowsAccessFromNoErrorMessage(JSC::ExecState* exec) const
{
    return allowsAccessFromPrivate(exec->lexicalGlobalObject());
}
    
inline bool JSDOMWindowBase::allowsAccessFrom(JSC::ExecState* exec, String& message) const
{
    if (allowsAccessFromPrivate(exec->lexicalGlobalObject()))
        return true;
    message = crossDomainAccessErrorMessage(exec->lexicalGlobalObject());
    return false;
}
    
ALWAYS_INLINE bool JSDOMWindowBase::allowsAccessFromPrivate(const JSGlobalObject* other) const
{
    const JSDOMWindow* originWindow = asJSDOMWindow(other);
    const JSDOMWindow* targetWindow = d()->shell->window();

    if (originWindow == targetWindow)
        return true;

    const SecurityOrigin* originSecurityOrigin = originWindow->impl()->securityOrigin();
    const SecurityOrigin* targetSecurityOrigin = targetWindow->impl()->securityOrigin();

    return originSecurityOrigin->canAccess(targetSecurityOrigin);
}

}

#endif // JSDOMWindowCustom_h
