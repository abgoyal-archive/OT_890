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
 * Copyright (C) 2005 Frerich Raabe <raabe@kde.org>
 * Copyright (C) 2006, 2009 Apple Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef XPathStep_h
#define XPathStep_h

#if ENABLE(XPATH)

#include "Node.h"
#include "XPathExpressionNode.h"
#include "XPathNodeSet.h"

namespace WebCore {

    namespace XPath {

        class Predicate;
        
        class Step : public ParseNode, public Noncopyable {
        public:
            enum Axis {
                AncestorAxis, AncestorOrSelfAxis, AttributeAxis,
                ChildAxis, DescendantAxis, DescendantOrSelfAxis,
                FollowingAxis, FollowingSiblingAxis, NamespaceAxis,
                ParentAxis, PrecedingAxis, PrecedingSiblingAxis,
                SelfAxis
            };
            
            class NodeTest {
            public:
                enum Kind {
                    TextNodeTest, CommentNodeTest, ProcessingInstructionNodeTest, AnyNodeTest, NameTest
                };
                
                NodeTest(Kind kind) : m_kind(kind) {}
                NodeTest(Kind kind, const String& data) : m_kind(kind), m_data(data) {}
                NodeTest(Kind kind, const String& data, const String& namespaceURI) : m_kind(kind), m_data(data), m_namespaceURI(namespaceURI) {}
                
                Kind kind() const { return m_kind; }
                const AtomicString& data() const { return m_data; }
                const AtomicString& namespaceURI() const { return m_namespaceURI; }
                Vector<Predicate*>& mergedPredicates() { return m_mergedPredicates; }
                const Vector<Predicate*>& mergedPredicates() const { return m_mergedPredicates; }
                
            private:
                Kind m_kind;
                AtomicString m_data;
                AtomicString m_namespaceURI;

                // When possible, we merge some or all predicates with node test for better performance.
                Vector<Predicate*> m_mergedPredicates;
            };

            Step(Axis, const NodeTest& nodeTest, const Vector<Predicate*>& predicates = Vector<Predicate*>());
            ~Step();

            void optimize();

            void evaluate(Node* context, NodeSet&) const;

            Axis axis() const { return m_axis; }
            const NodeTest& nodeTest() const { return m_nodeTest; }

        private:
            friend void optimizeStepPair(Step*, Step*, bool&);
            bool predicatesAreContextListInsensitive() const;

            void parseNodeTest(const String&);
            void nodesInAxis(Node* context, NodeSet&) const;
            String namespaceFromNodetest(const String& nodeTest) const;

            Axis m_axis;
            NodeTest m_nodeTest;
            Vector<Predicate*> m_predicates;
        };

        void optimizeStepPair(Step*, Step*, bool& dropSecondStep);
    }

}

#endif // ENABLE(XPATH)

#endif // XPathStep_h
