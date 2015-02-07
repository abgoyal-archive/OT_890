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
 *  Copyright (C) 2007 OpenedHand
 *  Copyright (C) 2007 Alp Toker <alp@atoker.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _HAVE_WEBKIT_VIDEO_SINK_H
#define _HAVE_WEBKIT_VIDEO_SINK_H

#include <cairo.h>
#include <glib-object.h>
#include <gst/base/gstbasesink.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_VIDEO_SINK webkit_video_sink_get_type()

#define WEBKIT_VIDEO_SINK(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST((obj), \
    WEBKIT_TYPE_VIDEO_SINK, WebKitVideoSink))

#define WEBKIT_VIDEO_SINK_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), \
    WEBKIT_TYPE_VIDEO_SINK, WebKitVideoSinkClass))

#define WEBKIT_IS_VIDEO_SINK(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE((obj), \
    WEBKIT_TYPE_VIDEO_SINK))

#define WEBKIT_IS_VIDEO_SINK_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), \
    WEBKIT_TYPE_VIDEO_SINK))

#define WEBKIT_VIDEO_SINK_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS((obj), \
    WEBKIT_TYPE_VIDEO_SINK, WebKitVideoSinkClass))

typedef struct _WebKitVideoSink        WebKitVideoSink;
typedef struct _WebKitVideoSinkClass   WebKitVideoSinkClass;
typedef struct _WebKitVideoSinkPrivate WebKitVideoSinkPrivate;

struct _WebKitVideoSink {
    /*< private >*/
    GstBaseSink parent;
    WebKitVideoSinkPrivate *priv;
};

struct _WebKitVideoSinkClass {
    /*< private >*/
    GstBaseSinkClass parent_class;

    /* Future padding */
    void (* _webkit_reserved1)(void);
    void (* _webkit_reserved2)(void);
    void (* _webkit_reserved3)(void);
    void (* _webkit_reserved4)(void);
    void (* _webkit_reserved5)(void);
    void (* _webkit_reserved6)(void);
};

GType       webkit_video_sink_get_type(void) G_GNUC_CONST;
GstElement *webkit_video_sink_new(cairo_surface_t *surface);

void webkit_video_sink_set_surface(WebKitVideoSink *sink, cairo_surface_t *surface);

G_END_DECLS

#endif
