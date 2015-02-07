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
    Copyright (C) 2004, 2005, 2006 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005, 2006, 2007 Rob Buis <buis@kde.org>

    This file is part of the KDE project

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef SVGSVGElement_h
#define SVGSVGElement_h

#if ENABLE(SVG)

#include "IntSize.h"
#include "SVGExternalResourcesRequired.h"
#include "SVGFitToViewBox.h"
#include "SVGLangSpace.h"
#include "SVGStyledLocatableElement.h"
#include "SVGTests.h"
#include "SVGZoomAndPan.h"

namespace WebCore {
    class SVGAngle;
    class SVGLength;
    class SVGTransform;
    class SVGViewSpec;
    class SVGViewElement;
    class SMILTimeContainer;
    class SVGSVGElement : public SVGStyledLocatableElement,
                          public SVGTests,
                          public SVGLangSpace,
                          public SVGExternalResourcesRequired,
                          public SVGFitToViewBox,
                          public SVGZoomAndPan {
    public:
        SVGSVGElement(const QualifiedName&, Document*);
        virtual ~SVGSVGElement();

        virtual bool isSVG() const { return true; }
        
        virtual bool isValid() const { return SVGTests::isValid(); }

        // 'SVGSVGElement' functions
        const AtomicString& contentScriptType() const;
        void setContentScriptType(const AtomicString& type);

        const AtomicString& contentStyleType() const;
        void setContentStyleType(const AtomicString& type);

        FloatRect viewport() const;

        void setContainerSize(const IntSize& containerSize) { m_containerSize = containerSize; m_hasSetContainerSize = true; }
        IntSize containerSize() const { return m_containerSize; }
        bool hasSetContainerSize() const { return m_hasSetContainerSize; }
        int relativeWidthValue() const;
        int relativeHeightValue() const;

        float pixelUnitToMillimeterX() const;
        float pixelUnitToMillimeterY() const;
        float screenPixelToMillimeterX() const;
        float screenPixelToMillimeterY() const;

        bool useCurrentView() const;
        void setUseCurrentView(bool currentView);

        SVGViewSpec* currentView() const;

        float currentScale() const;
        void setCurrentScale(float scale);

        FloatPoint currentTranslate() const;
        void setCurrentTranslate(const FloatPoint&);
        
        SMILTimeContainer* timeContainer() const { return m_timeContainer.get(); }
        
        void pauseAnimations();
        void unpauseAnimations();
        bool animationsPaused() const;

        float getCurrentTime() const;
        void setCurrentTime(float seconds);

        unsigned long suspendRedraw(unsigned long max_wait_milliseconds);
        void unsuspendRedraw(unsigned long suspend_handle_id, ExceptionCode&);
        void unsuspendRedrawAll();
        void forceRedraw();

        NodeList* getIntersectionList(const FloatRect&, SVGElement* referenceElement);
        NodeList* getEnclosureList(const FloatRect&, SVGElement* referenceElement);
        bool checkIntersection(SVGElement*, const FloatRect&);
        bool checkEnclosure(SVGElement*, const FloatRect&);
        void deselectAll();

        static float createSVGNumber();
        static SVGLength createSVGLength();
        static PassRefPtr<SVGAngle> createSVGAngle();
        static FloatPoint createSVGPoint();
        static TransformationMatrix createSVGMatrix();
        static FloatRect createSVGRect();
        static SVGTransform createSVGTransform();
        static SVGTransform createSVGTransformFromMatrix(const TransformationMatrix&);

        virtual void parseMappedAttribute(MappedAttribute*);

        // 'virtual SVGLocatable' functions
        virtual TransformationMatrix getCTM() const;
        virtual TransformationMatrix getScreenCTM() const;

        virtual bool rendererIsNeeded(RenderStyle* style) { return StyledElement::rendererIsNeeded(style); }
        virtual RenderObject* createRenderer(RenderArena*, RenderStyle*);

        virtual void insertedIntoDocument();
        virtual void removedFromDocument();

        virtual void svgAttributeChanged(const QualifiedName&);

        virtual TransformationMatrix viewBoxToViewTransform(float viewWidth, float viewHeight) const;

        void inheritViewAttributes(SVGViewElement*);

    protected:
        virtual const SVGElement* contextElement() const { return this; }

        friend class RenderSVGRoot;
        friend class RenderSVGViewportContainer;

        virtual bool hasRelativeValues() const;
        
        bool isOutermostSVG() const;

    private:
        ANIMATED_PROPERTY_DECLARATIONS(SVGSVGElement, SVGNames::svgTagString, SVGNames::xAttrString, SVGLength, X, x)
        ANIMATED_PROPERTY_DECLARATIONS(SVGSVGElement, SVGNames::svgTagString, SVGNames::yAttrString, SVGLength, Y, y)
        ANIMATED_PROPERTY_DECLARATIONS(SVGSVGElement, SVGNames::svgTagString, SVGNames::widthAttrString, SVGLength, Width, width)
        ANIMATED_PROPERTY_DECLARATIONS(SVGSVGElement, SVGNames::svgTagString, SVGNames::heightAttrString, SVGLength, Height, height)

        virtual void documentWillBecomeInactive();
        virtual void documentDidBecomeActive();

        bool m_useCurrentView;
        RefPtr<SMILTimeContainer> m_timeContainer;
        FloatPoint m_translation;
        mutable OwnPtr<SVGViewSpec> m_viewSpec;
        IntSize m_containerSize;
        bool m_hasSetContainerSize;
    };

} // namespace WebCore

#endif // ENABLE(SVG)
#endif
