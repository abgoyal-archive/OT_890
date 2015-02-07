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
    Copyright (C) 2008,2009 Nokia Corporation and/or its subsidiary(-ies)
    Copyright (C) 2007 Staikos Computing Services Inc.

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

#ifndef QWEBFRAME_H
#define QWEBFRAME_H

#include <QtCore/qobject.h>
#include <QtCore/qurl.h>
#include <QtCore/qvariant.h>
#include <QtGui/qicon.h>
#include <QtScript/qscriptengine.h>
#if QT_VERSION >= 0x040400
#include <QtNetwork/qnetworkaccessmanager.h>
#endif
#include "qwebkitglobal.h"

QT_BEGIN_NAMESPACE
class QRect;
class QPoint;
class QPainter;
class QPixmap;
class QMouseEvent;
class QWheelEvent;
class QNetworkRequest;
class QRegion;
class QPrinter;
QT_END_NAMESPACE

class QWebNetworkRequest;
class QWebFramePrivate;
class QWebPage;
class QWebHitTestResult;
class QWebHistoryItem;
class QWebSecurityOrigin;
class QWebElement;

namespace WebCore {
    class WidgetPrivate;
    class FrameLoaderClientQt;
    class ChromeClientQt;
}
class QWebFrameData;
class QWebHitTestResultPrivate;
class QWebFrame;

class QWEBKIT_EXPORT QWebHitTestResult {
public:
    QWebHitTestResult();
    QWebHitTestResult(const QWebHitTestResult &other);
    QWebHitTestResult &operator=(const QWebHitTestResult &other);
    ~QWebHitTestResult();

    bool isNull() const;

    QPoint pos() const;
    QRect boundingRect() const;
    QWebElement enclosingBlockElement() const;
    QString title() const;

    QString linkText() const;
    QUrl linkUrl() const;
    QUrl linkTitle() const;
    QWebFrame *linkTargetFrame() const;
    QWebElement linkElement() const;

    QString alternateText() const; // for img, area, input and applet

    QUrl imageUrl() const;
    QPixmap pixmap() const;

    bool isContentEditable() const;
    bool isContentSelected() const;

    QWebElement element() const;

    QWebFrame *frame() const;

private:
    QWebHitTestResult(QWebHitTestResultPrivate *priv);
    QWebHitTestResultPrivate *d;

    friend class QWebFrame;
    friend class QWebPagePrivate;
    friend class QWebPage;
};

class QWEBKIT_EXPORT QWebFrame : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal textSizeMultiplier READ textSizeMultiplier WRITE setTextSizeMultiplier DESIGNABLE false)
    Q_PROPERTY(qreal zoomFactor READ zoomFactor WRITE setZoomFactor)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(QUrl requestedUrl READ requestedUrl)
    Q_PROPERTY(QUrl baseUrl READ baseUrl)
    Q_PROPERTY(QIcon icon READ icon)
    Q_PROPERTY(QSize contentsSize READ contentsSize)
    Q_PROPERTY(QPoint scrollPosition READ scrollPosition WRITE setScrollPosition)
    Q_PROPERTY(bool clipRenderToViewport READ clipRenderToViewport WRITE setClipRenderToViewport)
    Q_PROPERTY(bool focus READ hasFocus)
private:
    QWebFrame(QWebPage *parent, QWebFrameData *frameData);
    QWebFrame(QWebFrame *parent, QWebFrameData *frameData);
    ~QWebFrame();

public:
    QWebPage *page() const;

    void load(const QUrl &url);
#if QT_VERSION < 0x040400
    void load(const QWebNetworkRequest &request);
#else
    void load(const QNetworkRequest &request,
              QNetworkAccessManager::Operation operation = QNetworkAccessManager::GetOperation,
              const QByteArray &body = QByteArray());
#endif
    void setHtml(const QString &html, const QUrl &baseUrl = QUrl());
    void setContent(const QByteArray &data, const QString &mimeType = QString(), const QUrl &baseUrl = QUrl());

    void addToJavaScriptWindowObject(const QString &name, QObject *object);
    void addToJavaScriptWindowObject(const QString &name, QObject *object, QScriptEngine::ValueOwnership ownership);
    QString toHtml() const;
    QString toPlainText() const;
    QString renderTreeDump() const;

    QString title() const;
    void setUrl(const QUrl &url);
    QUrl url() const;
    QUrl requestedUrl() const;
    QUrl baseUrl() const;
    QIcon icon() const;
    QMultiMap<QString, QString> metaData() const;

    QString frameName() const;

    QWebFrame *parentFrame() const;
    QList<QWebFrame*> childFrames() const;

    Qt::ScrollBarPolicy scrollBarPolicy(Qt::Orientation orientation) const;
    void setScrollBarPolicy(Qt::Orientation orientation, Qt::ScrollBarPolicy policy);

    void setScrollBarValue(Qt::Orientation orientation, int value);
    int scrollBarValue(Qt::Orientation orientation) const;
    int scrollBarMinimum(Qt::Orientation orientation) const;
    int scrollBarMaximum(Qt::Orientation orientation) const;
    QRect scrollBarGeometry(Qt::Orientation orientation) const;

    void scroll(int, int);
    QPoint scrollPosition() const;
    void setScrollPosition(const QPoint &pos);

    void render(QPainter *painter, const QRegion &clip);
    void render(QPainter *painter);
    bool clipRenderToViewport() const;
    void setClipRenderToViewport(bool clipRenderToViewport);

    void setTextSizeMultiplier(qreal factor);
    qreal textSizeMultiplier() const;

    qreal zoomFactor() const;
    void setZoomFactor(qreal factor);

    bool hasFocus() const;
    void setFocus();

    QPoint pos() const;
    QRect geometry() const;
    QSize contentsSize() const;

    QWebElement documentElement() const;
    QList<QWebElement> findAllElements(const QString &selectorQuery) const;
    QWebElement findFirstElement(const QString &selectorQuery) const;

    QWebHitTestResult hitTestContent(const QPoint &pos) const;

    virtual bool event(QEvent *);

    QWebSecurityOrigin securityOrigin() const;

public Q_SLOTS:
    QVariant evaluateJavaScript(const QString& scriptSource);
#ifndef QT_NO_PRINTER
    void print(QPrinter *printer) const;
#endif

Q_SIGNALS:
    void javaScriptWindowObjectCleared();

    void provisionalLoad();
    void titleChanged(const QString &title);
    void urlChanged(const QUrl &url);

    void initialLayoutCompleted();

    void iconChanged();

    void contentsSizeChanged(const QSize &size);

    void loadStarted();
    void loadFinished(bool ok);

private:
    friend class QWebPage;
    friend class QWebPagePrivate;
    friend class QWebFramePrivate;
    friend class WebCore::WidgetPrivate;
    friend class WebCore::FrameLoaderClientQt;
    friend class WebCore::ChromeClientQt;
    QWebFramePrivate *d;
};

#endif
