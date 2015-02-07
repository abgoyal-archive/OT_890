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

#ifndef LiteralParser_h
#define LiteralParser_h

#include "JSGlobalObjectFunctions.h"
#include "JSValue.h"
#include "UString.h"

namespace JSC {

    class LiteralParser {
    public:
        typedef enum { StrictJSON, NonStrictJSON } ParserMode;
        LiteralParser(ExecState* exec, const UString& s, ParserMode mode)
            : m_exec(exec)
            , m_lexer(s, mode)
            , m_mode(mode)
        {
        }
        
        JSValue tryLiteralParse()
        {
            m_lexer.next();
            JSValue result = parse(m_mode == StrictJSON ? StartParseExpression : StartParseStatement);
            if (m_lexer.currentToken().type != TokEnd)
                return JSValue();
            return result;
        }
    private:
        enum ParserState { StartParseObject, StartParseArray, StartParseExpression, 
                           StartParseStatement, StartParseStatementEndStatement, 
                           DoParseObjectStartExpression, DoParseObjectEndExpression,
                           DoParseArrayStartExpression, DoParseArrayEndExpression };
        enum TokenType { TokLBracket, TokRBracket, TokLBrace, TokRBrace, 
                         TokString, TokIdentifier, TokNumber, TokColon, 
                         TokLParen, TokRParen, TokComma, TokTrue, TokFalse,
                         TokNull, TokEnd, TokError };

        class Lexer {
        public:
            struct LiteralParserToken {
                TokenType type;
                const UChar* start;
                const UChar* end;
                UString stringToken;
                double numberToken;
            };
            Lexer(const UString& s, ParserMode mode)
                : m_string(s)
                , m_mode(mode)
                , m_ptr(s.data())
                , m_end(s.data() + s.size())
            {
            }
            
            TokenType next()
            {
                return lex(m_currentToken);
            }
            
            const LiteralParserToken& currentToken()
            {
                return m_currentToken;
            }
            
        private:
            TokenType lex(LiteralParserToken&);
            template <ParserMode parserMode> TokenType lexString(LiteralParserToken&);
            TokenType lexNumber(LiteralParserToken&);
            LiteralParserToken m_currentToken;
            UString m_string;
            ParserMode m_mode;
            const UChar* m_ptr;
            const UChar* m_end;
        };
        
        class StackGuard;
        JSValue parse(ParserState);

        ExecState* m_exec;
        LiteralParser::Lexer m_lexer;
        ParserMode m_mode;
    };
}

#endif
