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
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2006 Allan Sandfeld Jensen (kde@carewolf.com) 
 *           (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 * Copyright (C) 2004, 2005, 2006, 2007, 2009 Apple Inc. All rights reserved.
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

#ifndef RenderImage_h
#define RenderImage_h

#include "RenderReplaced.h"

namespace WebCore {

class HTMLMapElement;

class RenderImage : public RenderReplaced {
public:
    RenderImage(Node*);
    virtual ~RenderImage();

    bool setImageSizeForAltText(CachedImage* newImage = 0);

    void updateAltText();

    void setCachedImage(CachedImage*);
    CachedImage* cachedImage() const { return m_cachedImage.get(); }

    HTMLMapElement* imageMap();

    void resetAnimation();

    virtual bool hasImage() const { return m_cachedImage; }

    void highQualityRepaintTimerFired(Timer<RenderImage>*);

protected:
    virtual Image* image(int /* width */ = 0, int /* height */ = 0) { return m_cachedImage ? m_cachedImage->image() : nullImage(); }
    virtual bool errorOccurred() const { return m_cachedImage && m_cachedImage->errorOccurred(); }

    virtual void imageChanged(WrappedImagePtr, const IntRect* = 0);

private:
    virtual const char* renderName() const { return "RenderImage"; }

    virtual bool isImage() const { return true; }
    virtual bool isRenderImage() const { return true; }
    
    virtual void paintReplaced(PaintInfo&, int tx, int ty);

    virtual int minimumReplacedHeight() const;

    virtual void notifyFinished(CachedResource*);
    virtual bool nodeAtPoint(const HitTestRequest&, HitTestResult&, int x, int y, int tx, int ty, HitTestAction);

    virtual int calcReplacedWidth(bool includeMaxWidth = true) const;
    virtual int calcReplacedHeight() const;

    virtual void calcPrefWidths();

    virtual bool usesImageContainerSize() const { return m_cachedImage ? m_cachedImage->usesImageContainerSize() : false; }
    virtual void setImageContainerSize(const IntSize& size) const { if (m_cachedImage) m_cachedImage->setImageContainerSize(size); }
    virtual bool imageHasRelativeWidth() const { return m_cachedImage ? m_cachedImage->imageHasRelativeWidth() : false; }
    virtual bool imageHasRelativeHeight() const { return m_cachedImage ? m_cachedImage->imageHasRelativeHeight() : false; }
    virtual IntSize imageSize(float multiplier) const { return m_cachedImage ? m_cachedImage->imageSize(multiplier) : IntSize(); }
    virtual WrappedImagePtr imagePtr() const { return m_cachedImage.get(); }

    virtual void intrinsicSizeChanged() { imageChanged(imagePtr()); }

    int calcAspectRatioWidth() const;
    int calcAspectRatioHeight() const;

    bool isWidthSpecified() const;
    bool isHeightSpecified() const;

protected:
    // The image we are rendering.
    CachedResourceHandle<CachedImage> m_cachedImage;

private:
    // Text to display as long as the image isn't available.
    String m_altText;

    static Image* nullImage();
    
    friend class RenderImageScaleObserver;
};

inline RenderImage* toRenderImage(RenderObject* object)
{ 
    ASSERT(!object || object->isRenderImage());
    return static_cast<RenderImage*>(object);
}

inline const RenderImage* toRenderImage(const RenderObject* object)
{ 
    ASSERT(!object || object->isRenderImage());
    return static_cast<const RenderImage*>(object);
}

// This will catch anyone doing an unnecessary cast.
void toRenderImage(const RenderImage*);

} // namespace WebCore

#endif // RenderImage_h