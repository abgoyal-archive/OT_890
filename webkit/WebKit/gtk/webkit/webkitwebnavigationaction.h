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
 * Copyright (C) 2008 Collabora Ltd.
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

#ifndef WEBKIT_WEB_NAVIGATION_ACTION_H
#define WEBKIT_WEB_NAVIGATION_ACTION_H

#include <glib-object.h>

#include <webkit/webkitdefines.h>

G_BEGIN_DECLS

/*
 * The order of this enum must be the same as NavigationType in
 * FrameLoaderTypes.h
 */
typedef enum {
    WEBKIT_WEB_NAVIGATION_REASON_LINK_CLICKED,
    WEBKIT_WEB_NAVIGATION_REASON_FORM_SUBMITTED,
    WEBKIT_WEB_NAVIGATION_REASON_BACK_FORWARD,
    WEBKIT_WEB_NAVIGATION_REASON_RELOAD,
    WEBKIT_WEB_NAVIGATION_REASON_FORM_RESUBMITTED,
    WEBKIT_WEB_NAVIGATION_REASON_OTHER,
} WebKitWebNavigationReason;

#define WEBKIT_TYPE_WEB_NAVIGATION_ACTION            (webkit_web_navigation_action_get_type())
#define WEBKIT_WEB_NAVIGATION_ACTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_WEB_NAVIGATION_ACTION, WebKitWebNavigationAction))
#define WEBKIT_WEB_NAVIGATION_ACTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), WEBKIT_TYPE_WEB_NAVIGATION_ACTION, WebKitWebNavigationActionClass))
#define WEBKIT_IS_WEB_NAVIGATION_ACTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_WEB_NAVIGATION_ACTION))
#define WEBKIT_IS_WEB_NAVIGATION_ACTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), WEBKIT_TYPE_WEB_NAVIGATION_ACTION))
#define WEBKIT_WEB_NAVIGATION_ACTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), WEBKIT_TYPE_WEB_NAVIGATION_ACTION, WebKitWebNavigationActionClass))

typedef struct _WebKitWebNavigationAction WebKitWebNavigationAction;
typedef struct _WebKitWebNavigationActionClass WebKitWebNavigationActionClass;
typedef struct _WebKitWebNavigationActionPrivate WebKitWebNavigationActionPrivate;

struct _WebKitWebNavigationAction {
    GObject parent_instance;

    /*< private >*/
    WebKitWebNavigationActionPrivate* priv;
};

struct _WebKitWebNavigationActionClass {
    GObjectClass parent_class;

    /* Padding for future expansion */
    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_web_navigation_action_get_type(void);

WEBKIT_API WebKitWebNavigationReason
webkit_web_navigation_action_get_reason(WebKitWebNavigationAction* navigationAction);

WEBKIT_API void
webkit_web_navigation_action_set_reason(WebKitWebNavigationAction* navigationAction, WebKitWebNavigationReason reason);

WEBKIT_API const gchar*
webkit_web_navigation_action_get_original_uri(WebKitWebNavigationAction* navigationAction);

WEBKIT_API void
webkit_web_navigation_action_set_original_uri(WebKitWebNavigationAction* navigationAction, const gchar* originalUri);

WEBKIT_API gint
webkit_web_navigation_action_get_button(WebKitWebNavigationAction* navigationAction);

WEBKIT_API gint
webkit_web_navigation_action_get_modifier_state(WebKitWebNavigationAction* navigationAction);

WEBKIT_API G_CONST_RETURN gchar *
webkit_web_navigation_action_get_target_frame(WebKitWebNavigationAction* navigationAction);

G_END_DECLS

#endif
