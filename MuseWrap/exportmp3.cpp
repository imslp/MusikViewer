//Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>
//=============================================================================
//  MuseScore
//  Linux Music Score Editor
//  $Id: MP3Exporter.cpp 2992 2010-04-22 14:42:39Z lasconic $
//
//  Copyright (C) 2011 Werner Schweer and others
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
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================

#include "libmscore/score.h"
#include "synthesizer/msynthesizer.h"
#include "libmscore/note.h"
#include "musescore.h"
#include "libmscore/part.h"
#include "preferences.h"
#include "exportmp3.h"
#include "lame/lame.h"


namespace Ms {

//---------------------------------------------------------
//   MP3Exporter
//---------------------------------------------------------

MP3Exporter::MP3Exporter()
{
    mLibraryLoaded = true;
    mEncoding = false;
    mGF = NULL;

    QSettings settings;
    mLibPath = settings.value("/Export/lameMP3LibPath", "").toString();

    mBitrate = 128;
    mQuality = QUALITY_2;
    mChannel = CHANNEL_JOINT;
    mMode = MODE_CBR;
    mRoutine = ROUTINE_FAST;
}

MP3Exporter::~MP3Exporter()
{
    freeLibrary();
}

//---------------------------------------------------------
//   findLibrary
//---------------------------------------------------------

bool MP3Exporter::findLibrary()
{
    return true;
}

//---------------------------------------------------------
//   loadLibrary
//---------------------------------------------------------

bool MP3Exporter::loadLibrary(AskUser askuser)
{

    return initLibrary("");

}

bool MP3Exporter::validLibraryLoaded()
{
    return true; //mLibraryLoaded;
}

void MP3Exporter::setMode(int mode)
{
    mMode = mode;
}

void MP3Exporter::setBitrate(int rate)
{
    mBitrate = rate;
}

void MP3Exporter::setQuality(int q, int r)
{
    mQuality = q;
    mRoutine = r;
}

void MP3Exporter::setChannel(int mode)
{
    mChannel = mode;
}

//---------------------------------------------------------
//   initLibrary
//---------------------------------------------------------

bool MP3Exporter::initLibrary(QString libpath)
{
    mGF = lame_init();
    if (mGF == NULL) {
        return false;
    }

    return true;
}

//---------------------------------------------------------
//   freeLibrary
//---------------------------------------------------------

void MP3Exporter::freeLibrary()
{
    if (mGF) {
        lame_close(mGF);
        mGF = NULL;
//        lame_lib->unload();
//        delete lame_lib;
    }
    return;
}

//---------------------------------------------------------
//   getLibraryVersion
//---------------------------------------------------------

QString MP3Exporter::getLibraryVersion()
{
    if (!mLibraryLoaded)
        return QString("");
    return QString("LAME %s").arg(get_lame_version());
}

//---------------------------------------------------------
//   initializeStream
//---------------------------------------------------------

int MP3Exporter::initializeStream(int channels, int sampleRate)
{

    if (channels > 2)
        return -1;

    lame_set_error_protection(mGF, false);
    lame_set_num_channels(mGF, channels);
    lame_set_in_samplerate(mGF, sampleRate);
    lame_set_out_samplerate(mGF, sampleRate);
    lame_set_disable_reservoir(mGF, true);
    lame_set_padding_type(mGF, PAD_NO);

    // Add the VbrTag for all types.  For ABR/VBR, a Xing tag will be created.
    // For CBR, it will be a Lame Info tag.
    lame_set_bWriteVbrTag(mGF, true);

    // Set the VBR quality or ABR/CBR bitrate
    switch (mMode) {
    case MODE_SET:
    {
        int preset;

        if (mQuality == PRESET_INSANE)
            preset = INSANE;
        else if (mRoutine == ROUTINE_FAST) {
            if (mQuality == PRESET_EXTREME)
                preset = EXTREME_FAST;
            else if (mQuality == PRESET_STANDARD)
                preset = STANDARD_FAST;
            else
                preset = 1007;    // Not defined until 3.96
        }
        else {
            if (mQuality == PRESET_EXTREME)
                preset = EXTREME;
            else if (mQuality == PRESET_STANDARD)
                preset = STANDARD;
            else
                preset = 1006;    // Not defined until 3.96
        }
        lame_set_preset(mGF, preset);
    }
        break;

    case MODE_VBR:
        lame_set_VBR(mGF, (mRoutine == ROUTINE_STANDARD ? vbr_rh : vbr_mtrh ));
        lame_set_VBR_q(mGF, mQuality);
        break;

    case MODE_ABR:
        lame_set_preset(mGF, mBitrate );
        break;

    default:
        lame_set_VBR(mGF, vbr_off);
        lame_set_brate(mGF, mBitrate);
        break;
    }

    // Set the channel mode
    MPEG_mode mode;
    if (channels == 1)
        mode = MONO;
    else if (mChannel == CHANNEL_JOINT)
        mode = JOINT_STEREO;
    else
        mode = STEREO;
    lame_set_mode(mGF, mode);

    int rc = lame_init_params(mGF);
    if (rc < 0)
        return rc;

#if 0
    dump_config(mGF);
#endif

    mInfoTagLen = 0;
    mEncoding = true;

    return mSamplesPerChunk;
}

//---------------------------------------------------------
//   getOutBufferSize
//---------------------------------------------------------

int MP3Exporter::getOutBufferSize()
{
    if (!mEncoding)
        return -1;

    return mOutBufferSize;
}

//---------------------------------------------------------
//   bufferPreamp
//---------------------------------------------------------

void MP3Exporter::bufferPreamp(float buffer[], int nSamples)
{
    for (int i = 0; i < nSamples; i++)
        buffer[i] = buffer[i] * 32768;
}

//---------------------------------------------------------
//   encodeBuffer
//---------------------------------------------------------

int MP3Exporter::encodeBuffer(float inbufferL[], float inbufferR[], unsigned char outbuffer[])
{
    if (!mEncoding)
        return -1;

    bufferPreamp(inbufferL, mSamplesPerChunk);
    bufferPreamp(inbufferR, mSamplesPerChunk);
    return lame_encode_buffer_float(mGF, inbufferL, inbufferR, mSamplesPerChunk,
                                    outbuffer, mOutBufferSize);
}

//---------------------------------------------------------
//   encodeRemainder
//---------------------------------------------------------

int MP3Exporter::encodeRemainder(float inbufferL[], float inbufferR[], int nSamples,
                                 unsigned char outbuffer[])
{
    if (!mEncoding)
        return -1;

    bufferPreamp(inbufferL, nSamples);
    bufferPreamp(inbufferR, nSamples);
    return lame_encode_buffer_float(mGF, inbufferL, inbufferR, nSamples, outbuffer,
                                    mOutBufferSize);
}

//---------------------------------------------------------
//   encodeBufferMono
//---------------------------------------------------------

int MP3Exporter::encodeBufferMono(float inbuffer[], unsigned char outbuffer[])
{
    if (!mEncoding)
        return -1;

    bufferPreamp(inbuffer, mSamplesPerChunk);
    return lame_encode_buffer_float(mGF, inbuffer, inbuffer, mSamplesPerChunk,
                                    outbuffer, mOutBufferSize);
}

//---------------------------------------------------------
//   encodeRemainderMono
//---------------------------------------------------------

int MP3Exporter::encodeRemainderMono(float inbuffer[], int nSamples,
                                     unsigned char outbuffer[])
{
    if (!mEncoding)
        return -1;

    bufferPreamp(inbuffer, nSamples);
    return lame_encode_buffer_float(mGF, inbuffer, inbuffer, nSamples, outbuffer,
                                    mOutBufferSize);
}

//---------------------------------------------------------
//   finishStream
//---------------------------------------------------------

int MP3Exporter::finishStream(unsigned char outbuffer[])
{
    if (!mEncoding)
        return -1;

    mEncoding  = false;
    int result = lame_encode_flush(mGF, outbuffer, mOutBufferSize);

    if (lame_get_lametag_frame)
        mInfoTagLen = lame_get_lametag_frame(mGF, mInfoTagBuf, sizeof(mInfoTagBuf));
    return result;
}

//---------------------------------------------------------
//   cancelEncoding
//---------------------------------------------------------

void MP3Exporter::cancelEncoding()
{
    mEncoding = false;
}

/*void MP3Exporter::PutInfoTag(QFile f, qint64 off)
{
  QDataStream out(&f);
   if (mGF) {
      if (mInfoTagLen > 0) {
         out.skipRawData (off);
         out.writeRawData(mInfoTagBuf, mInfoTagLen);
      }
#if defined(Q_OS_WIN)
      else if (beWriteInfoTag) {
         f.flush();
         QFileInfo fi(f);
         beWriteInfoTag(mGF, qPrintable(fi.completeBaseName()));
         mGF = NULL;
      }
#endif
      else if (lame_mp3_tags_fid) {
      std::FILE *fp;
      if ((fp = std::fdopen(file.handle(), "w+")) != NULL)
         lame_mp3_tags_fid(mGF, fp);
      }
   }

   f.seek(f.size());
}*/

#if defined(Q_OS_WIN)
/* values for Windows */

QString MP3Exporter::getLibraryPath()
{
    QSettings settings("HKEY_LOCAL_MACHINE\\Software\\Lame for Audacity", QSettings::NativeFormat);
    QString sReturnedValue = settings.value( "InstallPath", "" ).toString();
    if (! sReturnedValue.isEmpty()) {
        return sReturnedValue;
    }
    return QDir::rootPath();
}

QString MP3Exporter::getLibraryName()
{
    return QString("lame_enc.dll");
}

QString MP3Exporter::getLibraryTypeString()
{
    return QString("Only lame_enc.dll (lame_enc.dll);;Dynamically Linked Libraries (*.dll);;All Files (*.*)");
}

#elif defined(Q_OS_MAC)
/* values for Mac OS X */

QString MP3Exporter::getLibraryPath()
{
    return QString("/usr/local/lib/audacity");
}

QString MP3Exporter::getLibraryName()
{
    return QString("libmp3lame.dylib");
}

QString MP3Exporter::getLibraryTypeString()
{
    return QString("Only libmp3lame.dylib (libmp3lame.dylib);;Dynamic Libraries (*.dylib);;All Files (*)");
}

#else //!Q_OS_MAC
/* Values for Linux / Unix systems */

QString MP3Exporter::getLibraryPath()
{
    return QString("/usr/lib");
}

QString MP3Exporter::getLibraryName()
{
    return QString("libmp3lame.so.0");
}

QString MP3Exporter::getLibraryTypeString()
{
    return QString("Only libmp3lame.so.0 (libmp3lame.so.0);;Primary Shared Object files (*.so);;Extended Libraries (*.so*);;All Files (*)");
}
#endif //mac

}

