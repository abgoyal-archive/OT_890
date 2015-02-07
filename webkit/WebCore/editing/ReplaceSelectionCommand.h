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
 * Copyright (C) 2005, 2006, 2008 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef ReplaceSelectionCommand_h
#define ReplaceSelectionCommand_h

#include "CompositeEditCommand.h"

namespace WebCore {

class DocumentFragment;
class ReplacementFragment;

class ReplaceSelectionCommand : public CompositeEditCommand {
public:
    static PassRefPtr<ReplaceSelectionCommand> create(Document* document, PassRefPtr<DocumentFragment> fragment,
        bool selectReplacement = true, bool smartReplace = false, bool matchStyle = false, bool preventNesting = true, bool movingParagraph = false,
        EditAction action = EditActionPaste)
    {
        return adoptRef(new ReplaceSelectionCommand(document, fragment, selectReplacement, smartReplace, matchStyle, preventNesting, movingParagraph, action));
    }

private:
    ReplaceSelectionCommand(Document*, PassRefPtr<DocumentFragment>,
        bool selectReplacement, bool smartReplace, bool matchStyle, bool preventNesting, bool movingParagraph, EditAction);

    virtual void doApply();
    virtual EditAction editingAction() const;

    void completeHTMLReplacement(const Position& lastPositionToSelect);

    void insertNodeAfterAndUpdateNodesInserted(PassRefPtr<Node> insertChild, Node* refChild);
    void insertNodeAtAndUpdateNodesInserted(PassRefPtr<Node>, const Position&);
    void insertNodeBeforeAndUpdateNodesInserted(PassRefPtr<Node> insertChild, Node* refChild);

    void updateNodesInserted(Node*);
    bool shouldRemoveEndBR(Node*, const VisiblePosition&);
    
    bool shouldMergeStart(bool, bool, bool);
    bool shouldMergeEnd(bool selectEndWasEndOfParagraph);
    bool shouldMerge(const VisiblePosition&, const VisiblePosition&);
    
    void mergeEndIfNeeded();
    
    void removeUnrenderedTextNodesAtEnds();
    
    void negateStyleRulesThatAffectAppearance();
    void handleStyleSpans();
    void handlePasteAsQuotationNode();
    
    virtual void removeNodePreservingChildren(Node*);
    virtual void removeNodeAndPruneAncestors(Node*);
    
    VisiblePosition positionAtStartOfInsertedContent();
    VisiblePosition positionAtEndOfInsertedContent();
    
    bool performTrivialReplace(const ReplacementFragment&);

    RefPtr<Node> m_firstNodeInserted;
    RefPtr<Node> m_lastLeafInserted;
    RefPtr<CSSMutableStyleDeclaration> m_insertionStyle;
    bool m_selectReplacement;
    bool m_smartReplace;
    bool m_matchStyle;
    RefPtr<DocumentFragment> m_documentFragment;
    bool m_preventNesting;
    bool m_movingParagraph;
    EditAction m_editAction;
    bool m_shouldMergeEnd;
};

} // namespace WebCore

#endif // ReplaceSelectionCommand_h
