//Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>
#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <libmscore/score.h>
#include <libmscore/part.h>
#include "../MuseWrap/musescore.h"
#include "../MuseWrap/seq.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);

    Ms::Score *getScore() const;
    void setScore(Ms::Score *value);


signals:

public slots:
    void play(int tick );
    void stop();


private:
    Ms::Score *score;
    Ms::MuseScoreCore *mscore;
    Ms::Seq *seq;

};

#endif // PLAYER_H
