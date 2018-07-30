/* ==========================================================================================
//   						GPL License and Copyright Notice
// This file is part of MuseViewer.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with MuseViewer; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>
// ========================================================================================== */


#ifndef QAUDIODRIVER_H
#define QAUDIODRIVER_H

#include <QObject>
#include <QBuffer>
#include <QByteArray>
#include <QAudioOutput>
#include <QThread>
#include "driver.h"
#include "../MuseWrap/musescore.h"
#include <QAudio>
#include <QElapsedTimer>
#include <QTimer>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

using namespace Ms;

class QAudioDriver : public QObject , public Driver
{
    Q_OBJECT
public:
    QAudioDriver(Ms::Seq *seq);
    ~QAudioDriver();
    // Driver interface
    bool init(bool hot) Q_DECL_OVERRIDE;
    bool start(bool hotPlug) Q_DECL_OVERRIDE;
    bool stop() Q_DECL_OVERRIDE;
    void stopTransport() Q_DECL_OVERRIDE;
    void startTransport() Q_DECL_OVERRIDE;
    Transport getState() Q_DECL_OVERRIDE;
    int sampleRate() const Q_DECL_OVERRIDE;


private slots:
    void handleStateChanged(QAudio::State state);
    void onAudioNotify();
    void processFinished();


private:
    Transport state;

    QByteArray audioBuffer;
    QBuffer m_buffer;
    QAudioOutput *output=0;
    QAudioFormat format;
    Ms::MasterSynthesizer* synti;
    QThread audiothread;

    unsigned int frames;
    float *buffer;  
    int bufferlength;//msecs

    QAudio::State prevState = QAudio::IdleState;

    QElapsedTimer playTime;
    QTimer timer;
    bool m_first=true;
//    QFutureWatcher<void> watcher;
//    bool processing=false;
};

#endif // QAUDIODRIVER_H
