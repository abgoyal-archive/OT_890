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
    Copyright (C) 2008, 2009 Nokia Corporation and/or its subsidiary(-ies)
    Copyright (C) 2008 Holger Hans Peter Freyther

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

#ifndef QWEBPAGE_P_H
#define QWEBPAGE_P_H

#include <qbasictimer.h>
#include <qnetworkproxy.h>
#include <qpointer.h>
#include <qevent.h>

#include "qwebpage.h"
#include "qwebhistory.h"
#include "qwebframe.h"

#include "KURL.h"
#include "PlatformString.h"

#include <wtf/RefPtr.h>

namespace WebCore {
    class ChromeClientQt;
    class ContextMenuClientQt;
    class ContextMenuItem;
    class ContextMenu;
    class EditorClientQt;
    class Element;
    class Node;
    class Page;
    class Frame;

#ifndef QT_NO_CURSOR
    class SetCursorEvent : public QEvent {
    public:
        static const int EventType = 724;
        SetCursorEvent(const QCursor&);

        QCursor cursor() const;
    private:
        QCursor m_cursor;
    };
#endif
}

QT_BEGIN_NAMESPACE
class QUndoStack;
class QMenu;
class QBitArray;
QT_END_NAMESPACE

class QWebPagePrivate {
public:
    QWebPagePrivate(QWebPage*);
    ~QWebPagePrivate();
    void createMainFrame();
#ifndef QT_NO_CONTEXTMENU
    QMenu* createContextMenu(const WebCore::ContextMenu* webcoreMenu, const QList<WebCore::ContextMenuItem>* items, QBitArray* visitedWebActions);
#endif
    void _q_onLoadProgressChanged(int);
    void _q_webActionTriggered(bool checked);
#ifndef NDEBUG
    void _q_cleanupLeakMessages();
#endif
    void updateAction(QWebPage::WebAction action);
    void updateNavigationActions();
    void updateEditorActions();

    void timerEvent(QTimerEvent*);

    void mouseMoveEvent(QMouseEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseDoubleClickEvent(QMouseEvent*);
    void mouseTripleClickEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent*);
#endif
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent*);
#endif
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
    void focusInEvent(QFocusEvent*);
    void focusOutEvent(QFocusEvent*);

    void dragEnterEvent(QDragEnterEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

    void inputMethodEvent(QInputMethodEvent*);

    void shortcutOverrideEvent(QKeyEvent*);
    void leaveEvent(QEvent *);
    bool handleScrolling(QKeyEvent*, WebCore::Frame*);

#ifndef QT_NO_SHORTCUT
    static QWebPage::WebAction editorActionForKeyEvent(QKeyEvent* event);
#endif
    static const char* editorCommandForWebActions(QWebPage::WebAction action);

    WebCore::ChromeClientQt *chromeClient;
    WebCore::ContextMenuClientQt *contextMenuClient;
    WebCore::EditorClientQt *editorClient;
    WebCore::Page *page;

    QPointer<QWebFrame> mainFrame;

    QWebPage *q;
#ifndef QT_NO_UNDOSTACK
    QUndoStack *undoStack;
#endif
    QWidget *view;

    bool insideOpenCall;
    quint64 m_totalBytes;
    quint64 m_bytesReceived;

    QPoint tripleClick;
    QBasicTimer tripleClickTimer;

#if QT_VERSION < 0x040400
    bool acceptNavigationRequest(QWebFrame *frame, const QWebNetworkRequest &request, QWebPage::NavigationType type);

    QWebNetworkInterface *networkInterface;
#ifndef QT_NO_NETWORKPROXY
    QNetworkProxy networkProxy;
#endif

#else
    bool acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, QWebPage::NavigationType type);
    QNetworkAccessManager *networkManager;
#endif

    bool forwardUnsupportedContent;
    QWebPage::LinkDelegationPolicy linkPolicy;

    QSize viewportSize;
    QSize fixedLayoutSize;
    QWebHistory history;
    QWebHitTestResult hitTestResult;
#ifndef QT_NO_CONTEXTMENU
    QPointer<QMenu> currentContextMenu;
#endif
    QWebSettings *settings;
    QPalette palette;
    bool editable;
    bool useFixedLayout;

    QAction *actions[QWebPage::WebActionCount];

    QWebPluginFactory *pluginFactory;

    static bool drtRun;
};

#endif
