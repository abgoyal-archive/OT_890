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
 * (C) 1999 Lars Knoll (knoll@kde.org)
 * (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
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

#ifndef RenderText_h
#define RenderText_h

#include "RenderObject.h"

namespace WebCore {

class InlineTextBox;
class StringImpl;

class RenderText : public RenderObject {
public:
    RenderText(Node*, PassRefPtr<StringImpl>);
#ifndef NDEBUG
    virtual ~RenderText();
#endif

    virtual const char* renderName() const;

    virtual bool isTextFragment() const;
    virtual bool isWordBreak() const;

    virtual PassRefPtr<StringImpl> originalText() const;

    void extractTextBox(InlineTextBox*);
    void attachTextBox(InlineTextBox*);
    void removeTextBox(InlineTextBox*);

    virtual void destroy();

    StringImpl* text() const { return m_text.get(); }

    InlineTextBox* createInlineTextBox();
    void dirtyLineBoxes(bool fullLayout);

    virtual void absoluteRects(Vector<IntRect>&, int tx, int ty);
    void absoluteRectsForRange(Vector<IntRect>&, unsigned startOffset = 0, unsigned endOffset = UINT_MAX, bool useSelectionHeight = false);

    virtual void absoluteQuads(Vector<FloatQuad>&);
    void absoluteQuadsForRange(Vector<FloatQuad>&, unsigned startOffset = 0, unsigned endOffset = UINT_MAX, bool useSelectionHeight = false);

    virtual VisiblePosition positionForPoint(const IntPoint&);

    const UChar* characters() const { return m_text->characters(); }
    unsigned textLength() const { return m_text->length(); } // non virtual implementation of length()
    void positionLineBox(InlineBox*);

    virtual unsigned width(unsigned from, unsigned len, const Font&, int xPos, HashSet<const SimpleFontData*>* fallbackFonts = 0) const;
    virtual unsigned width(unsigned from, unsigned len, int xPos, bool firstLine = false, HashSet<const SimpleFontData*>* fallbackFonts = 0) const;

    virtual int lineHeight(bool firstLine, bool isRootLineBox = false) const;

    virtual int minPrefWidth() const;
    virtual int maxPrefWidth() const;

    void trimmedPrefWidths(int leadWidth,
                           int& beginMinW, bool& beginWS,
                           int& endMinW, bool& endWS,
                           bool& hasBreakableChar, bool& hasBreak,
                           int& beginMaxW, int& endMaxW,
                           int& minW, int& maxW, bool& stripFrontSpaces);

    IntRect linesBoundingBox() const;

    IntPoint firstRunOrigin() const;
    int firstRunX() const;
    int firstRunY() const;

    void setText(PassRefPtr<StringImpl>, bool force = false);
    void setTextWithOffset(PassRefPtr<StringImpl>, unsigned offset, unsigned len, bool force = false);

    virtual bool canBeSelectionLeaf() const { return true; }
    virtual void setSelectionState(SelectionState s);
    virtual IntRect selectionRectForRepaint(RenderBoxModelObject* repaintContainer, bool clipToVisibleContent = true);
    virtual IntRect localCaretRect(InlineBox*, int caretOffset, int* extraWidthToEndOfLine = 0);

    virtual int marginLeft() const { return style()->marginLeft().calcMinValue(0); }
    virtual int marginRight() const { return style()->marginRight().calcMinValue(0); }

    virtual IntRect clippedOverflowRectForRepaint(RenderBoxModelObject* repaintContainer);

    InlineTextBox* firstTextBox() const { return m_firstTextBox; }
    InlineTextBox* lastTextBox() const { return m_lastTextBox; }

    virtual int caretMinOffset() const;
    virtual int caretMaxOffset() const;
    virtual unsigned caretMaxRenderedOffset() const;

    virtual int previousOffset(int current) const;
    virtual int previousOffsetForBackwardDeletion(int current) const;
    virtual int nextOffset(int current) const;

    bool containsReversedText() const { return m_containsReversedText; }

    InlineTextBox* findNextInlineTextBox(int offset, int& pos) const;

    bool allowTabs() const { return !style()->collapseWhiteSpace(); }

    void checkConsistency() const;

    virtual void calcPrefWidths(int leadWidth);

protected:
    virtual void styleWillChange(StyleDifference, const RenderStyle*) { }
    virtual void styleDidChange(StyleDifference, const RenderStyle* oldStyle);

    virtual void setTextInternal(PassRefPtr<StringImpl>);
    virtual UChar previousCharacter();
    
    virtual InlineTextBox* createTextBox(); // Subclassed by SVG.

private:
    void calcPrefWidths(int leadWidth, HashSet<const SimpleFontData*>& fallbackFonts);

    // Make length() private so that callers that have a RenderText*
    // will use the more efficient textLength() instead, while
    // callers with a RenderObject* can continue to use length().
    virtual unsigned length() const { return textLength(); }

    virtual void paint(PaintInfo&, int, int) { ASSERT_NOT_REACHED(); }
    virtual void layout() { ASSERT_NOT_REACHED(); }
    virtual bool nodeAtPoint(const HitTestRequest&, HitTestResult&, int, int, int, int, HitTestAction) { ASSERT_NOT_REACHED(); return false; }

    void deleteTextBoxes();
    bool containsOnlyWhitespace(unsigned from, unsigned len) const;
    int widthFromCache(const Font&, int start, int len, int xPos, HashSet<const SimpleFontData*>* fallbackFonts) const;
    bool isAllASCII() const { return m_isAllASCII; }

    int m_minWidth; // here to minimize padding in 64-bit.

    RefPtr<StringImpl> m_text;

    InlineTextBox* m_firstTextBox;
    InlineTextBox* m_lastTextBox;

    int m_maxWidth;
    int m_beginMinWidth;
    int m_endMinWidth;

    bool m_hasBreakableChar : 1; // Whether or not we can be broken into multiple lines.
    bool m_hasBreak : 1; // Whether or not we have a hard break (e.g., <pre> with '\n').
    bool m_hasTab : 1; // Whether or not we have a variable width tab character (e.g., <pre> with '\t').
    bool m_hasBeginWS : 1; // Whether or not we begin with WS (only true if we aren't pre)
    bool m_hasEndWS : 1; // Whether or not we end with WS (only true if we aren't pre)
    bool m_linesDirty : 1; // This bit indicates that the text run has already dirtied specific
                           // line boxes, and this hint will enable layoutInlineChildren to avoid
                           // just dirtying everything when character data is modified (e.g., appended/inserted
                           // or removed).
    bool m_containsReversedText : 1;
    bool m_isAllASCII : 1;
    mutable bool m_knownNotToUseFallbackFonts : 1;
};

inline RenderText* toRenderText(RenderObject* object)
{ 
    ASSERT(!object || object->isText());
    return static_cast<RenderText*>(object);
}

inline const RenderText* toRenderText(const RenderObject* object)
{ 
    ASSERT(!object || object->isText());
    return static_cast<const RenderText*>(object);
}

// This will catch anyone doing an unnecessary cast.
void toRenderText(const RenderText*);

#ifdef NDEBUG
inline void RenderText::checkConsistency() const
{
}
#endif

} // namespace WebCore

#endif // RenderText_h
