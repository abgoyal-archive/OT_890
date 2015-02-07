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
    Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)

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

#ifndef QWEBHISTORY_H
#define QWEBHISTORY_H

#include <QtCore/qurl.h>
#include <QtCore/qstring.h>
#include <QtGui/qicon.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qshareddata.h>

#include "qwebkitglobal.h"

class QWebPage;

namespace WebCore {
    class FrameLoaderClientQt;
}

class QWebHistoryItemPrivate;

class QWEBKIT_EXPORT QWebHistoryItem {
public:
    QWebHistoryItem(const QWebHistoryItem &other);
    QWebHistoryItem &operator=(const QWebHistoryItem &other);
    ~QWebHistoryItem();

    //bool restoreState(QByteArray& buffer);
    //QByteArray saveState(QWebHistory::HistoryStateVersion version = DefaultHistoryVersion) const;

    QUrl originalUrl() const;
    QUrl url() const;

    QString title() const;
    QDateTime lastVisited() const;

    QIcon icon() const;

    QVariant userData() const;
    void setUserData(const QVariant& userData);

    bool isValid() const;

private:
    QWebHistoryItem(QWebHistoryItemPrivate *priv);
    friend class QWebHistory;
    friend class QWebPage;
    friend class WebCore::FrameLoaderClientQt;
    friend class QWebHistoryItemPrivate;
    //friend QDataStream & operator<<(QDataStream& out,const QWebHistoryItem& hist);
    //friend QDataStream & operator>>(QDataStream& in,QWebHistoryItem& hist);
    QExplicitlySharedDataPointer<QWebHistoryItemPrivate> d;
};

//QWEBKIT_EXPORT QDataStream & operator<<(QDataStream& out,const QWebHistoryItem& hist);
//QWEBKIT_EXPORT QDataStream & operator>>(QDataStream& in,QWebHistoryItem& hist);


class QWebHistoryPrivate;
class QWEBKIT_EXPORT QWebHistory {
public:
    enum HistoryStateVersion {
        HistoryVersion_1,
        /*, HistoryVersion_2, */
        DefaultHistoryVersion = HistoryVersion_1
    };

    bool restoreState(const QByteArray& buffer);
    QByteArray saveState(HistoryStateVersion version = DefaultHistoryVersion) const;

    void clear();

    QList<QWebHistoryItem> items() const;
    QList<QWebHistoryItem> backItems(int maxItems) const;
    QList<QWebHistoryItem> forwardItems(int maxItems) const;

    bool canGoBack() const;
    bool canGoForward() const;

    void back();
    void forward();
    void goToItem(const QWebHistoryItem &item);

    QWebHistoryItem backItem() const;
    QWebHistoryItem currentItem() const;
    QWebHistoryItem forwardItem() const;
    QWebHistoryItem itemAt(int i) const;

    int currentItemIndex() const;

    int count() const;

    int maximumItemCount() const;
    void setMaximumItemCount(int count);

private:
    QWebHistory();
    ~QWebHistory();

    friend class QWebPage;
    friend class QWebPagePrivate;
    friend QWEBKIT_EXPORT QDataStream& operator>>(QDataStream&, QWebHistory&);
    friend QWEBKIT_EXPORT QDataStream& operator<<(QDataStream&, const QWebHistory&);

    Q_DISABLE_COPY(QWebHistory)

    QWebHistoryPrivate *d;
};

QWEBKIT_EXPORT QDataStream& operator<<(QDataStream& stream, const QWebHistory& history);
QWEBKIT_EXPORT QDataStream& operator>>(QDataStream& stream, QWebHistory& history);

#endif
