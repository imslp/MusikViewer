//Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>
//=============================================================================
//  MuseScore
//  Linux Music Score Editor
//  $Id: musedata.h 1840 2009-05-20 11:57:51Z wschweer $
//
//  Copyright (C) 2002-2009 Werner Schweer and others
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

#ifndef __MUSEDATA_H__
#define __MUSEDATA_H__

#include <stdio.h>
#include <limits.h>
#include <map>
#include <set>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <functional>

#include <QtGui>
#include <QModelIndex>

#include <QtXml>
#include <QAbstractMessageHandler>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
// #include <QXmlStreamReader>

#include <QPointF>
#include <QVariant>
#include <QMap>
#include <QByteArray>
#include <QDateTime>
#include <QtGlobal>
#include <QtDebug>
#include <QSharedData>

#include <QAtomicInt>
#include <QErrorMessage>

#include <QPainterPath>
#include <QPixmap>
#include <QPainter>
#include <QKeyEvent>

#include <QFontDatabase>
#include <QProcess>
#include <QDesktopServices>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>
#include <QTextBlock>
#include <QTextList>
#include <QClipboard>
#include <QPlainTextEdit>
#include <QStyledItemDelegate>

#include <QDateTimeEdit>
#include <QInputDialog>
#include <QFormLayout>
#include <QItemDelegate>
#include <QStandardItemModel>
#include <QSpinBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QToolBar>
#include <QTreeWidget>
#include <QFileDialog>
#include <QPrintDialog>
#include <QColorDialog>
#include <QDockWidget>
#include <QStackedWidget>
#include <QStackedLayout>
#include <QListWidget>
#include <QMessageBox>
#include <QPrinter>
#include <QComboBox>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
#include <QAction>
#include <QActionGroup>
#include <QLayout>
#include <QBoxLayout>
#include <QStandardItemModel>
#include <QToolTip>
#include <QToolBox>
#include <QToolButton>
#include <QPushButton>
#include <QWizard>
#include <QGroupBox>
#include <QDial>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QProgressBar>
#include <QProgressDialog>
#include <QRadioButton>
#include <QSplashScreen>
#include <QFontComboBox>
#include <QApplication>
#include <QStatusBar>
#include <QStylePainter>
#include <QStyleOptionButton>
#include <QHeaderView>
#include <QUndoGroup>
#include <QUndoStack>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QLabel>
#include <QFocusFrame>
#include <QMouseEventTransition>
#include <QCommonStyle>
#include <QMdiSubWindow>
#include <QSpacerItem>
#include <QGraphicsSceneMouseEvent>
#include <QtConcurrent>
#include <QScreen>
#include <QGestureEvent>

#include <QSvgRenderer>
#include <QSvgGenerator>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkProxyFactory>
#include <QHostAddress>
#include <QUdpSocket>

#include <QHttpPart>
#include <QHttpMultiPart>

#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QQuickView>
//#include <QQuickWidget>

#include <QHelpEngine>
//#include <QWidgetAction>
#include <QHelpIndexModel>
#include <QTextBrowser>

#include <QJsonDocument>

namespace Ms {

class Staff;
class Part;
class Score;
class ChordRest;
class Measure;
class Slur;

//---------------------------------------------------------
//   MuseData
//    used importing Musedata files
//---------------------------------------------------------

class MuseData {
      int _division;
      int curTick;
      QList<QStringList> parts;
      Score* score;
      ChordRest* chordRest;
      int ntuplet;
      Measure* measure;
      int voice;
      Slur* slur[4];

      void musicalAttribute(QString s, Part*);
      void readPart(QStringList sl, Part*);
      void readNote(Part*, const QString& s);
      void readChord(Part*, const QString& s);
      void readRest(Part*, const QString& s);
      void readBackup(const QString& s);
      Measure* createMeasure();
      int countStaves(const QStringList& sl);
      void openSlur(int idx, int tick, Staff* staff, int voice);
      void closeSlur(int idx, int tick, Staff* staff, int voice);
      QString diacritical(QString);

   public:
      MuseData(Score* s) { score = s; }
      bool read(const QString&);
      void convert();
      };


} // namespace Ms
#endif

