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

/**
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
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

#ifndef WMLInputElement_h
#define WMLInputElement_h

#if ENABLE(WML)
#include "WMLFormControlElement.h"
#include "InputElement.h"

namespace WebCore {

class FormDataList;

class WMLInputElement : public WMLFormControlElement, public InputElement {
public:
    WMLInputElement(const QualifiedName& tagName, Document*);
    virtual ~WMLInputElement();

    virtual bool isKeyboardFocusable(KeyboardEvent*) const;
    virtual bool isMouseFocusable() const;
    virtual void dispatchFocusEvent();
    virtual void dispatchBlurEvent();
    virtual void updateFocusAppearance(bool restorePreviousSelection);
    virtual void aboutToUnload();

    virtual bool shouldUseInputMethod() const { return !m_isPasswordField; }
    virtual bool isChecked() const { return false; }
    virtual bool isAutofilled() const { return false; }
    virtual bool isIndeterminate() const { return false; }
    virtual bool isTextFormControl() const { return true; }
    virtual bool isRadioButton() const { return false; }
    virtual bool isTextField() const { return true; }
    virtual bool isSearchField() const { return false; }
    virtual bool isInputTypeHidden() const { return false; }
    virtual bool isPasswordField() const { return m_isPasswordField; }
    virtual bool searchEventsShouldBeDispatched() const { return false; }

    virtual int size() const;
    virtual const AtomicString& formControlType() const;
    virtual const AtomicString& formControlName() const;
    virtual String value() const;
    virtual void setValue(const String&);
    virtual String placeholder() const { return String(); }
    virtual void setPlaceholder(const String&) { }
    virtual void setValueFromRenderer(const String&);

    virtual bool saveFormControlState(String& value) const;
    virtual void restoreFormControlState(const String&);

    virtual void select();
    virtual void accessKeyAction(bool sendToAnyElement);
    virtual void parseMappedAttribute(MappedAttribute*);

    virtual void copyNonAttributeProperties(const Element* source);

    virtual RenderObject* createRenderer(RenderArena*, RenderStyle*);
    virtual void detach();
    virtual bool appendFormData(FormDataList&, bool);
    virtual void reset();

    virtual void defaultEventHandler(Event*);
    virtual void cacheSelection(int start, int end);

    virtual String constrainValue(const String& proposedValue) const;

    virtual void documentDidBecomeActive();
    virtual bool placeholderShouldBeVisible() const;

    virtual void willMoveToNewOwnerDocument();
    virtual void didMoveToNewOwnerDocument();

    bool isConformedToInputMask(const String&);
    bool isConformedToInputMask(UChar, unsigned, bool isUserInput = true);

private:
    friend class WMLCardElement;
    void initialize();

    String validateInputMask(const String&);
    unsigned cursorPositionToMaskIndex(unsigned);

    InputElementData m_data;
    bool m_isPasswordField;
    bool m_isEmptyOk;
    String m_formatMask;
    unsigned m_numOfCharsAllowedByMask;
};

}

#endif
#endif
