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


#include "player.h"
#include "musescore.h"
#include <libmscore/repeatlist.h>
#include <libmscore/measure.h>
#include <preferences.h>
#include <QTimer>
#include <QBuffer>
#include <QByteArray>
#include <QAudioOutput>

using namespace Ms;


Player::Player(QObject *parent) : QObject(parent)
{
    seq = static_cast<Ms::Seq*>(Ms::MScore::seq);
//    //    connect(&m_player,QMediaPlayer::positionChanged , this , &Player::positionChanged);
//    synti = Ms::synthesizerFactory();
//    synti->init();

//    connect(&m_timer , &QTimer::timeout , this , &Player::positionChanged );
//    connect(&m_player , &QMediaPlayer::stateChanged , this , &Player::mediaPlayerStateChanged );


//    format.setChannelCount(1);
//    format.setSampleRate(Ms::preferences.exportAudioSampleRate);
//    format.setSampleType(QAudioFormat::Float);
//    format.setSampleSize(32);
////    format.setByteOrder(QAudioFormat::LittleEndian);
//    format.setCodec("audio/pcm");


//    qDebug() << "Bytes per frame" << format.bytesPerFrame();
//    qDebug() << "Sample Rate" << format.sampleRate();

//    output = new QAudioOutput(format , this);


//    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
//    if (!info.isFormatSupported(format)) {
//        qWarning() << "Raw audio format not supported by backend, cannot play audio.";


//        return;
//    }

    //    return;

//    m_buffer.setBuffer(&audioBuffer);
//    m_buffer.open(QIODevice::ReadWrite);


//    connect(output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
}

Ms::Score *Player::getScore() const
{
    return score;
}

void Player::setScore(Ms::Score *value)
{
    score = value;

    QMetaObject::invokeMethod(seq, "setScore" , Q_ARG(Score*, value));

//    seq->setScore(score);
}

void Player::play(int tick)
{
//        seq->seek(tick);

    score->updateRepeatList(true);
//        score->update();
//        score->setPlayPos(tick);
//        seq->seekRT(score->repeatList()->tick2utick(tick));
//         seq->seek(tick);

    //    score->end();
    //    Ms::synti->setGain(1.0);

        seq->start();

    //    qreal utick = score->repeatList()->tick2utick(tick);

    //    qreal utime = score->utick2utime(utick);

    //    qDebug() << "Utime" << utime;

    //    m_player.setPosition(utime*10);




//    playrt();


    //    if(scoreChanged)
    //        exportWav();
    //    scoreChanged=false;

    //    m_player.setPosition(tick);

    //    m_player.play();
    //    m_timer.start(100);

}

void Player::stop()
{
        seq->stop();
//    m_player.pause();
//    m_timer.stop();
}

void Player::re_export()
{
    scoreChanged = true;
}

void Player::positionChanged()
{
    qint64 pos = m_player.position();

    qDebug() << "Position" << pos ;

    //    Ms::Measure *measure = score->tick2measure(pos);

    //    if(measure!=0){
    //        heartBeat(measure->tick() , (qreal)measure->tick()/10.0 );
    heartBeat(pos , pos/(qreal)10.0 );
    //    }

}

void Player::setPlaybackSpeed(qreal speed)
{
    m_player.setPlaybackRate(speed);
}

void Player::mediaPlayerStateChanged(QMediaPlayer::State newState)
{
    emit playChanged(newState == QMediaPlayer::PlayingState);
    if(newState != QMediaPlayer::PlayingState){
        m_timer.stop();
    }
}

void Player::handleStateChanged(QAudio::State state)
{

    qDebug() << "State Changed!" << state;

    switch (state) {
    case QAudio::IdleState:
        // Finished playing (no more data)
        output->stop();
        break;

    case QAudio::StoppedState:
        // Stopped for other reasons
        if (output->error() != QAudio::NoError) {
            // Error handling
            qDebug() << "Output Error! " << output->error();
        }
        break;

    default:
        // ... other cases as appropriate
        break;
    }
}

bool Player::playrt()
{

    m_buffer.setBuffer(&audioBuffer);
    m_buffer.open(QIODevice::ReadWrite);


    Ms::EventMap events;
    score->renderMidi(&events);
    if(events.size() == 0)
        return false;

    int sampleRate = Ms::preferences.exportAudioSampleRate;
    synti->setSampleRate(sampleRate);

    int oldSampleRate  = Ms::MScore::sampleRate;
    Ms::MScore::sampleRate = sampleRate;

    float peak  = 0.0;
    double gain = 1.0;
    Ms::EventMap::const_iterator endPos = events.cend();
    --endPos;
    const int et = (score->utick2utime(endPos->first) + 1) * Ms::MScore::sampleRate;
    const int maxEndTime = (score->utick2utime(endPos->first) + 3) * Ms::MScore::sampleRate;

    bool cancelled = false;
    //    for (int pass = 0; pass < 2; ++pass) {

    QElapsedTimer timer1;
    timer1.start();

    Ms::EventMap::const_iterator playPos;
    playPos = events.cbegin();
    synti->allSoundsOff(-1);

    //
    // init instruments
    //
    foreach(Part* part, score->parts()) {
        const InstrumentList* il = part->instruments();
        for(auto i = il->begin(); i!= il->end(); i++) {
            foreach(const Channel* a, i->second->channel()) {
                a->updateInitList();
                foreach(MidiCoreEvent e, a->init) {
                    if (e.type() == ME_INVALID)
                        continue;
                    e.setChannel(a->channel);
                    int syntiIdx= synti->index(score->midiMapping(a->channel)->articulation->synti);
                    synti->play(e, syntiIdx);
                }
            }
        }
    }

    static const unsigned FRAMES = 512;
    float buffer[FRAMES * 2];
    int playTime = 0;



    int i=0;


    for (;;) {
        i++;
        unsigned frames = FRAMES;
        //
        // collect events for one segment
        //
        float max = 0.0;
        memset(buffer, 0, sizeof(float) * FRAMES * 2);
        int endTime = playTime + frames;
        float* p = buffer;

        for (; playPos != events.cend(); ++playPos) {
            int f = score->utick2utime(playPos->first) * MScore::sampleRate;
            if (f >= endTime)
                break;
            int n = f - playTime;
            if (n) {
                synti->process(n, p);
                p += 2 * n;
            }

            playTime  += n;
            frames    -= n;
            const NPlayEvent& e = playPos->second;
            if (e.isChannelEvent()) {
                int channelIdx = e.channel();
                Channel* c = score->midiMapping(channelIdx)->articulation;
                if (!c->mute) {
                    synti->play(e, synti->index(c->synti));
                }
            }
        }

        if (frames) {
            synti->process(frames, p);
            playTime += frames;
        }


        //         qToLittleEndian<qint16>(value, ptr); ///How to. ptr is unsigned char*;


        for(unsigned int i=0 ;i < FRAMES * 2; i+=2 ){

//            qDebug() << "Float Data" << buffer[i];
//            m_buffer.write(reinterpret_cast<const char*>(&buffer[i]) , sizeof(buffer[i]));
            audioBuffer.append(reinterpret_cast<const char*>(&buffer[i]) , sizeof(buffer[i]));
        }

        if(audioBuffer.size() == FRAMES*sizeof(float)){
            output->start(&m_buffer);
        }




//        m_buffer.write(reinterpret_cast<const char*>(qToLittleEndian(buffer)), 2 * FRAMES * sizeof(float));



        //        for (unsigned i = 0; i < FRAMES * 2; ++i) {
        //            max = qMax(max, qAbs(buffer[i]));
        //            peak = qMax(peak, qAbs(buffer[i]));
        //        }


        qDebug() << "pos p" << p;
        qDebug() << "Play Time" << playTime;

        playTime = endTime;

        //Update progress goes here

//         usleep(10000);

        if (playTime >= et)
            synti->allNotesOff(-1);
        // create sound until the sound decays
        if (playTime >= et && max*peak < 0.000001)
            break;
        // hard limit
        if (playTime > maxEndTime)
            break;
    }


    qDebug() << "export time " << timer1.elapsed();
    qDebug() << "Export loops" << i;
    qDebug() << "Buffer size" << audioBuffer.size();



//    output->start(&m_buffer);

    MScore::sampleRate = oldSampleRate;
    delete synti;


    return !cancelled;
}

void Player::exportWav()
{

    score->update();
    QDir dir(QDir::tempPath());
    dir.mkdir("muse");

    QFile f(QDir::tempPath()+"/muse/"+score->name()+".wav");
    if(f.exists()){
        m_player.setMedia(NULL);

        f.remove();
    }

    if(f.open(QFile::ReadWrite)){
        qDebug() << "File created";
        f.close();
    }

    Ms::mscore->saveAs(score , true , QDir::tempPath()+"/muse/"+score->name() , "wav");
    m_player.setMedia(QUrl::fromLocalFile( QDir::tempPath()+"/muse/"+score->name()+".wav"));
}
