//Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>
#include <QGuiApplication>
#include "scoreobject.h"
#include "player.h"

#if 0


int main(int argc, char *argv[])
{

    QGuiApplication app(argc , argv);
    Ms::MScore::init();
    ScoreObject obj;
    obj.OpenScore(QDir::currentPath() + "/Two.mscz");

    obj.play(0);

    return app.exec();
}
#endif
