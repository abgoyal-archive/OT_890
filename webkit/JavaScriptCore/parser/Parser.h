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
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *  Copyright (C) 2003, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef Parser_h
#define Parser_h

#include "Debugger.h"
#include "Nodes.h"
#include "SourceProvider.h"
#include <wtf/Forward.h>
#include <wtf/Noncopyable.h>
#include <wtf/OwnPtr.h>
#include <wtf/RefPtr.h>

namespace JSC {

    class FunctionBodyNode;
    class ProgramNode;
    class UString;

    template <typename T> struct ParserArenaData : ParserArenaDeletable { T data; };

    class Parser : public Noncopyable {
    public:
        template <class ParsedNode> PassRefPtr<ParsedNode> parse(ExecState*, Debugger*, const SourceCode&, int* errLine = 0, UString* errMsg = 0);
        template <class ParsedNode> PassRefPtr<ParsedNode> reparse(JSGlobalData*, ParsedNode*);
        void reparseInPlace(JSGlobalData*, FunctionBodyNode*);

        void didFinishParsing(SourceElements*, ParserArenaData<DeclarationStacks::VarStack>*, 
                              ParserArenaData<DeclarationStacks::FunctionStack>*, CodeFeatures features, int lastLine, int numConstants);

        ParserArena& arena() { return m_arena; }

    private:
        void parse(JSGlobalData*, int* errLine, UString* errMsg);

        ParserArena m_arena;
        const SourceCode* m_source;
        SourceElements* m_sourceElements;
        ParserArenaData<DeclarationStacks::VarStack>* m_varDeclarations;
        ParserArenaData<DeclarationStacks::FunctionStack>* m_funcDeclarations;
        CodeFeatures m_features;
        int m_lastLine;
        int m_numConstants;
    };

    template <class ParsedNode> PassRefPtr<ParsedNode> Parser::parse(ExecState* exec, Debugger* debugger, const SourceCode& source, int* errLine, UString* errMsg)
    {
        m_source = &source;
        parse(&exec->globalData(), errLine, errMsg);
        RefPtr<ParsedNode> result;
        if (m_sourceElements) {
            result = ParsedNode::create(&exec->globalData(),
                                         m_sourceElements,
                                         m_varDeclarations ? &m_varDeclarations->data : 0, 
                                         m_funcDeclarations ? &m_funcDeclarations->data : 0,
                                         *m_source,
                                         m_features,
                                         m_numConstants);
            result->setLoc(m_source->firstLine(), m_lastLine);
        }

        m_arena.reset();

        m_source = 0;
        m_varDeclarations = 0;
        m_funcDeclarations = 0;

        if (debugger)
            debugger->sourceParsed(exec, source, *errLine, *errMsg);
        return result.release();
    }

    template <class ParsedNode> PassRefPtr<ParsedNode> Parser::reparse(JSGlobalData* globalData, ParsedNode* oldParsedNode)
    {
        m_source = &oldParsedNode->source();
        parse(globalData, 0, 0);
        RefPtr<ParsedNode> result;
        if (m_sourceElements) {
            result = ParsedNode::create(globalData,
                                        m_sourceElements,
                                        m_varDeclarations ? &m_varDeclarations->data : 0, 
                                        m_funcDeclarations ? &m_funcDeclarations->data : 0,
                                        *m_source,
                                        oldParsedNode->features(),
                                        m_numConstants);
            result->setLoc(m_source->firstLine(), m_lastLine);
        }

        m_arena.reset();

        m_source = 0;
        m_varDeclarations = 0;
        m_funcDeclarations = 0;

        return result.release();
    }

} // namespace JSC

#endif // Parser_h
