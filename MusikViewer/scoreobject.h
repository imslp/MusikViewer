//Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>
#ifndef SCOREOBJECT_H
#define SCOREOBJECT_H

#include <QObject>
#include "../MuseWrap/musescore.h"
#include <libmscore/score.h>
#include <libmscore/part.h>
//#include <QMultimedia>
//#include <QAudioOutput>
#include <QBuffer>

#include "player.h"

class ScoreObject : public QObject
{
    Q_OBJECT
public:
    ScoreObject(QObject *parent = 0);

    Ms::Score *getScore() const;

signals:

public slots:
    void OpenScore(QString path);
    void setScale(double scale);
    double getScale();
    QStringList transposeValues();
    void transpose(int key);

    QList<Ms::Part *> getStaves();
    void muteStave(int pos, bool mute);
    void hideStave(int pos, bool hide);
    void setStaveVolume(int pos, int volume);
    void setTempo(qreal tempo);
    void play(int measure);


private:
      Ms::Score *score;
      Ms::MuseScore muse;
//      QAudioOutput audioOut;
      QBuffer audoIO;
      Player *m_player;

};

#endif // SCOREOBJECT_H
