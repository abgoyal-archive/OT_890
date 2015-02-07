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
 * Copyright (C) 2008 Jan Michael C. Alonzo
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

#ifndef WEBKIT_WEB_HISTORY_ITEM_H
#define WEBKIT_WEB_HISTORY_ITEM_H

#include <glib.h>
#include <glib-object.h>

#include <webkit/webkitdefines.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_WEB_HISTORY_ITEM            (webkit_web_history_item_get_type())
#define WEBKIT_WEB_HISTORY_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_WEB_HISTORY_ITEM, WebKitWebHistoryItem))
#define WEBKIT_WEB_HISTORY_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_WEB_HISTORY_ITEM, WebKitWebHistoryItemClass))
#define WEBKIT_IS_WEB_HISTORY_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_WEB_HISTORY_ITEM))
#define WEBKIT_IS_WEB_HISTORY_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_WEB_HISTORY_ITEM))
#define WEBKIT_WEB_HISTORY_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_WEB_HISTORY_ITEM, WebKitWebHistoryItemClass))

typedef struct _WebKitWebHistoryItemPrivate WebKitWebHistoryItemPrivate;

struct _WebKitWebHistoryItem {
    GObject parent_instance;

    /*< private >*/
    WebKitWebHistoryItemPrivate *priv;
};

struct _WebKitWebHistoryItemClass {
    GObjectClass parent_class;

    /* Padding for future expansion */
    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_web_history_item_get_type (void);

WEBKIT_API WebKitWebHistoryItem *
webkit_web_history_item_new (void);

WEBKIT_API WebKitWebHistoryItem *
webkit_web_history_item_new_with_data         (const gchar          *uri,
                                               const gchar          *title);

WEBKIT_API G_CONST_RETURN gchar *
webkit_web_history_item_get_title             (WebKitWebHistoryItem *web_history_item);

WEBKIT_API G_CONST_RETURN gchar *
webkit_web_history_item_get_alternate_title   (WebKitWebHistoryItem *web_history_item);

WEBKIT_API void
webkit_web_history_item_set_alternate_title   (WebKitWebHistoryItem *web_history_item,
                                               const gchar          *title);

WEBKIT_API G_CONST_RETURN gchar *
webkit_web_history_item_get_uri               (WebKitWebHistoryItem *web_history_item);

WEBKIT_API G_CONST_RETURN gchar *
webkit_web_history_item_get_original_uri      (WebKitWebHistoryItem *web_history_item);

WEBKIT_API gdouble
webkit_web_history_item_get_last_visited_time (WebKitWebHistoryItem *web_history_item);

G_END_DECLS

#endif /* WEBKIT_WEB_HISTORY_ITEM_H */
