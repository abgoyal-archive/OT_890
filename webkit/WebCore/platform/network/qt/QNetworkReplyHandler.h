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
#ifndef QNETWORKREPLYHANDLER_H
#define QNETWORKREPLYHANDLER_H

#include <QObject>

#if QT_VERSION >= 0x040400

#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include "FormData.h"

QT_BEGIN_NAMESPACE
class QFile;
class QNetworkReply;
QT_END_NAMESPACE

namespace WebCore {

class ResourceHandle;

class QNetworkReplyHandler : public QObject
{
    Q_OBJECT
public:
    enum LoadMode {
        LoadNormal,
        LoadDeferred,
        LoadResuming
    };

    QNetworkReplyHandler(ResourceHandle *handle, LoadMode);
    void setLoadMode(LoadMode);

    QNetworkReply* reply() const { return m_reply; }

    void abort();

    QNetworkReply* release();

signals:
    void processQueuedItems();

private slots:
    void finish();
    void sendResponseIfNeeded();
    void forwardData();
    void sendQueuedItems();

private:
    void start();
    void resetState();

    QNetworkReply* m_reply;
    ResourceHandle* m_resourceHandle;
    bool m_redirected;
    bool m_responseSent;
    LoadMode m_loadMode;
    QNetworkAccessManager::Operation m_method;
    QNetworkRequest m_request;

    // defer state holding
    bool m_shouldStart;
    bool m_shouldFinish;
    bool m_shouldSendResponse;
    bool m_shouldForwardData;
};

// Self destructing QIODevice for FormData
//  For QNetworkAccessManager::put we will have to gurantee that the
//  QIODevice is valid as long finished() of the QNetworkReply has not
//  been emitted. With the presence of QNetworkReplyHandler::release I do
//  not want to gurantee this.
class FormDataIODevice : public QIODevice {
    Q_OBJECT
public:
    FormDataIODevice(FormData*);
    ~FormDataIODevice();

    void setParent(QNetworkReply*);
    bool isSequential() const;

protected:
    qint64 readData(char*, qint64);
    qint64 writeData(const char*, qint64);

private Q_SLOTS:
    void slotFinished();

private:
    void moveToNextElement();

private:
    Vector<FormDataElement> m_formElements;
    QFile* m_currentFile;
    qint64 m_currentDelta;
};

}

#endif

#endif // QNETWORKREPLYHANDLER_H
