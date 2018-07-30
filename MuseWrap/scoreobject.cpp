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




#include "scoreobject.h"
//#include <mscore/pagesettings.h>
#include <libmscore/score.h>
//#include <mscore/transposedialog.h>
#include <libmscore/measurebase.h>
#include <libmscore/measure.h>
#include <libmscore/repeatlist.h>
#include <QStringList>
#include <libmscore/page.h>
#include <libmscore/system.h>
#include <QPointF>
#include <QRectF>
#include <QFileInfo>
#include <libmscore/tempo.h>

#include "qaudiodriver.h"
#include <QtConcurrent>

namespace Ms {

MuseScore* mscore;
MasterSynthesizer* synti;

bool enableExperimental = false;
bool enableTestMode = false;

QString dataPath;
QString iconPath;

bool converterMode = false;
bool processJob = false;
bool externalIcons = false;
bool pluginMode = false;
static bool startWithNewScore = false;
double converterDpi = 100;
double guiScaling = 0.0;
static double userDPI = 0.0;
int trimMargin = -1;
bool noWebView = false;
bool exportScoreParts = false;
bool ignoreWarnings = false;

QString mscoreGlobalShare;

static QString outFileName;
static QString jsonFileName;
static QString audioDriver;
static QString pluginName;
static QString styleFile;
static bool scoresOnCommandline { false };

QString localeName;
bool useFactorySettings = false;
bool deletePreferences = false;
QString styleName;
QString revision;
const char* voiceActions[] = { "voice-1", "voice-2", "voice-3", "voice-4" };

extern bool savePositions(Score*, const QString& name, bool segments );
//extern TextPalette* textPalette;

static constexpr double SCALE_MAX  = 16.0;
static constexpr double SCALE_MIN  = 0.05;
static constexpr double SCALE_STEP = 1.7;
}

using namespace Ms;

ScoreObject::ScoreObject(QObject *parent) : QObject(parent)
{

    if(mscore==0){
        mscore = new MuseScore(this);        
    }

    connect(mscore , &MuseScore::saveCompleted , this , &ScoreObject::saveCompleted);
    connect(mscore , &MuseScore::exportProgress, this , &ScoreObject::exportProgress);

    muse = mscore;

    if(MScore::seq==0){

        qDebug() << "Creating Seq!";

        seq =  new Seq();
        connect(&seqThread , &QThread::started , seq , &Seq::threadStarted);
        seq->moveToThread(&seqThread);
        seqThread.start(QThread::TimeCriticalPriority);
        MScore::seq    = seq;
        Driver* driver = driverFactory(seq, "qaudio");

        QAudioDriver *_driver = dynamic_cast<QAudioDriver*>(driver);

        if(_driver != nullptr){
            _driver->moveToThread(&seqThread);
        }

        synti          = synthesizerFactory();
        synti->moveToThread(&seqThread);
        if (driver) {
            MScore::sampleRate = driver->sampleRate();
            synti->setSampleRate(MScore::sampleRate);
            synti->init();
            seq->setDriver(driver);
        }
        else {
            // Do not delete the sequencer If we can't load driver.
            // Allow user to select the working driver later.
            MScore::sampleRate = 44100;  // Would be changed when user changes driver
            synti->setSampleRate(MScore::sampleRate);
            synti->init();
        }
        seq->setMasterSynthesizer(synti);
        if (!seq->init())
            qDebug("sequencer init failed");
        else{
            connect(seq,&Seq::heartBeat , this, &ScoreObject::onheartbeat);
            connect(seq,&Seq::started , this, &ScoreObject::onPlayStarted);
            connect(seq,&Seq::stopped , this , &ScoreObject::onPlayStoped);
        }

    }



    m_player= new Player(this);

    //    connect(m_player , &Player::playChanged , this , &ScoreObject::onPlayStateChanged);
    //    connect(m_player, &Player::heartBeat , this , &ScoreObject::onheartbeat);

}

ScoreObject::~ScoreObject()
{
    seqThread.quit();
    seqThread.wait(1000);
    //    seq->moveToThread(this);
    seq->deleteLater();

}

void ScoreObject::OpenScore(QString path)
{
    if (path.isEmpty())
        return ;

    Ms::Score *_score = muse->openScore(path);

    if(_score==0)
        return;

    score->deleteLater();
    score=0;
    measure=0;

    score = _score;

    Ms::PageFormat *format = score->pageFormat();

//    format->setEvenBottomMargin(0);
//    format->setEvenLeftMargin(0);
//    format->setEvenTopMargin(0);
//    format->setOddBottomMargin(0);
//    format->setOddLeftMargin(0);
//    format->setOddTopMargin(0);

//    score->update();


    Page* page = score->pages()[0];
    QRectF pr(page->abbox());

    Ms::Text *txt = score->getText(Ms::TextStyleType::TITLE);
//       txt->parent()->setVisible(false);
//    txt->setVisible(false);


    name = txt->xmlText();


    muse->setCurrentScore(score);

    qDebug() << "Score Opened" << score->name();
    score->setShowFrames(false);
    score->updateRepeatList(true);
    score->setPlayPos(0);

    m_player->setScore(score);
    tempoOrig = score->tempo(0);

    currentPage =0;
    playing = false;
     emit scoreChanged();
    emit pageChanged(0);
    emit playChanged();
    emit spatiumChanged();
    emit pagesChanged();
    emit tempoChanged();
    emit nstavesChanged();

}

void ScoreObject::setScale(double scale)
{
    //    muse.pageSettings->spatiumChanged(scale);
    //    muse.pageSettings->apply();

    if(score==0)
        return;


    qDebug() << "Spatium " << scale;


    double old = score->spatium();

    score->setSpatium(scale*DPMM);
    score->spatiumChanged(old,scale*DPMM);
    //    spatiumChanged();
    score->doLayout();

    emit pagesChanged();

    if(currentPage > score->npages()-1){
        currentPage = score->npages()-1;
        emit pageChanged(currentPage);
    }

    emit spatiumChanged();

    UpdateNeeded();
}

qreal ScoreObject::getScale()
{
    if(score==0)
        return 0;

    qDebug() << Q_FUNC_INFO << score->spatium()/DPMM;

    double spacemm = score->spatium()/DPMM;

    double spacecut;


    int tmp = spacemm*100;
    spacecut = tmp /100.0;

    return spacecut;
}

QStringList ScoreObject::transposeValues()
{
    QStringList names;

    names          << QApplication::translate("TransposeDialogBase", "C\342\231\255 major / A\342\231\255 minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "G\342\231\255 major / E\342\231\255 minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "D\342\231\255 major / B\342\231\255 minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "A\342\231\255 major / F minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "E\342\231\255 major / C minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "B\342\231\255 major / G minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "F major / D minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "C major / A minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "G major / E minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "D major / B minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "A major / F\342\231\257 minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "E major / C\342\231\257 minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "B major / G\342\231\257 minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "F\342\231\257 major / D\342\231\257 minor", Q_NULLPTR)
                   << QApplication::translate("TransposeDialogBase", "C\342\231\257 major / A\342\231\257 minor", Q_NULLPTR);

    return names;
}

void ScoreObject::transpose(int key)
{
    if(score==0)
        return;

//    bool noSelection = score->selection().isNone();
//    if (noSelection)
        score->cmdSelectAll();

    score->transpose(TransposeMode::BY_KEY, TransposeDirection::CLOSEST, Ms::Key(key-7), 1, true , true, true);

    score->doLayout();

//    if (noSelection)
        score->deselectAll();

    UpdateNeeded();
}

QList<Ms::Part *> ScoreObject::getStaves()
{
    if(score==0)
        return QList<Ms::Part *>();

    return score->parts();
}

int ScoreObject::nstaves()
{
    if(score==0)
        return 0;

    return getStaves().size();
}

int ScoreObject::getVolume(int pos)
{

    if(score==0)
        return 0;

    if(pos>=0 && getStaves().size()>pos)
        return getStaves()[pos]->volume();
    return 0;
}

bool ScoreObject::getMuted(int pos)
{
    if(score==0)
        return true;

    if(pos>=0 && getStaves().size()>pos)
        return getStaves()[pos]->mute();
    return false;
}

bool ScoreObject::getStavevisible(int pos)
{

    if(score==0)
        return false;
    if(pos>=0 && getStaves().size()>pos)
        return getStaves()[pos]->show();
    return false;
}

void ScoreObject::muteStave(int pos , bool mute)
{
    if(score==0)
        return;


    if(pos<0 || getStaves().size()-1<pos)
        return;


    if(pos>=0 && getStaves().size()>pos)
        getStaves()[pos]->setMute(mute);



    //    Ms::Part *part = getStaves()[pos];

    Ms::Instrument *instrument = getStaves()[pos]->instrument();
    QList<Channel*>  channels = instrument->channel();

    for(int i=0 ; i< channels.size() ; i++){
        seq->setController(channels[i]->channel, CTRL_VOLUME, mute ? 0 :  getVolume(pos));
    }

}

void ScoreObject::hideStave(int pos , bool hide)
{

    if(score==0)
        return;
    qDebug() << Q_FUNC_INFO << hide;

    if(pos>=0 && getStaves().size()>pos)
        getStaves()[pos]->setShow(!hide);
    score->doLayout();
    UpdateNeeded();

}

void ScoreObject::setStaveVolume(int pos, int volume)
{
    if(score==0)
        return;

    qDebug() << Q_FUNC_INFO << volume;

    if(pos<0 || getStaves().size()-1<pos)
        return;

    //    Ms::Part *part = getStaves()[pos];

    Ms::Instrument *instrument = getStaves()[pos]->instrument();
    QList<Channel*>  channels = instrument->channel();

    for(int i=0 ; i< channels.size() ; i++){
        //                 int iv = lrint(volume);
        seq->setController(channels[i]->channel, CTRL_VOLUME, volume);
        channels[i]->volume=volume;
        channels[i]->updateInitList();
    }
    //    m_player->re_export();

}

void ScoreObject::setTempo(qreal tempo)
{

    if(score==0)
        return;

    ///TODO: Update seq to the new tempo, using percentage

    score->setTempo(0 ,tempo/60 );
    score->updateRepeatList(true);
    score->tempomap()->setRelTempo((tempo/60)/tempoOrig);
    score->update();

    tempoMulti = (tempo/60)/tempoOrig;

    qDebug() << "Tempo Multi" << tempoMulti;

    //    m_player->re_export();

    //    m_player->setPlaybackSpeed((tempo/60)/score->tempo(0));

    seq->setRelTempo((tempo/60)/score->tempo(0));


     emit tempoChanged();

}

qreal ScoreObject::getTempo()
{
    //    return score->tempo(0);

    if(score==0)
        return 0;
    //    return score->tempo(0)*60;

    return seq->curTempo()*60;
}

void ScoreObject::play(int measure)
{
    Q_UNUSED(measure);

    if(score==0)
        return;

    score->updateRepeatList(true);

    //        muse->_sstate = Ms::STATE_PLAY;
    ////        score->updateSelection();
    //    Ms::Selection sel = score->selection();


    //    qDebug() << "PlayPos tick" << score->playPos();

    //    int tick = score->playPos();
    //    //        int tick;
    //    if(sel.elements().isEmpty())
    //        tick =0;
    //    else
    //        tick = sel.findMeasure()->tick();

    //    qDebug() << "Measure Tick" << tick;


    if(this->measure!=0){
        m_player->play(this->measure->tick());
    }else{
        m_player->play(0);
    }

}

void ScoreObject::stop()
{

    if(score==0)
        return;
    m_player->stop();
}

void ScoreObject::elementClicked(qreal x, qreal y)
{

    if(score==0)
        return;

    QPointF point(x,y);
    int staffidx , pitch;
    Ms::Segment* segment;
    QPointF offset;


    //        Ms::MeasureBase *measure = score->pos2measure(point, &staffidx , &pitch  , &segment , &offset);

    //    Ms::MeasureBase *measure = score->pages().at(currentPage)->pos2measure(point, &staffidx , &pitch  , &segment , &offset);

    //    Ms::Measure *measure =  score->pages().at(currentPage)->searchMeasure(point);

    measure = static_cast<Ms::Measure*>(score->pages().at(currentPage)->pos2measure(point, &staffidx , &pitch  , &segment , &offset));

    score->setPlayPos(measure->tick());


    score->deselectAll();

    if(measure==0){
        QRectF rect;
        emit selectedRectangle(rect);
        emit UpdateNeeded();
        return;
    }

    qDebug() << measure << staffidx << pitch << offset.x() << offset.y();

    score->select(measure,Ms::SelectType::SINGLE,staffidx);


    QRectF rect = measure->pageBoundingRect();

    qreal _x,_y,width,height;
    _x = rect.x()*UIscale;
    _y = rect.y()*UIscale;
    width = rect.width()*UIscale;
    height = rect.height()*UIscale;

    rect.setX(_x);
    rect.setY(_y);
    rect.setWidth(width);
    rect.setHeight(height);



    emit selectedRectangle(rect);

    emit UpdateNeeded();

}

int ScoreObject::pages()
{

    if(score==0)
        return 0;
    return score->npages();
}

void ScoreObject::exportPDF(QString path)
{
    if(score==0)
        return;

    if(!path.startsWith("/")){
        QUrl url(path);
        path = url.toLocalFile();
    }

    QFileInfo info(path);
    if(info.fileName().isEmpty()){
        path.append(score->name());
        path.append(".pdf");
    }else if(info.completeSuffix().isEmpty() || info.completeSuffix() !="pdf" ){
        path.append(".pdf");
    }

    qDebug() << "Export PDF returned" << muse->savePdf(path);
}

void ScoreObject::exportPDF(int fd)
{
    muse->savePdf(fd);
}

void ScoreObject::saveAs(int fd, QString ext)
{
    if(score==0)
        return;

//    muse->saveAs(score,false ,fd , ext , currentPage);

    QtConcurrent::run(muse , &MuseScore::saveAs , score , false , fd , ext , currentPage);
}

void ScoreObject::cancelExport()
{
    mscore->cancelExport();
}

void ScoreObject::onheartbeat(qint64 tick , int utick , int endframe)
{

    if(score==0)
        return;

    //    qDebug() << Q_FUNC_INFO << "Tempo after multiply" << tick*tempoMulti*tempoMulti;

    //    int _tick = score->tempomap()->time2tick(tick); //tick is the player time.
    //    qreal _utick = score->utime2utick(utick);
    //    int _tick = score->repeatList()->utick2tick(_utick);


// Ms::Measure *_measure = score->tick2measure(tick*tempoMulti*tempoMulti);
    Ms::Measure *_measure = score->tick2measure(tick);

    if(!_measure)
        return;

    //    Ms::Page *page = score->searchPage(_measure->pagePos());

    int pageNum = _measure->system()->page()->no();

    //    int pageNum = score->pageIdx(page);

    //    qDebug() << "Heartbeat page num" << pageNum;

    if(pageNum!=currentPage){
        currentPage = pageNum;
        emit pageChanged(currentPage);
    }




    Ms::MeasureBase *measure = score->tick2measureBase(tick);

    if(!measure)
        return;

    QRectF rect = measure->pageBoundingRect();


    //    measure->

    //    score->select(measure,Ms::SelectType::SINGLE, );

    qreal _x,_y,width,height;
    _x = rect.x()*UIscale;
    _y = rect.y()*UIscale;
    width = rect.width()*UIscale;
    height = rect.height()*UIscale;

    rect.setX(_x);
    rect.setY(_y);
    rect.setWidth(width);
    rect.setHeight(height);

    emit selectedRectangle(rect);

    emit UpdateNeeded();
}

void ScoreObject::onPlayStarted()
{
    playing = true;
    emit playChanged();
}

void ScoreObject::onPlayStoped()
{
    playing = false;
    emit playChanged();
}

void ScoreObject::onPlayStateChanged(bool _playing)
{
    playing = _playing;
    emit playChanged();
}

bool ScoreObject::getPlaying() const
{
    return playing;
}

void ScoreObject::setPlaying(bool value)
{
    playing = value;
}

QString ScoreObject::getname()
{
    if(score==0)
        return 0;

    if(name.isEmpty())
        return score->name();
    return name;
}

void ScoreObject::setCurrentPage(int value)
{
    currentPage = value;
    emit pageChanged(currentPage);
}

int ScoreObject::getCurrentPage() const
{
    return currentPage;
}

void ScoreObject::setUIscale(const qreal &value)
{
    UIscale = value;
}
Ms::Score *ScoreObject::getScore() const
{
    return score;
}


