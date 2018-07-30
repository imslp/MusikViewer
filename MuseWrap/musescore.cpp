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


#include "musescore.h"
#include <effects/effect.h>
#include <zerberus/zerberus.h>
#include <effects/zita1/zita.h>
#include <effects/compressor/compressor.h>
#include <effects/noeffect/noeffect.h>
#include <libmscore/score.h>
#include "exportmp3.h"
#include "preferences.h"
#include <libmscore/instrument.h>
#include <libmscore/part.h>
#include "preferences.h"

namespace Ms {

MasterSynthesizer* synthesizerFactory()
{
    if(!Ms::preferences_init){
        Ms::preferences.init();
        Ms::preferences_init = true;
    }


    MasterSynthesizer* ms = new MasterSynthesizer();

    FluidS::Fluid* fluid = new FluidS::Fluid();
    ms->registerSynthesizer(fluid);

#ifdef AEOLUS
    ms->registerSynthesizer(::createAeolus());
#endif
#ifdef ZERBERUS
    ms->registerSynthesizer(new Zerberus);
#endif
    ms->registerEffect(0, new NoEffect);
    ms->registerEffect(0, new ZitaReverb);
    ms->registerEffect(0, new Compressor);
    // ms->registerEffect(0, new Freeverb);
    ms->registerEffect(1, new NoEffect);
    ms->registerEffect(1, new ZitaReverb);
    ms->registerEffect(1, new Compressor);
    // ms->registerEffect(1, new Freeverb);
    ms->setEffect(0, 1);
    ms->setEffect(1, 0);
    return ms;
}

SynthesizerState MuseScore::synthesizerState()
{
    SynthesizerState state;
    return synti ? synti->state() : state;
}

void MuseScore::cancelExport()
{
    m_exportCancelled = true;
}

MuseScore::MuseScore(QObject *parent):QObject(parent){



    qRegisterMetaType<Score*>("Score*");
}

bool MuseScore::saveMp3(Score* score, const QString& name)
{
#ifndef USE_LAME
    return false;
#else
    EventMap events;
    score->renderMidi(&events);
    if(events.size() == 0)
        return false;

    MP3Exporter exporter;
    if (!exporter.loadLibrary(MP3Exporter::AskUser::MAYBE)) {
        QSettings settings;
        settings.setValue("/Export/lameMP3LibPath", "");
        qDebug("Could not open MP3 encoding library!");
        emit errorSignal("Could not open MP3 encoding library!");
        return false;
    }

    if (!exporter.validLibraryLoaded()) {
        QSettings settings;
        settings.setValue("/Export/lameMP3LibPath", "");
        qDebug("Not a valid or supported MP3 encoding library!");
        emit errorSignal("Not a valid or supported MP3 encoding library!");
        return false;
    }

    // Retrieve preferences
    //      int highrate = 48000;
    //      int lowrate = 8000;
    //      int bitrate = 64;
    //      int brate = 128;
    //      int rmode = MODE_CBR;
    //      int vmode = ROUTINE_FAST;
    //      int cmode = CHANNEL_STEREO;

    int channels = 2;

    int oldSampleRate = MScore::sampleRate;
    int sampleRate = preferences.exportAudioSampleRate;
    exporter.setBitrate(preferences.exportMp3BitRate);

    int inSamples = exporter.initializeStream(channels, sampleRate);
    if (inSamples < 0) {
        qDebug("Unable to initialize MP3 stream");
        emit errorSignal("Unable to initialize MP3 stream");
        MScore::sampleRate = oldSampleRate;
        return false;
    }

    QFile file(name);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Unable to open target file for writing";
        emit errorSignal("Unable to open target file for writing");
        MScore::sampleRate = oldSampleRate;
        return false;
    }

    int bufferSize   = exporter.getOutBufferSize();
    uchar* bufferOut = new uchar[bufferSize];
    MasterSynthesizer* synti = synthesizerFactory();
    synti->init();
    synti->setSampleRate(sampleRate);
    bool r = synti->setState(score->synthesizerState());
    if (!r)
        synti->init();


    MScore::sampleRate = sampleRate;

    //    QProgressDialog progress(this);
    //    progress.setWindowFlags(Qt::WindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint));
    //    progress.setWindowModality(Qt::ApplicationModal);
    //    //progress.setCancelButton(0);
    //    progress.setCancelButtonText(tr("Cancel"));
    //    progress.setLabelText(tr("Exporting..."));
    //    if (!MScore::noGui)
    //        progress.show();

    static const int FRAMES = 512;
    float bufferL[FRAMES];
    float bufferR[FRAMES];

    float  peak = 0.0;
    double gain = 1.0;
    EventMap::const_iterator endPos = events.cend();
    --endPos;
    const int et = (score->utick2utime(endPos->first) + 1) * MScore::sampleRate;
    const int maxEndTime = (score->utick2utime(endPos->first) + 3) * MScore::sampleRate;
    //progress.setRange(0, et);

    for (int pass = 0; pass < 2; ++pass) {
        EventMap::const_iterator playPos;
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

        int playTime = 0.0;

        for (;;) {
            unsigned frames = FRAMES;
            float max = 0;
            //
            // collect events for one segment
            //
            memset(bufferL, 0, sizeof(float) * FRAMES);
            memset(bufferR, 0, sizeof(float) * FRAMES);
            double endTime = playTime + frames;

            float* l = bufferL;
            float* r = bufferR;

            for (; playPos != events.cend(); ++playPos) {
                double f = score->utick2utime(playPos->first) * MScore::sampleRate;
                if (f >= endTime)
                    break;
                int n = f - playTime;
                if (n) {
                    float bu[n * 2];
                    memset(bu, 0, sizeof(float) * 2 * n);

                    synti->process(n, bu);
                    float* sp = bu;
                    for (int i = 0; i < n; ++i) {
                        *l++ = *sp++;
                        *r++ = *sp++;
                    }
                    playTime  += n;
                    frames    -= n;
                }
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
                float bu[frames * 2];
                memset(bu, 0, sizeof(float) * 2 * frames);
                synti->process(frames, bu);
                float* sp = bu;
                for (unsigned i = 0; i < frames; ++i) {
                    *l++ = *sp++;
                    *r++ = *sp++;
                }
                playTime += frames;
            }

            if (pass == 1) {
                for (int i = 0; i < FRAMES; ++i) {
                    max = qMax(max, qAbs(bufferL[i]));
                    max = qMax(max, qAbs(bufferR[i]));
                    bufferL[i] *= gain;
                    bufferR[i] *= gain;
                }
                long bytes;
                if (FRAMES < inSamples)
                    bytes = exporter.encodeRemainder(bufferL, bufferR,  FRAMES , bufferOut);
                else
                    bytes = exporter.encodeBuffer(bufferL, bufferR, bufferOut);
                if (bytes < 0) {
                    qDebug("exportmp3: error from encoder: %ld", bytes);
                    QString err = QString("exportmp3: error from encoder: %1").arg(bytes);
                    emit errorSignal(err);
                    break;
                }
                else
                    file.write((char*)bufferOut, bytes);
            }
            else {
                for (int i = 0; i < FRAMES; ++i) {
                    max = qMax(max, qAbs(bufferL[i]));
                    max = qMax(max, qAbs(bufferR[i]));
                    peak = qMax(peak, qAbs(bufferL[i]));
                    peak = qMax(peak, qAbs(bufferR[i]));
                }
            }
            playTime = endTime;
            if (playTime >= et)
                synti->allNotesOff(-1);
            // create sound until the sound decays
            if (playTime >= et && max * peak < 0.000001)
                break;
            // hard limit
            if (playTime > maxEndTime)
                break;
        }
        //        if (progress.wasCanceled())
        //            break;
        if (pass == 0 && peak == 0.0) {
            qDebug("song is empty");
            break;
        }
        gain = 0.99 / peak;
    }

    long bytes = exporter.finishStream(bufferOut);
    if (bytes > 0L)
        file.write((char*)bufferOut, bytes);

    bool wasCanceled = false; //= progress.wasCanceled();
    delete synti;
    delete[] bufferOut;
    file.close();
    if (wasCanceled)
        file.remove();
    MScore::sampleRate = oldSampleRate;
    return true;



#endif
}


bool MuseScore::saveMp3(Score* score, int fd)
{
#ifndef USE_LAME
    return false;
#else
    EventMap events;
    score->renderMidi(&events);
    if(events.size() == 0)
        return false;

    MP3Exporter exporter;
    if (!exporter.loadLibrary(MP3Exporter::AskUser::MAYBE)) {
        QSettings settings;
        settings.setValue("/Export/lameMP3LibPath", "");
        qDebug("Could not open MP3 encoding library!");
        emit errorSignal("Could not open MP3 encoding library!");
        return false;
    }

    if (!exporter.validLibraryLoaded()) {
        QSettings settings;
        settings.setValue("/Export/lameMP3LibPath", "");
        qDebug("Not a valid or supported MP3 encoding library!");
        emit errorSignal("Not a valid or supported MP3 encoding library!");
        return false;
    }

    // Retrieve preferences
    //      int highrate = 48000;
    //      int lowrate = 8000;
    //      int bitrate = 64;
    //      int brate = 128;
    //      int rmode = MODE_CBR;
    //      int vmode = ROUTINE_FAST;
    //      int cmode = CHANNEL_STEREO;

    int channels = 2;

    int oldSampleRate = MScore::sampleRate;
    int sampleRate = preferences.exportAudioSampleRate;
    exporter.setBitrate(preferences.exportMp3BitRate);

    int inSamples = exporter.initializeStream(channels, sampleRate);
    if (inSamples < 0) {
        qDebug("Unable to initialize MP3 stream");
        emit errorSignal("Unable to initialize MP3 stream");
        MScore::sampleRate = oldSampleRate;
        return false;
    }

    QFile file;
    if (!file.open( fd ,QIODevice::WriteOnly)) {
        qDebug() << "Unable to open target file for writing";
        emit errorSignal("Unable to open target file for writing");
        MScore::sampleRate = oldSampleRate;
        return false;
    }

    int bufferSize   = exporter.getOutBufferSize();
    uchar* bufferOut = new uchar[bufferSize];
    MasterSynthesizer* synti = synthesizerFactory();
    synti->init();
    synti->setSampleRate(sampleRate);
    bool r = synti->setState(score->synthesizerState());
    if (!r)
        synti->init();


    MScore::sampleRate = sampleRate;

    static const int FRAMES = 512;
    float bufferL[FRAMES];
    float bufferR[FRAMES];

    float  peak = 0.0;
    double gain = 1.0;
    EventMap::const_iterator endPos = events.cend();
    --endPos;
    const int et = (score->utick2utime(endPos->first) + 1) * MScore::sampleRate;
    const int maxEndTime = (score->utick2utime(endPos->first) + 3) * MScore::sampleRate;
    //progress.setRange(0, et);

    for (int pass = 0; pass < 2; ++pass) {
        EventMap::const_iterator playPos;
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

        int playTime = 0.0;

        for (;;) {
            unsigned frames = FRAMES;
            float max = 0;
            //
            // collect events for one segment
            //
            memset(bufferL, 0, sizeof(float) * FRAMES);
            memset(bufferR, 0, sizeof(float) * FRAMES);
            double endTime = playTime + frames;

            float* l = bufferL;
            float* r = bufferR;

            for (; playPos != events.cend(); ++playPos) {
                double f = score->utick2utime(playPos->first) * MScore::sampleRate;
                if (f >= endTime)
                    break;
                int n = f - playTime;
                if (n) {
                    float bu[n * 2];
                    memset(bu, 0, sizeof(float) * 2 * n);

                    synti->process(n, bu);
                    float* sp = bu;
                    for (int i = 0; i < n; ++i) {
                        *l++ = *sp++;
                        *r++ = *sp++;
                    }
                    playTime  += n;
                    frames    -= n;
                }
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
                float bu[frames * 2];
                memset(bu, 0, sizeof(float) * 2 * frames);
                synti->process(frames, bu);
                float* sp = bu;
                for (unsigned i = 0; i < frames; ++i) {
                    *l++ = *sp++;
                    *r++ = *sp++;
                }
                playTime += frames;
            }

            if (pass == 1) {
                for (int i = 0; i < FRAMES; ++i) {
                    max = qMax(max, qAbs(bufferL[i]));
                    max = qMax(max, qAbs(bufferR[i]));
                    bufferL[i] *= gain;
                    bufferR[i] *= gain;
                }
                long bytes;
                if (FRAMES < inSamples)
                    bytes = exporter.encodeRemainder(bufferL, bufferR,  FRAMES , bufferOut);
                else
                    bytes = exporter.encodeBuffer(bufferL, bufferR, bufferOut);
                if (bytes < 0) {
                    qDebug("exportmp3: error from encoder: %ld", bytes);
                    QString err = QString("exportmp3: error from encoder: %1").arg(bytes);
                    emit errorSignal(err);
                    break;
                }
                else
                    file.write((char*)bufferOut, bytes);
            }
            else {
                for (int i = 0; i < FRAMES; ++i) {
                    max = qMax(max, qAbs(bufferL[i]));
                    max = qMax(max, qAbs(bufferR[i]));
                    peak = qMax(peak, qAbs(bufferL[i]));
                    peak = qMax(peak, qAbs(bufferR[i]));
                }
            }
            playTime = endTime;
            emit exportProgress((pass * et + playTime) / 2.0 / et);

            if(m_exportCancelled)
                break;

            if (playTime >= et)
                synti->allNotesOff(-1);
            // create sound until the sound decays
            if (playTime >= et && max * peak < 0.000001)
                break;
            // hard limit
            if (playTime > maxEndTime)
                break;
        }
        if(m_exportCancelled)
            break;
        if (pass == 0 && peak == 0.0) {
            qDebug("song is empty");
            break;
        }
        gain = 0.99 / peak;
    }

    long bytes = exporter.finishStream(bufferOut);
    if (bytes > 0L)
        file.write((char*)bufferOut, bytes);

//    bool wasCanceled = false; //= progress.wasCanceled();
    delete synti;
    delete[] bufferOut;
    file.close();
    if (m_exportCancelled){
        file.remove();
        m_exportCancelled = false;
    }
    MScore::sampleRate = oldSampleRate;
    return true;



#endif
}




}
