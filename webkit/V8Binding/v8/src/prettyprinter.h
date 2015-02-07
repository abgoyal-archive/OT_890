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

// Copyright 2006-2008 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef V8_PRETTYPRINTER_H_
#define V8_PRETTYPRINTER_H_

#include "ast.h"

namespace v8 {
namespace internal {

#ifdef DEBUG

class PrettyPrinter: public AstVisitor {
 public:
  PrettyPrinter();
  virtual ~PrettyPrinter();

  // The following routines print a node into a string.
  // The result string is alive as long as the PrettyPrinter is alive.
  const char* Print(AstNode* node);
  const char* PrintExpression(FunctionLiteral* program);
  const char* PrintProgram(FunctionLiteral* program);

  // Print a node to stdout.
  static void PrintOut(AstNode* node);

  // Individual nodes
#define DEF_VISIT(type)                         \
  virtual void Visit##type(type* node);
  AST_NODE_LIST(DEF_VISIT)
#undef DEF_VISIT

 private:
  char* output_;  // output string buffer
  int size_;  // output_ size
  int pos_;  // current printing position

 protected:
  void Init();
  void Print(const char* format, ...);
  const char* Output() const { return output_; }

  virtual void PrintStatements(ZoneList<Statement*>* statements);
  void PrintLabels(ZoneStringList* labels);
  virtual void PrintArguments(ZoneList<Expression*>* arguments);
  void PrintLiteral(Handle<Object> value, bool quote);
  void PrintParameters(Scope* scope);
  void PrintDeclarations(ZoneList<Declaration*>* declarations);
  void PrintFunctionLiteral(FunctionLiteral* function);
  void PrintCaseClause(CaseClause* clause);
};


// Prints the AST structure
class AstPrinter: public PrettyPrinter {
 public:
  AstPrinter();
  virtual ~AstPrinter();

  const char* PrintProgram(FunctionLiteral* program);

  // Individual nodes
#define DEF_VISIT(type)                         \
  virtual void Visit##type(type* node);
  AST_NODE_LIST(DEF_VISIT)
#undef DEF_VISIT
 private:
  friend class IndentedScope;
  void PrintIndented(const char* txt);
  void PrintIndentedVisit(const char* s, AstNode* node);

  void PrintStatements(ZoneList<Statement*>* statements);
  void PrintDeclarations(ZoneList<Declaration*>* declarations);
  void PrintParameters(Scope* scope);
  void PrintArguments(ZoneList<Expression*>* arguments);
  void PrintCaseClause(CaseClause* clause);
  void PrintLiteralIndented(const char* info, Handle<Object> value, bool quote);
  void PrintLiteralWithModeIndented(const char* info,
                                    Variable* var,
                                    Handle<Object> value,
                                    SmiAnalysis* type);
  void PrintLabelsIndented(const char* info, ZoneStringList* labels);

  void inc_indent() { indent_++; }
  void dec_indent() { indent_--; }

  static int indent_;
};

#endif  // DEBUG

} }  // namespace v8::internal

#endif  // V8_PRETTYPRINTER_H_
