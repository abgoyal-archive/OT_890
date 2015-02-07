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
 * ico.h - kimgio import filter for MS Windows .ico files
 *
 * Distributed under the terms of the LGPL
 * Copyright (c) 2000 Malte Starostik <malte@kde.org>
 *
 */

// You can use QImageIO::setParameters() to request a specific
// Icon out of an .ico file:
//
// Options consist of a name=value pair and are separated by a semicolon.
// Available options are:
//     size=<size>   select the icon that most closely matches <size> (pixels)
//                   default: 32
//     colors=<num>  select the icon that has <num> colors (or comes closest)
//                   default: 1 << display depth or 0 (RGB) if display depth > 8
//     index=<index> select the indexth icon from the file. If this option
//                   is present, the size and colors options will be ignored.
//                   default: none
// If both size and colors are given, size takes precedence.
//
// The old format is still supported:
//     the parameters consist of a single string in the form
//     "<size>[:<colors>]" which correspond to the options above
//
// If an icon was returned (i.e. the file is valid and the index option
// if present was not out of range), the icon's index within the .ico
// file is returned in the text tag "X-Index" of the image.
// If the icon is in fact a cursor, its hotspot coordinates are returned
// in the text tags "X-HotspotX" and "X-HotspotY".

#ifndef _ICOHANDLER_H_
#define _ICOHANDLER_H_

#include <QtGui/QImageIOPlugin>

class ICOHandler : public QImageIOHandler
{
public:
    ICOHandler();

    bool canRead() const;
    bool read(QImage *image);
    bool write(const QImage &image);

    QByteArray name() const;

    static bool canRead(QIODevice *device);
};

#endif
