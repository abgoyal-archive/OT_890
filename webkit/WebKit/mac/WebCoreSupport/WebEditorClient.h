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
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import <WebCore/EditorClient.h>
#import <wtf/RetainPtr.h>
#import <wtf/Forward.h>
#import <wtf/Vector.h>

@class WebView;
@class WebEditorUndoTarget;

class WebEditorClient : public WebCore::EditorClient {
public:
    WebEditorClient(WebView *);
    
    virtual void pageDestroyed();

    virtual bool isGrammarCheckingEnabled();
    virtual void toggleGrammarChecking();
    virtual bool isContinuousSpellCheckingEnabled();
    virtual void toggleContinuousSpellChecking();
    virtual int spellCheckerDocumentTag();

    virtual bool smartInsertDeleteEnabled();
    virtual bool isSelectTrailingWhitespaceEnabled();
    virtual bool isEditable();

    virtual bool shouldDeleteRange(WebCore::Range*);    
    virtual bool shouldShowDeleteInterface(WebCore::HTMLElement*);

    virtual bool shouldBeginEditing(WebCore::Range*);
    virtual bool shouldEndEditing(WebCore::Range*);
    virtual bool shouldInsertNode(WebCore::Node*, WebCore::Range*, WebCore::EditorInsertAction);
    virtual bool shouldInsertText(const WebCore::String&, WebCore::Range*, WebCore::EditorInsertAction);
    virtual bool shouldChangeSelectedRange(WebCore::Range* fromRange, WebCore::Range* toRange, WebCore::EAffinity, bool stillSelecting);

    virtual bool shouldApplyStyle(WebCore::CSSStyleDeclaration*, WebCore::Range*);
    
    virtual bool shouldMoveRangeAfterDelete(WebCore::Range* range, WebCore::Range* rangeToBeReplaced); 

    virtual void didBeginEditing();
    virtual void didEndEditing();
    virtual void didWriteSelectionToPasteboard();
    virtual void didSetSelectionTypesForPasteboard();

    virtual NSString* userVisibleString(NSURL*);
#ifdef BUILDING_ON_TIGER
    virtual NSArray* pasteboardTypesForSelection(WebCore::Frame*);
#endif
    
#if !defined(BUILDING_ON_TIGER) && !defined(BUILDING_ON_LEOPARD)
    virtual void uppercaseWord();
    virtual void lowercaseWord();
    virtual void capitalizeWord();
    virtual void showSubstitutionsPanel(bool show);
    virtual bool substitutionsPanelIsShowing();
    virtual void toggleSmartInsertDelete();
    virtual bool isAutomaticQuoteSubstitutionEnabled();
    virtual void toggleAutomaticQuoteSubstitution();
    virtual bool isAutomaticLinkDetectionEnabled();
    virtual void toggleAutomaticLinkDetection();
    virtual bool isAutomaticDashSubstitutionEnabled();
    virtual void toggleAutomaticDashSubstitution();
    virtual bool isAutomaticTextReplacementEnabled();
    virtual void toggleAutomaticTextReplacement();
    virtual bool isAutomaticSpellingCorrectionEnabled();
    virtual void toggleAutomaticSpellingCorrection();
#endif

    virtual void respondToChangedContents();
    virtual void respondToChangedSelection();

    virtual void registerCommandForUndo(PassRefPtr<WebCore::EditCommand>);
    virtual void registerCommandForRedo(PassRefPtr<WebCore::EditCommand>);
    virtual void clearUndoRedoOperations();
    
    virtual bool canUndo() const;
    virtual bool canRedo() const;
    
    virtual void undo();
    virtual void redo();
    
    virtual void handleKeyboardEvent(WebCore::KeyboardEvent*);
    virtual void handleInputMethodKeydown(WebCore::KeyboardEvent*);

    virtual void textFieldDidBeginEditing(WebCore::Element*);
    virtual void textFieldDidEndEditing(WebCore::Element*);
    virtual void textDidChangeInTextField(WebCore::Element*);
    virtual bool doTextFieldCommandFromEvent(WebCore::Element*, WebCore::KeyboardEvent*);
    virtual void textWillBeDeletedInTextField(WebCore::Element*);
    virtual void textDidChangeInTextArea(WebCore::Element*);
    
    virtual void ignoreWordInSpellDocument(const WebCore::String&);
    virtual void learnWord(const WebCore::String&);
    virtual void checkSpellingOfString(const UChar*, int length, int* misspellingLocation, int* misspellingLength);
    virtual WebCore::String getAutoCorrectSuggestionForMisspelledWord(const WebCore::String&);
    virtual void checkGrammarOfString(const UChar*, int length, WTF::Vector<WebCore::GrammarDetail>&, int* badGrammarLocation, int* badGrammarLength);
    virtual void checkTextOfParagraph(const UChar* text, int length, uint64_t checkingTypes, WTF::Vector<WebCore::TextCheckingResult>& results);
    virtual void updateSpellingUIWithGrammarString(const WebCore::String&, const WebCore::GrammarDetail&);
    virtual void updateSpellingUIWithMisspelledWord(const WebCore::String&);
    virtual void showSpellingUI(bool show);
    virtual bool spellingUIIsShowing();
    virtual void getGuessesForWord(const WebCore::String&, WTF::Vector<WebCore::String>& guesses);
    virtual void setInputMethodState(bool enabled);
private:
    void registerCommandForUndoOrRedo(PassRefPtr<WebCore::EditCommand>, bool isRedo);
    WebEditorClient();
    
    WebView *m_webView;
    RetainPtr<WebEditorUndoTarget> m_undoTarget;
    
    bool m_haveUndoRedoOperations;
};
