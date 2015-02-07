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
    Copyright (C) 2009 Apple Inc. All rights reserved.

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

#ifndef MediaPlayerPrivatePhonon_h
#define MediaPlayerPrivatePhonon_h

#include "MediaPlayerPrivate.h"

#include <QObject>
#include <phononnamespace.h>

QT_BEGIN_NAMESPACE
class QWidget;
class QUrl;

namespace Phonon {
    class MediaObject;
    class VideoWidget;
    class AudioOutput;
    class MediaSource;
}
QT_END_NAMESPACE

namespace WebCore {

    class MediaPlayerPrivate : public QObject, public MediaPlayerPrivateInterface {

        Q_OBJECT

    public:
        static void registerMediaEngine(MediaEngineRegistrar);
        ~MediaPlayerPrivate();

        // These enums are used for debugging
        Q_ENUMS(ReadyState NetworkState PhononState)

        enum ReadyState {
            HaveNothing, 
            HaveMetadata, 
            HaveCurrentData, 
            HaveFutureData, 
            HaveEnoughData
        };

        enum NetworkState {
            Empty, 
            Idle, 
            Loading, 
            Loaded, 
            FormatError, 
            NetworkError, 
            DecodeError
        };

        enum PhononState {
            LoadingState,
            StoppedState,
            PlayingState,
            BufferingState,
            PausedState,
            ErrorState
        };

        IntSize naturalSize() const;
        bool hasVideo() const;

        void load(const String &url);
        void cancelLoad();

        void play();
        void pause();

        bool paused() const;
        bool seeking() const;

        float duration() const;
        float currentTime() const;
        void seek(float);
        void setEndTime(float);

        void setRate(float);
        void setVolume(float);
        void setMuted(bool);

        int dataRate() const;

        MediaPlayer::NetworkState networkState() const;
        MediaPlayer::ReadyState readyState() const;

        float maxTimeBuffered() const;
        float maxTimeSeekable() const;
        unsigned bytesLoaded() const;
        bool totalBytesKnown() const;
        unsigned totalBytes() const;

        void setVisible(bool);
        void setSize(const IntSize&);

        void paint(GraphicsContext*, const IntRect&);

    protected:
        bool eventFilter(QObject*, QEvent*);

    private slots:
        void stateChanged(Phonon::State, Phonon::State);
        void metaDataChanged();
        void seekableChanged(bool);
        void hasVideoChanged(bool);
        void bufferStatus(int);
        void finished();
        void currentSourceChanged(const Phonon::MediaSource&);
        void aboutToFinish();
        void totalTimeChanged(qint64);

    private:
        MediaPlayerPrivate(MediaPlayer*);
        static MediaPlayerPrivateInterface* create(MediaPlayer* player);

        static void getSupportedTypes(HashSet<String>&);
        static MediaPlayer::SupportsType supportsType(const String& type, const String& codecs);
        static bool isAvailable() { return true; }

        void updateStates();

        MediaPlayer* m_player;

        MediaPlayer::NetworkState m_networkState;
        MediaPlayer::ReadyState m_readyState;

        Phonon::MediaObject* m_mediaObject;
        Phonon::VideoWidget* m_videoWidget;
        Phonon::AudioOutput* m_audioOutput;

        bool m_isVisible;
    };
}

#endif // MediaPlayerPrivatePhonon_h
