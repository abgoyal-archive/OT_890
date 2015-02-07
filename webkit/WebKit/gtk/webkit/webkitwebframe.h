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
 * Copyright (C) 2007 Holger Hans Peter Freyther
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
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
 */

#ifndef WEBKIT_WEB_FRAME_H
#define WEBKIT_WEB_FRAME_H

#include <glib-object.h>
#include <gtk/gtk.h>

#include <JavaScriptCore/JSBase.h>

#include <webkit/webkitdefines.h>
#include <webkit/webkitnetworkrequest.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_WEB_FRAME            (webkit_web_frame_get_type())
#define WEBKIT_WEB_FRAME(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_WEB_FRAME, WebKitWebFrame))
#define WEBKIT_WEB_FRAME_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_WEB_FRAME, WebKitWebFrameClass))
#define WEBKIT_IS_WEB_FRAME(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_WEB_FRAME))
#define WEBKIT_IS_WEB_FRAME_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_WEB_FRAME))
#define WEBKIT_WEB_FRAME_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_WEB_FRAME, WebKitWebFrameClass))

typedef struct _WebKitWebFramePrivate WebKitWebFramePrivate;

struct _WebKitWebFrame {
    GObject parent_instance;

    /*< private >*/
    WebKitWebFramePrivate *priv;
};

struct _WebKitWebFrameClass {
    GObjectClass parent_class;

    /*< public >*/
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
    void (*_webkit_reserved4) (void);
    void (*_webkit_reserved5) (void);
    void (*_webkit_reserved6) (void);
};

/**
 * WebKitLoadStatus
 * @WEBKIT_LOAD_PROVISIONAL: No data has been received yet, empty
 * structures have been allocated to perform the load; the load may
 * still fail for transport issues such as not being able to resolve a
 * name, or connect to a port.
 * @WEBKIT_LOAD_COMMITTED: The first data chunk has arrived, meaning
 * that the necessary transport requirements are stabilished, and the
 * load is being performed.
 * @WEBKIT_LOAD_FIRST_VISUALLY_NON_EMPTY_LAYOUT: The first layout with
 * actual visible content happened; one or more layouts may have
 * happened before that caused nothing to be visible on the screen,
 * because the data available at the time was not significant enough.
 * @WEBKIT_LOAD_FINISHED: This state means either that everything that
 * was required to display the page has been loaded, or that an error
 * has happened.
 */
typedef enum {
    WEBKIT_LOAD_PROVISIONAL,
    WEBKIT_LOAD_COMMITTED,
    WEBKIT_LOAD_FINISHED,
    WEBKIT_LOAD_FIRST_VISUALLY_NON_EMPTY_LAYOUT
} WebKitLoadStatus;

WEBKIT_API GType
webkit_web_frame_get_type           (void);

#ifndef WEBKIT_DISABLE_DEPRECATED
WEBKIT_API WebKitWebFrame *
webkit_web_frame_new                (WebKitWebView        *web_view);
#endif

WEBKIT_API WebKitWebView *
webkit_web_frame_get_web_view       (WebKitWebFrame       *frame);

WEBKIT_API G_CONST_RETURN gchar *
webkit_web_frame_get_name           (WebKitWebFrame       *frame);

WEBKIT_API G_CONST_RETURN gchar *
webkit_web_frame_get_title          (WebKitWebFrame       *frame);

WEBKIT_API G_CONST_RETURN gchar *
webkit_web_frame_get_uri            (WebKitWebFrame       *frame);

WEBKIT_API WebKitWebFrame*
webkit_web_frame_get_parent         (WebKitWebFrame       *frame);

WEBKIT_API void
webkit_web_frame_load_uri           (WebKitWebFrame       *frame,
                                     const gchar          *uri);

WEBKIT_API void
webkit_web_frame_load_string        (WebKitWebFrame       *frame,
                                     const gchar          *content,
                                     const gchar          *mime_type,
                                     const gchar          *encoding,
                                     const gchar          *base_uri);

WEBKIT_API void
webkit_web_frame_load_alternate_string (WebKitWebFrame    *frame,
                                        const gchar       *content,
                                        const gchar       *base_url,
                                        const gchar       *unreachable_url);

WEBKIT_API void
webkit_web_frame_load_request       (WebKitWebFrame       *frame,
                                     WebKitNetworkRequest *request);

WEBKIT_API void
webkit_web_frame_stop_loading       (WebKitWebFrame       *frame);

WEBKIT_API void
webkit_web_frame_reload             (WebKitWebFrame       *frame);

WEBKIT_API WebKitWebFrame *
webkit_web_frame_find_frame         (WebKitWebFrame       *frame,
                                     const gchar          *name);

WEBKIT_API JSGlobalContextRef
webkit_web_frame_get_global_context (WebKitWebFrame       *frame);

WEBKIT_API GtkPrintOperationResult
webkit_web_frame_print_full         (WebKitWebFrame       *frame,
                                     GtkPrintOperation    *operation,
                                     GtkPrintOperationAction action,
                                     GError              **error);

WEBKIT_API void
webkit_web_frame_print              (WebKitWebFrame       *frame);

WEBKIT_API WebKitLoadStatus
webkit_web_frame_get_load_status    (WebKitWebFrame       *frame);

G_END_DECLS

#endif
