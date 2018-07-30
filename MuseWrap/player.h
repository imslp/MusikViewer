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


#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <libmscore/score.h>
#include <libmscore/part.h>
#include "../MuseWrap/musescore.h"
#include "../MuseWrap/seq.h"
#include <QMediaPlayer>
#include <QTimer>
#include <QBuffer>
#include <QByteArray>
#include <QAudioOutput>

#if defined MuseWrapLib
 #define MuseWrap_COMMON_DLLSPEC Q_DECL_EXPORT
#else
 #define MuseWrap_COMMON_DLLSPEC Q_DECL_IMPORT
#endif

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);

    Ms::Score *getScore() const;
    void setScore(Ms::Score *value);


signals:
    void playChanged(bool playing);
    void heartBeat(int tick , qreal utick);

public slots:
    void play(int tick );
    void stop();
    void re_export();
    void positionChanged();
    void setPlaybackSpeed(qreal speed); // 1.0 for normal speed (percentage)

private slots:
    void mediaPlayerStateChanged(QMediaPlayer::State newState);
    void handleStateChanged(QAudio::State state);
    bool playrt();


private:
    Ms::Score *score;
    Ms::MuseScoreCore *mscore;
    QMediaPlayer m_player;
    QTimer m_timer;
    void exportWav();
    bool scoreChanged = false;
    QByteArray audioBuffer;
    QBuffer m_buffer;
    QAudioOutput *output=0;
    QAudioFormat format;
    Ms::MasterSynthesizer* synti;
    Ms::Seq *seq;

};

#endif // PLAYER_H
