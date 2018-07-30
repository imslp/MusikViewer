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



#include "qaudiodriver.h"
#include <QAudio>
#include <QAudioFormat>
#include <QDebug>
#include "preferences.h"
#include "seq.h"



QAudioDriver::QAudioDriver(Ms::Seq *seq) : Driver(seq)
{
    synti = Ms::synthesizerFactory();
    synti->init();

    //    connect(&m_timer , &QTimer::timeout , this , &Player::positionChanged );
    //    connect(&m_player , &QMediaPlayer::stateChanged , this , &Player::mediaPlayerStateChanged );


    format.setChannelCount(1);
    format.setSampleRate(Ms::preferences.exportAudioSampleRate);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleSize(16);

    //    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");


    qDebug() << "Bytes per frame" << format.bytesPerFrame();
    qDebug() << "Sample Rate" << format.sampleRate();



    //    output->moveToThread(&audiothread);
    //    audiothread.start();


    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());

    QList<QAudioDeviceInfo> outdevs = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    qWarning() << "Output devices number" << outdevs.size();

    for(int i=0;i<outdevs.size();i++){
        qWarning() << "Output device name" << outdevs.at(i).deviceName();
    }

    qDebug() << "_____ INFO 1 END ____";


    QList<int> samplerates = info.supportedSampleRates();

    for(int i=0;i<samplerates.size();i++){
        qWarning() << "Output sample Rates" << samplerates.at(i);
    }


    QList<int> chcounts = info.supportedChannelCounts();

    for(int i=0;i<chcounts.size();i++){
        qWarning() << "Output channels" << chcounts.at(i);
    }

    QStringList supportedcodecs =info.supportedCodecs();

    for(int i=0;i<supportedcodecs.size();i++){
        qWarning() << "Output codecs" << supportedcodecs.at(i);
    }

    QList<int> samplesize = info.supportedSampleSizes();

    for(int i=0;i<samplesize.size();i++){
        qWarning() << "Output samplesize" << samplesize.at(i);
    }

    QList<QAudioFormat::SampleType> sampleTypes = info.supportedSampleTypes();

    for(int i=0;i<sampleTypes.size();i++){
        qWarning() << "Output sampleTypes" << sampleTypes.at(i);
    }

    QList<QAudioFormat::Endian> endian = info.supportedByteOrders();

    qDebug() << "Current endian" << format.byteOrder();

    for(int i=0;i<endian.size();i++){
        qWarning() << "Output endian" << endian.at(i);
    }


    if (!info.isFormatSupported(format)) {
        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }

    //    return;


    ///512 seems to be a bit more than 10 msecs
    frames = 4096; //Need to make this more than 10msecs
    buffer = (float*)malloc(frames*2*sizeof(float));
    bufferlength = frames/(format.sampleRate()/1000);


    output = new QAudioOutput(format);
    connect(output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));

    m_buffer.setBuffer(&audioBuffer);
    m_buffer.open(QIODevice::ReadOnly);


    qDebug() << Q_FUNC_INFO << "Constructor finished!";

}

QAudioDriver::~QAudioDriver()
{
    output->stop();
    output->deleteLater();
    audiothread.quit();
    audiothread.wait(1000);
    free(buffer);

}

void QAudioDriver::handleStateChanged(QAudio::State state)
{

    qDebug() << "State Changed!" << state << prevState;


    switch (state) {
    case QAudio::IdleState:
        // Finished playing (no more data)
        //        output->stop();

        //        if(processing)
        //            return;

//        if(m_buffer.atEnd()){
//            output->stop();
//        }
        break;

    case QAudio::StoppedState:
        // Stopped for other reasons

        //        if(this->state==Transport::STOP)
        //            return;

        this->state = Transport::STOP;
        //        output->stop();
        //        seq->stop();
        //        timer.stop();
        //        audioBuffer.clear();

        //        disconnect(output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));

        //        m_buffer.close();
        //        output->reset();
        //        seq->setState(this->state);
        //        seq->process(frames , buffer);
        if (output->error() != QAudio::NoError) {
            // Error handling
            qDebug() << "Output Error! " << output->error();
//            seq->stop();
        }

        //        output->deleteLater();
        //        output=0;
        break;

    case QAudio::ActiveState:

        if(prevState!=QAudio::ActiveState){
            //            timer.start();
            playTime.restart();
            //            QtConcurrent::run(this, &QAudioDriver::onAudioNotify);
        }


        break;
    default:
        // ... other cases as appropriate
        break;
    }

    prevState = state;
}

void QAudioDriver::onAudioNotify()
{
    unsigned int processed;
    qint64 buffered;
    bool first = true;
    while (true) {

        //        if(output==0)
        //            break;

        //        if(output->state()!=QAudio::ActiveState)
        //            break;


//        if(output->state()!=QAudio::ActiveState)
//            first = false;

        processed = m_buffer.pos()/sizeof(qint16)/(format.sampleRate()/1000); //playTime.elapsed(); //in ms
        buffered = (audioBuffer.size()/sizeof(qint16))/(format.sampleRate()/1000); //in ms

        //        qDebug() << "buffer pos" << m_buffer.pos()/sizeof(qint16)/(format.sampleRate()/1000) << "processed" << processed << "buffered" << buffered << "Buffer length" << bufferlength;

        if(processed>=(buffered-(bufferlength*2))){
                        qDebug() << "Calling process we need new data";
            seq->process(frames , buffer);
            for(unsigned int i=0 ;i < frames * 2; i+=2 ){
                qint16 buf = (qint16)(buffer[i]*32767.0f);
                audioBuffer.append(reinterpret_cast<const char*>(&buf) , sizeof(buf));

//                if(first){
//                    qDebug() << "Data" << buf;
//                }
            }
            first = false;
            //        processing = true;
        }else{
            //            qDebug() << "Time before Sleep" << playTime.elapsed();
            usleep(5000);
            //            qDebug() << "Time after Sleep" << playTime.elapsed();
        }
    }


}

void QAudioDriver::processFinished()
{

#if 0
    for(unsigned int i=0 ;i < frames * 2; i+=2 ){
        qint16 buf = (qint16)(buffer[i]*32767.0f);
        audioBuffer.append(reinterpret_cast<const char*>(&buf) , sizeof(buf));
    }

    processing = false;

    qDebug() << Q_FUNC_INFO << "set processing flag to false";
#endif
}


bool QAudioDriver::init(bool hot)
{

    Q_UNUSED(hot);
    audioBuffer.clear();
    return true;

}

bool QAudioDriver::start(bool hotPlug)
{
    Q_UNUSED(hotPlug);
    QtConcurrent::run(this, &QAudioDriver::onAudioNotify);
    return true;
}

bool QAudioDriver::stop()
{

    qDebug() << Q_FUNC_INFO;

    state = Transport::STOP;
//    output->stop();

    return true;
}

void QAudioDriver::stopTransport()
{
    qDebug() << Q_FUNC_INFO;

    state = Transport::STOP;
//    output->stop();

    //    audioBuffer.clear();

    //    m_buffer.seek(0);
//        seq->process(frames , buffer);
}

void QAudioDriver::startTransport()
{
    qDebug() << Q_FUNC_INFO;


//        audioBuffer.clear();
//        output->reset();

    //    state = Transport::PLAY;

//    for(unsigned int i=0 ;i < frames * 2; i+=2 ){
//        qint16 buf = 0;
//        audioBuffer.append(reinterpret_cast<const char*>(&buf) , sizeof(buf));
//    }

if(m_first){

    seq->process(frames , buffer);

    for(unsigned int i=0 ;i < frames * 2; i+=2 ){
        qint16 buf = (qint16)(buffer[i]*32767.0f);
        audioBuffer.append(reinterpret_cast<const char*>(&buf) , sizeof(buf));
//        qDebug() << "Data" << buf;
    }
    m_first=false;
        output->start(&m_buffer);
}

//    m_buffer.seek(0);
//    output->start(&m_buffer);
    state = Transport::PLAY;

    //    seq->process(frames , buffer);

    //    for(unsigned int i=0 ;i < frames * 2; i+=2 ){
    //        qint16 buf = (qint16)(buffer[i]*32767.0f);
    //        audioBuffer.append(reinterpret_cast<const char*>(&buf) , sizeof(buf));
    //    }



    //    output = new QAudioOutput(format);
    //    connect(output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));


    //    m_buffer.setBuffer(NULL);
    //    m_buffer.setBuffer(&audioBuffer);
    //    m_buffer.open(QIODevice::ReadOnly);
    //    m_buffer.seek(0);




}

Transport QAudioDriver::getState()
{
    return state;
}

int QAudioDriver::sampleRate() const
{
    return format.sampleRate();
}
