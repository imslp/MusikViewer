//Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>
//=============================================================================
//  MuseScore
//  Music Composition & Notation
//  $Id: musescore.h 5657 2012-05-21 15:46:06Z lasconic $
//
//  Copyright (C) 2002-2011 Werner Schweer and others
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

#ifndef __MUSESCORE_H__
#define __MUSESCORE_H__

#include "config.h"
#include "globals.h"
#include "libmscore/musescoreCore.h"
#include <QSettings>
#include <QTimer>
#include <QNetworkAccessManager>
#include "driver.h"
#include "synthesizer/msynthesizer.h"
#include <libmscore/element.h>
#include <fluid/fluid.h>

#include <libmscore/score.h>



namespace Ms {
class MuseScore : public QObject , public MuseScoreCore {
      Q_OBJECT

public slots:
    Score *openScore(const QString &fn);
    Score *readScore(const QString &name);
    bool saveMp3(Score *score, const QString &name);
    bool saveMp3(Score *score, int fd);
    bool saveFile();
    bool saveFile(Score *score);
    QString createDefaultName() const;
    void newFile();
    QStringList getOpenScoreNames(const QString &filter, const QString &title);
    QString getSaveScoreName(const QString &title, const QString &name, const QString &filter, bool selectFolder = false);
    void saveScoreDialogFilterSelected(const QString &s);
    QString getStyleFilename(bool open, const QString &title);
    QString getChordStyleFilename(bool open);
    QString getScanFile(const QString &d);
    QString getAudioFile(const QString &d);
    QString getFotoFilename(QString &filter, QString *selectedFilter);
    QString getDrumsetFilename(bool open);
    void printFile();
    void exportFile();
    bool exportParts();
    bool saveAs(Score *cs, bool saveCopy, const QString &path, const QString &ext);
    bool saveAs(Score *cs, bool saveCopy, int fd, const QString &ext , int currpage);
    bool saveMidi(Score *score, const QString &name);
    bool saveMidi(Score *score, int fd);
    bool savePdf(const QString &saveName);
    bool savePdf(const int fd);
    bool savePdf(Score *score, const QString &saveName);
    bool savePdf(Score *score,int fd);
    bool savePdf(QList<Score *> cs, const QString &saveName);
    bool saveAs(Score *cs, bool saveCopy);
    bool saveSelection(Score *cs);
    void addImage(Score *score, Element *e);
    bool savePng(Score *score, const QString &name);
    bool savePng(Score *score, int fd , int currpage);
    bool savePng(Score *score, const QString &name, bool screenshot, bool transparent, double convDpi, int trimMargin, QImage::Format format);
    bool savePng(Score *score, int fd, bool screenshot, bool transparent, double convDpi, int trimMargin, QImage::Format format , int _page);
    bool saveSvg(Score *score, const QString &saveName);
    QPixmap extractThumbnail(const QString &name);
    bool saveAudio(Score *score, QIODevice *device, std::function<bool (float)> updateProgress);
    bool saveAudio(Score *score, const QString &name);
    bool saveAudio(Score *score, int fd , QString ext);
    bool checkDirty(Score *s);
    void loadFiles();
    bool loop() const              { return m_loop; }
    SynthesizerState synthesizerState();
    bool metronome() const         { return m_metronome; }
    bool countIn() const           { return m_countIn; }
    bool panDuringPlayback() const { return m_pan; }
    ScoreState state() const { return _sstate; }
    void cancelExport();


    void seqStarted();
    void seqStopped();
public:
    MuseScore (QObject *parent =0);
    QString lastSaveDirectory;
    QString lastSaveCopyDirectory;
    QString lastSaveCopyFormat;
    bool m_loop = false ;
    bool m_metronome = false;
    bool m_countIn = false;
    bool m_pan = false;
    ScoreState _sstate;


signals:
    void errorSignal(QString message);
    void saveCompleted();
    void exportProgress(float progress);

private:
    bool m_exportCancelled = false;
};

extern MuseScore* mscore;
extern QStringList recentScores;
extern QString dataPath;
extern MasterSynthesizer* synti;
MasterSynthesizer* synthesizerFactory();
Driver* driverFactory(Seq*, QString driver);



//extern void loadTranslation(QString fileName, QString localeName);
//extern void setMscoreLocale(QString localeName);
extern bool saveMxl(Score*, const QString& name);
extern bool saveMxl(Score*, int fd);
extern bool saveXml(Score*, const QString& name);
extern bool saveXml(Score*, int fd);


//struct PluginDescription;
//extern void collectPluginMetaInformation(PluginDescription*);
//extern QString getSharePath();
} // namespace Ms

Q_DECLARE_METATYPE(Ms::Score*)

#endif
