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
 * Copyright (C) 2008 Luca Bruno <lethalman88@gmail.com>
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

#ifndef WEBKIT_ERROR_H
#define WEBKIT_ERROR_H

#include <glib.h>

#include <webkit/webkitdefines.h>

G_BEGIN_DECLS

#define WEBKIT_NETWORK_ERROR        webkit_network_error_quark ()
#define WEBKIT_POLICY_ERROR         webkit_policy_error_quark  ()
#define WEBKIT_PLUGIN_ERROR         webkit_plugin_error_quark  ()

typedef enum {
    WEBKIT_NETWORK_ERROR_FAILED                                 = 399,
    WEBKIT_NETWORK_ERROR_TRANSPORT                              = 300,
    WEBKIT_NETWORK_ERROR_UNKNOWN_PROTOCOL                       = 301,
    WEBKIT_NETWORK_ERROR_CANCELLED                              = 302,
    WEBKIT_NETWORK_ERROR_FILE_DOES_NOT_EXIST                    = 303,
} WebKitNetworkError;

/* Sync'd with Mac's WebKit Errors */
typedef enum {
    WEBKIT_POLICY_ERROR_FAILED                                  = 199,
    WEBKIT_POLICY_ERROR_CANNOT_SHOW_MIME_TYPE                   = 100,
    WEBKIT_POLICY_ERROR_CANNOT_SHOW_URL                         = 101,
    WEBKIT_POLICY_ERROR_FRAME_LOAD_INTERRUPTED_BY_POLICY_CHANGE = 102,
    WEBKIT_POLICY_ERROR_CANNOT_USE_RESTRICTED_PORT              = 103,
} WebKitPolicyError;

typedef enum {
    WEBKIT_PLUGIN_ERROR_FAILED                                  = 299,
    WEBKIT_PLUGIN_ERROR_CANNOT_FIND_PLUGIN                      = 200,
    WEBKIT_PLUGIN_ERROR_CANNOT_LOAD_PLUGIN                      = 201,
    WEBKIT_PLUGIN_ERROR_JAVA_UNAVAILABLE                        = 202,
    WEBKIT_PLUGIN_ERROR_CONNECTION_CANCELLED                    = 203,
    WEBKIT_PLUGIN_ERROR_WILL_HANDLE_LOAD                        = 204,
} WebKitPluginError;


WEBKIT_API GQuark
webkit_network_error_quark (void);

WEBKIT_API GQuark
webkit_policy_error_quark  (void);

WEBKIT_API GQuark
webkit_plugin_error_quark  (void);

G_END_DECLS

#endif
