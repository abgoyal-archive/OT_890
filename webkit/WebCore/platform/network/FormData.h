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
 * Copyright (C) 2004, 2006, 2008 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB. If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef FormData_h
#define FormData_h

#include "PlatformString.h"
#include <wtf/RefCounted.h>
#include <wtf/Vector.h>

namespace WebCore {

class ChromeClient;

class FormDataElement {
public:
    FormDataElement() : m_type(data) { }
    FormDataElement(const Vector<char>& array) : m_type(data), m_data(array) { }
    FormDataElement(const String& filename, bool shouldGenerateFile) : m_type(encodedFile), m_filename(filename), m_shouldGenerateFile(shouldGenerateFile) { }

    enum { data, encodedFile } m_type;
    Vector<char> m_data;
    String m_filename;
    String m_generatedFilename;
    bool m_shouldGenerateFile;
};

inline bool operator==(const FormDataElement& a, const FormDataElement& b)
{
    if (&a == &b)
        return true;
    
    if (a.m_type != b.m_type)
        return false;
    if (a.m_data != b.m_data)
        return false;
    if (a.m_filename != b.m_filename)
        return false;

    return true;
}
 
inline bool operator!=(const FormDataElement& a, const FormDataElement& b)
{
    return !(a == b);
}
 
class FormData : public RefCounted<FormData> {
public:
    static PassRefPtr<FormData> create();
    static PassRefPtr<FormData> create(const void*, size_t);
    static PassRefPtr<FormData> create(const CString&);
    static PassRefPtr<FormData> create(const Vector<char>&);
    PassRefPtr<FormData> copy() const;
    PassRefPtr<FormData> deepCopy() const;
    ~FormData();
    
    void appendData(const void* data, size_t);
    void appendFile(const String& filename, bool shouldGenerateFile = false);

    void flatten(Vector<char>&) const; // omits files
    String flattenToString() const; // omits files

    bool isEmpty() const { return m_elements.isEmpty(); }
    const Vector<FormDataElement>& elements() const { return m_elements; }

    void generateFiles(ChromeClient*);
    void removeGeneratedFilesIfNeeded();

    bool alwaysStream() const { return m_alwaysStream; }
    void setAlwaysStream(bool alwaysStream) { m_alwaysStream = alwaysStream; }

    // Identifies a particular form submission instance.  A value of 0 is used
    // to indicate an unspecified identifier.
    void setIdentifier(int64_t identifier) { m_identifier = identifier; }
    int64_t identifier() const { return m_identifier; }

private:
    FormData();
    FormData(const FormData&);

    Vector<FormDataElement> m_elements;
    int64_t m_identifier;
    bool m_hasGeneratedFiles;
    bool m_alwaysStream;
};

inline bool operator==(const FormData& a, const FormData& b)
{
    return a.elements() == b.elements();
}

inline bool operator!=(const FormData& a, const FormData& b)
{
    return a.elements() != b.elements();
}

} // namespace WebCore

#endif
