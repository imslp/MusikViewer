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


#ifndef APPLICATIONUI_H
#define APPLICATIONUI_H

#include <QObject>
#include "../MuseWrap/scoreobject.h"
#include <libmscore/plugins.h>
#include <QQuickPaintedItem>
#include <QQmlEngine>
#include <shareutils.hpp>

#ifdef Q_OS_ANDROID
    #include "androidExtra/filedialogutils.h"
#endif

class ApplicationUI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int maxPage READ maxPage NOTIFY maxpageChanged)
    Q_PROPERTY(int currentPage READ currentPage  NOTIFY currentPageChanged)
    Q_PROPERTY(bool playing READ  isPlaying NOTIFY playingChanged)
    Q_PROPERTY(ScoreObject* score READ getScore NOTIFY scoreChanged)
    Q_PROPERTY(bool Android READ getAndroid NOTIFY AndroidChanged)
public:
    explicit ApplicationUI(QObject *parent = nullptr);
    ~ApplicationUI();
    void setEngine(QQmlEngine *value);


    bool getAndroid() const;

signals:
    void selectedRectangle(QRectF rect);
    void currentPageChanged();
    void maxpageChanged();
    void playingChanged();
    void scoreChanged();
    void AndroidChanged();
    void exportProgress(float progress);   


public slots:
    void setScoreView(Ms::MsScoreView *view);
    void UpdateNeeded();
    void clicked(qreal x, qreal y);
    void play();
    void stop();
    void pageChanged(int page);
    int maxPage();
    void gotoPage(int page);
    int currentPage();
    bool isPlaying();
    void openScore(QString path);
    ScoreObject *getScore();
    void selectFile();
    void exportPDF(QString path);
    void saveAs(QString path , QString ext);
    void share(QString ext);
    void onShareSaveDone();
    void save();     
#ifdef Q_OS_ANDROID
    void onActionCompleted(QString path, int fd);
    void onApplicationStateChanged(Qt::ApplicationState applicationState);
#endif



private:
    ScoreObject obj;
    Ms::MsScoreView *m_view=NULL;
    bool Android=false;
    QString exportExt;
    QFile *shareFile;
    ShareUtils m_shareUtils;



#ifdef Q_OS_ANDROID
FileDialogUtils futils;
#endif

};

#endif // APPLICATIONUI_H
