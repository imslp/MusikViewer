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


#include "applicationui.h"
#include "libmscore/page.h"
#include "QFileInfo"
#include <QStandardPaths>
#include <QMimeDatabase>
#include <QMimeType>
#include "androidExtra/AndroidWrapper.h"


ApplicationUI::ApplicationUI(QObject *parent) : QObject(parent)
{
    //        obj.OpenScore(QDir::currentPath() + "/Two.mscz");
    //    qDebug() << "Open Score path" << QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)+ "/Two.mscz";

    //    obj.OpenScore(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)+ "/Two.mscz");
    //     obj.OpenScore("Two.mscz");
    //       obj.getScore()->setShowVBox(false);
    //       obj.getScore()->cropPage(0);

    //qDebug() << "Page Format" << obj.getScore()->pageFormat()->evenTopMargin()  << obj.getScore()->pageFormat()->evenRightMargin() <<  obj.getScore()->pageFormat()->oddTopMargin() <<  obj.getScore()->pageFormat()->oddRightMargin();

    connect(&obj , &ScoreObject::UpdateNeeded , this, &ApplicationUI::UpdateNeeded);
    connect(&obj,&ScoreObject::selectedRectangle , this, &ApplicationUI::selectedRectangle);
    connect(&obj,&ScoreObject::pageChanged , this , &ApplicationUI::pageChanged);
    connect(&obj , &ScoreObject::playChanged , this , &ApplicationUI::playingChanged);
    connect(&obj , &ScoreObject::exportProgress , this , &ApplicationUI::exportProgress);

#ifdef Q_OS_ANDROID
    //    connect(&obj , &ScoreObject::saveCompleted , &futils , &FileDialogUtils::closeFD);
    Android=true;
    AndroidChanged();
    connect(&futils , &FileDialogUtils::actionCompleted , this , &ApplicationUI::onActionCompleted);
#endif

    QString share_temp = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).value(0) + "/temp_share";

    QDir("").mkpath(share_temp);

}

ApplicationUI::~ApplicationUI()
{
        QString share_temp = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).value(0) + "/temp_share";
        QDir dir(QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).value(0));
        dir.rmdir("temp_share");
//        dir.remove("temp_share");
}

void ApplicationUI::setEngine(QQmlEngine *value)
{
#ifdef Q_OS_ANDROID
    futils.setEngine(value);
    futils.populateShortcuts();
#endif

}

void ApplicationUI::setScoreView(Ms::MsScoreView *view)
{
    m_view = view;

    if(obj.getScore()==0)
        return;

    view->setScore(obj.getScore());
    obj.setUIscale(m_view->scale());
    //    obj.play(0);
}

void ApplicationUI::UpdateNeeded()
{
    if(obj.getScore()==0)
        return;

    //    qDebug() << Q_FUNC_INFO;
    if(m_view!=0){
        //        qDebug() << Q_FUNC_INFO;
        m_view->update();
        obj.setUIscale(m_view->scale());
    }
}

void ApplicationUI::clicked(qreal x, qreal y)
{
    qDebug() << Q_FUNC_INFO << "X" << x << "y" << y;

    if(obj.getScore()==0)
        return;

    qreal topMargin =obj.getScore()->pageFormat()->evenTopMargin();
    qreal rightMargin =  obj.getScore()->pageFormat()->evenRightMargin();
    qreal scale = m_view->scale();

    //    obj.elementClicked((x-(Ms::DPI_DISPLAY*rightMargin))/scale,(y-(Ms::DPI_DISPLAY*topMargin))/scale);

    obj.elementClicked(x/scale,y/scale);

}

void ApplicationUI::play()
{

    obj.play(0);
}

void ApplicationUI::stop()
{
    obj.stop();
}

int ApplicationUI::maxPage()
{
    return obj.pages();
}

void ApplicationUI::gotoPage(int page)
{
    obj.setCurrentPage(page-1);
}

int ApplicationUI::currentPage()
{
    return obj.getCurrentPage()+1;
}

bool ApplicationUI::isPlaying()
{
    obj.getPlaying();
}

void ApplicationUI::openScore(QString path)
{
    qDebug() << Q_FUNC_INFO << path;
    //    obj.OpenScore(QDir::currentPath() + "/Two.mscz");
    QUrl url(path);
    obj.OpenScore(url.toLocalFile());

    Ms::Score *score = obj.getScore();

    Ms::Page* page = score->pages()[0];

    QRectF f= page->abbox();


    if(m_view!=NULL){
        m_view->setScore(obj.getScore());
        obj.setUIscale(m_view->scale());
    }

}

ScoreObject *ApplicationUI::getScore()
{
    return &obj;
}

void ApplicationUI::selectFile()
{
    //    awrp.selectFile(false);
}

void ApplicationUI::exportPDF(QString path)
{


    if(obj.getScore()==0)
        return;

    qDebug() << Q_FUNC_INFO << path;


#ifdef Q_OS_ANDROID

    exportExt= "pdf";

    QString name;
    if(!path.startsWith("/")){
        QUrl url(path);
        path = url.toLocalFile();
    }


    QFileInfo info(path);

    name = info.fileName();

    if(info.fileName().isEmpty()){
        path.append(obj.getScore()->name());
        path.append(".pdf");
        name.append(obj.getScore()->name());
        name.append(".pdf");
    }else if(info.completeSuffix().isEmpty() || info.completeSuffix() !="pdf" ){
        name.append(".pdf");
        path.append(".pdf");
    }

    futils.androidItent(name);

#else
    obj.exportPDF(path);
#endif
}

void ApplicationUI::saveAs(QString path, QString ext)
{

    connect(&obj , &ScoreObject::saveCompleted , &futils , &FileDialogUtils::closeFD);


    if(obj.getScore()==0)
        return;

    qDebug() << Q_FUNC_INFO << path;


#ifdef Q_OS_ANDROID

    exportExt = ext;

    QString name;
    if(!path.startsWith("/")){
        QUrl url(path);
        path = url.toLocalFile();
    }


    QFileInfo info(path);

    name = info.fileName();

    if(info.fileName().isEmpty()){
        path.append(obj.getScore()->name());
        path.append("."+ext);
        name.append(obj.getScore()->name());
        name.append("."+ext);
    }else if(info.completeSuffix().isEmpty() || info.completeSuffix() !=ext ){
        name.append("."+ext);
        path.append("."+ext);
    }

    futils.androidItent(name);

#else
    obj.saveAs(path);
#endif
}

void ApplicationUI::share(QString ext)
{
    exportExt = ext;
    connect(&obj , &ScoreObject::saveCompleted , this , &ApplicationUI::onShareSaveDone);
    QString name;
    name.append(obj.getScore()->name());
    name.append("."+ext);
    shareFile= new  QFile (QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).value(0)+"/temp_share/"+ name , this);
    shareFile->open(QIODevice::ReadWrite);

    obj.saveAs(shareFile->handle() , ext);

}

void ApplicationUI::onShareSaveDone()
{

    disconnect(&obj , &ScoreObject::saveCompleted , this , &ApplicationUI::onShareSaveDone);

    shareFile->close();

    QMimeDatabase mdb;

    QMimeType mimeType = mdb.mimeTypeForData(shareFile);

    QString mime = mimeType.name();
    if(exportExt =="mscz" || exportExt =="mscx")
        mime = "*/*";

#ifdef Q_OS_ANDROID

    m_shareUtils.sendFile(shareFile->fileName() , "Share Using" , mime , -1 , false);
#endif

    shareFile->deleteLater();

}


void ApplicationUI::save()
{
    if(obj.getScore()==0)
        return;

    obj.getScore()->saveFile();


}


#ifdef Q_OS_ANDROID
void ApplicationUI::onActionCompleted(QString path , int fd)
{
    qDebug() << Q_FUNC_INFO << fd << path;
    //    obj.exportPDF(fd);
    obj.saveAs(fd, exportExt);

    //    QFile file;
    //    if(file.open(fd, QFile::ReadWrite)){
    //        qDebug() << "File Open";
    //        file.write("This is a test");
    //        file.close();
    //    }else{
    //        qDebug() << "Failed to open file!";
    //        close(fd);
    //    }

}
#endif

bool ApplicationUI::getAndroid() const
{
    return Android;
}


void ApplicationUI::pageChanged(int page)
{
    if(m_view!=0){
        m_view->setCurrentPage(page);
        emit currentPageChanged();
    }
}

