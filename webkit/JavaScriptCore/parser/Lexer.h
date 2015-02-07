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
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef Lexer_h
#define Lexer_h

#include "Lookup.h"
#include "SourceCode.h"
#include <wtf/ASCIICType.h>
#include <wtf/SegmentedVector.h>
#include <wtf/Vector.h>
#include <wtf/unicode/Unicode.h>

namespace JSC {

    class RegExp;

    class Lexer : public Noncopyable {
    public:
        // Character manipulation functions.
        static bool isWhiteSpace(int character);
        static bool isLineTerminator(int character);
        static unsigned char convertHex(int c1, int c2);
        static UChar convertUnicode(int c1, int c2, int c3, int c4);

        // Functions to set up parsing.
        void setCode(const SourceCode&);
        void setIsReparsing() { m_isReparsing = true; }

        // Functions for the parser itself.
        int lex(void* lvalp, void* llocp);
        int lineNumber() const { return m_lineNumber; }
        bool prevTerminator() const { return m_terminator; }
        SourceCode sourceCode(int openBrace, int closeBrace, int firstLine);
        bool scanRegExp();
        const UString& pattern() const { return m_pattern; }
        const UString& flags() const { return m_flags; }

        // Functions for use after parsing.
        bool sawError() const { return m_error; }
        void clear();

    private:
        friend class JSGlobalData;

        Lexer(JSGlobalData*);
        ~Lexer();

        void shift1();
        void shift2();
        void shift3();
        void shift4();
        void shiftLineTerminator();

        void record8(int);
        void record16(int);
        void record16(UChar);

        void copyCodeWithoutBOMs();

        int currentOffset() const;
        const UChar* currentCharacter() const;

        JSC::Identifier* makeIdentifier(const UChar* buffer, size_t length);

        bool lastTokenWasRestrKeyword() const;

        static const size_t initialReadBufferCapacity = 32;
        static const size_t initialIdentifierTableCapacity = 64;

        int m_lineNumber;

        Vector<char> m_buffer8;
        Vector<UChar> m_buffer16;
        bool m_terminator;
        bool m_delimited; // encountered delimiter like "'" and "}" on last run
        int m_lastToken;

        const SourceCode* m_source;
        const UChar* m_code;
        const UChar* m_codeStart;
        const UChar* m_codeEnd;
        bool m_isReparsing;
        bool m_atLineStart;
        bool m_error;

        // current and following unicode characters (int to allow for -1 for end-of-file marker)
        int m_current;
        int m_next1;
        int m_next2;
        int m_next3;
        
        WTF::SegmentedVector<JSC::Identifier, initialIdentifierTableCapacity> m_identifiers;

        JSGlobalData* m_globalData;

        UString m_pattern;
        UString m_flags;

        const HashTable m_keywordTable;

        Vector<UChar> m_codeWithoutBOMs;
    };

    inline bool Lexer::isWhiteSpace(int ch)
    {
        return isASCII(ch) ? (ch == ' ' || ch == '\t' || ch == 0xB || ch == 0xC) : WTF::Unicode::isSeparatorSpace(ch);
    }

    inline bool Lexer::isLineTerminator(int ch)
    {
        return ch == '\r' || ch == '\n' || (ch & ~1) == 0x2028;
    }

    inline unsigned char Lexer::convertHex(int c1, int c2)
    {
        return (toASCIIHexValue(c1) << 4) | toASCIIHexValue(c2);
    }

    inline UChar Lexer::convertUnicode(int c1, int c2, int c3, int c4)
    {
        return (convertHex(c1, c2) << 8) | convertHex(c3, c4);
    }

} // namespace JSC

#endif // Lexer_h
