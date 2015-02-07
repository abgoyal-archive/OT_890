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
 * Copyright 2005 Frerich Raabe <raabe@kde.org>
 * Copyright (C) 2006 Apple Computer, Inc.
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

#ifndef XPathPredicate_h
#define XPathPredicate_h

#if ENABLE(XPATH)

#include "XPathExpressionNode.h"
#include "XPathValue.h"

namespace WebCore {

    namespace XPath {
        
        class Number : public Expression {
        public:
            Number(double);
        private:
            virtual Value evaluate() const;
            virtual Value::Type resultType() const { return Value::NumberValue; }

            Value m_value;
        };

        class StringExpression : public Expression {
        public:
            StringExpression(const String&);
        private:
            virtual Value evaluate() const;
            virtual Value::Type resultType() const { return Value::StringValue; }

            Value m_value;
        };

        class Negative : public Expression {
        private:
            virtual Value evaluate() const;
            virtual Value::Type resultType() const { return Value::NumberValue; }
        };

        class NumericOp : public Expression {
        public:
            enum Opcode {
                OP_Add, OP_Sub, OP_Mul, OP_Div, OP_Mod
            };
            NumericOp(Opcode, Expression* lhs, Expression* rhs);
        private:
            virtual Value evaluate() const;
            virtual Value::Type resultType() const { return Value::NumberValue; }

            Opcode m_opcode;
        };

        class EqTestOp : public Expression {
        public:
            enum Opcode { OP_EQ, OP_NE, OP_GT, OP_LT, OP_GE, OP_LE };
            EqTestOp(Opcode, Expression* lhs, Expression* rhs);
            virtual Value evaluate() const;
        private:
            virtual Value::Type resultType() const { return Value::BooleanValue; }
            bool compare(const Value&, const Value&) const;

            Opcode m_opcode;
        };

        class LogicalOp : public Expression {
        public:
            enum Opcode { OP_And, OP_Or };
            LogicalOp(Opcode, Expression* lhs, Expression* rhs);
        private:
            virtual Value::Type resultType() const { return Value::BooleanValue; }
            bool shortCircuitOn() const;
            virtual Value evaluate() const;

            Opcode m_opcode;
        };

        class Union : public Expression {
        private:
            virtual Value evaluate() const;
            virtual Value::Type resultType() const { return Value::NodeSetValue; }
        };

        class Predicate : public Noncopyable {
        public:
            Predicate(Expression*);
            ~Predicate();
            bool evaluate() const;

            bool isContextPositionSensitive() const { return m_expr->isContextPositionSensitive() || m_expr->resultType() == Value::NumberValue; }
            bool isContextSizeSensitive() const { return m_expr->isContextSizeSensitive(); }

        private:
            Expression* m_expr;
        };

    }

}

#endif // ENABLE(XPATH)

#endif // XPathPredicate_h
