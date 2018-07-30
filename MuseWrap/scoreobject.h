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


#ifndef SCOREOBJECT_H
#define SCOREOBJECT_H

#include <QObject>
#include "../MuseWrap/musescore.h"
#include <libmscore/score.h>
#include <libmscore/part.h>
//#include <QMultimedia>
//#include <QAudioOutput>
#include <QBuffer>
#include "preferences.h"

#include "player.h"
#include <QThread>

#if defined MuseWrapLib
 #define MuseWrap_COMMON_DLLSPEC Q_DECL_EXPORT
#else
 #define MuseWrap_COMMON_DLLSPEC Q_DECL_IMPORT
#endif

class ScoreObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList transposeValues READ transposeValues)
    Q_PROPERTY(qreal spatium READ getScale WRITE setScale  NOTIFY spatiumChanged)
    Q_PROPERTY(int maxPage READ pages NOTIFY pagesChanged)
    Q_PROPERTY(qreal tempo READ getTempo WRITE setTempo  NOTIFY tempoChanged)
    Q_PROPERTY(Ms::Score* score READ getScore  NOTIFY scoreChanged)
    Q_PROPERTY(int nstaves READ nstaves NOTIFY nstavesChanged)
    Q_PROPERTY(QString name READ getname  NOTIFY scoreChanged)
public:
    ScoreObject(QObject *parent = 0);
    ~ScoreObject();

    Ms::Score *getScore() const;

    void setUIscale(const qreal &value);

    int getCurrentPage() const;

    void setCurrentPage(int value);

    bool getPlaying() const;
    void setPlaying(bool value);

    QString getname();

signals:
    void UpdateNeeded();
    void selectedRectangle(QRectF rectangle);
    void pageChanged(int page);
    void playChanged();
    void spatiumChanged();
    void pagesChanged();
    void tempoChanged();
    void scoreChanged();
    void nstavesChanged();

    void saveCompleted();
    void exportProgress(float progress);

public slots:
    void OpenScore(QString path);
    void setScale(double scale);
    qreal getScale();
    QStringList transposeValues();
    void transpose(int key);

    QList<Ms::Part *> getStaves();
    int nstaves();
    int getVolume(int pos);
    bool getMuted(int pos);
    bool getStavevisible(int pos);
    void muteStave(int pos, bool mute);
    void hideStave(int pos, bool hide);
    void setStaveVolume(int pos, int volume);
    void setTempo(qreal tempo);
    qreal getTempo();
    void play(int measure);
    void stop();
    void elementClicked(qreal x, qreal y);
    int pages();
    void exportPDF(QString path);
    void exportPDF(int fd);
    void saveAs(int fd , QString ext );
    void cancelExport();


private slots:
    void onheartbeat(qint64 tick , int utick , int endframe);
    void onPlayStarted();
    void onPlayStoped();
    void onPlayStateChanged(bool _playing);



private:
      Ms::Score *score =0;
      Ms::MuseScore *muse;
      Ms::Measure *measure;

//      QAudioOutput audioOut;

      QBuffer audoIO;
      Player *m_player;
      qreal UIscale=1;
      int currentPage=0;
      bool playing = false;
      qreal tempoMulti =1.0;
      qreal tempoOrig; //in BPS multiply by 60 to get BPM;
      QThread seqThread;
      QString name;

};


#endif // SCOREOBJECT_H
