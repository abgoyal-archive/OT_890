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

#ifndef QWEBNETWORKINTERFACE_H
#define QWEBNETWORKINTERFACE_H

#include <QtCore/qobject.h>
#include <QtCore/qurl.h>
#include <QtNetwork/qhttp.h>
#include <QtCore/qbytearray.h>

#include "qwebkitglobal.h"

#if QT_VERSION < 0x040400

class QAuthenticator;
class QNetworkProxy;
class QSslError;
class QWebFrame;
class QWebNetworkJobPrivate;
class QWebNetworkInterface;
class QWebObjectPluginConnector;

namespace WebCore {
    class WebCoreHttp;
    class ResourceRequest;
    class FrameLoaderClientQt;
}

struct QWebNetworkRequestPrivate;
class QWEBKIT_EXPORT QWebNetworkRequest {
public:
    enum Method {
        Get,
        Post
        //Head
    };

    QWebNetworkRequest();
    explicit QWebNetworkRequest(const QUrl &url, Method method = Get, const QByteArray &postData = QByteArray());
    QWebNetworkRequest(const QWebNetworkRequest &other);

    QWebNetworkRequest &operator=(const QWebNetworkRequest &other);
    ~QWebNetworkRequest();

    QUrl url() const;
    void setUrl(const QUrl &url);

    QHttpRequestHeader httpHeader() const;
    void setHttpHeader(const QHttpRequestHeader &header) const;

    QString httpHeaderField(const QString &key) const;
    void setHttpHeaderField(const QString &key, const QString &value);

    QByteArray postData() const;
    void setPostData(const QByteArray &data);

private:
    explicit QWebNetworkRequest(const QWebNetworkRequestPrivate &priv);
    explicit QWebNetworkRequest(const WebCore::ResourceRequest &request);
    friend class QWebNetworkJob;
    friend class WebCore::FrameLoaderClientQt;

    QWebNetworkRequestPrivate *d;
    friend class QWebObjectPluginConnector;
};

class QWEBKIT_EXPORT QWebNetworkJob {
public:

    QUrl url() const;
    QByteArray postData() const;
    QHttpRequestHeader httpHeader() const;
    QWebNetworkRequest request() const;
    QString errorString() const;

    QHttpResponseHeader response() const;
    void setResponse(const QHttpResponseHeader &response);
    void setErrorString(const QString&);

    bool cancelled() const;

    void ref();
    bool deref();

    QWebNetworkInterface *networkInterface() const;

    QWebFrame *frame() const;

protected:
    enum JobStatus {
        JobCreated,
        JobRecreated,
        JobStarted,
        JobReceivingData,
        JobFinished
    };

    JobStatus status() const;
    void setStatus(const JobStatus&);

private:
    QWebNetworkJob();
    ~QWebNetworkJob();

    friend class QWebNetworkManager;
    friend class QWebObjectPluginConnector;
    friend class QWebNetworkJobPrivate;

    QWebNetworkJobPrivate *d;
};

class QWebNetworkInterfacePrivate;

class QWEBKIT_EXPORT QWebNetworkInterface : public QObject {
    Q_OBJECT
public:
    QWebNetworkInterface(QObject *parent = 0);
    ~QWebNetworkInterface();

    static void setDefaultInterface(QWebNetworkInterface *defaultInterface);
    static QWebNetworkInterface *defaultInterface();

    virtual void addJob(QWebNetworkJob *job);
    virtual void cancelJob(QWebNetworkJob *job);

protected:
    void started(QWebNetworkJob*);
    void data(QWebNetworkJob*, const QByteArray &data);
    void finished(QWebNetworkJob*, int errorCode);

signals:
    void sslErrors(QWebFrame *frame, const QUrl& url, const QList<QSslError>& errors, bool *continueAnyway);
    void authenticate(QWebFrame *frame, const QUrl& url, const QString& hostname, quint16 port, QAuthenticator *auth);
    void authenticateProxy(QWebFrame *frame, const QUrl& url, const QNetworkProxy& proxy, QAuthenticator *auth);

private:
    friend class QWebNetworkInterfacePrivate;
    friend class WebCore::WebCoreHttp;
    QWebNetworkInterfacePrivate *d;
};

#endif

#endif
