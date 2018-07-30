//Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>
//=============================================================================
//  MusE Score
//  Linux Music Score Editor
//  $Id: globals.h 5576 2012-04-24 19:15:22Z wschweer $
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

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <QtGlobal>
#include <QPaintDevice>
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

extern bool enableExperimental;
extern bool enableTestMode;
extern bool noSeq;            ///< Dont use sequencer; cmd line option.
extern bool noMidi;           ///< Dont use midi; cmd line option.
extern bool midiInputTrace;   ///< debug option: dump midi input
extern bool midiOutputTrace;  ///< debug option: dump midi output
extern bool converterMode;
extern bool pluginMode;
extern double converterDpi;
extern double guiScaling;
extern int trimMargin;
extern bool noWebView;
extern bool ignoreWarnings;

//---------------------------------------------------------
// MsWidget
// used to assign actions (shortcuts) to the apropriate
// widget (actions.cpp, shortcuts.cpp)
//---------------------------------------------------------

enum MsWidget {
      MAIN_WINDOW = 0,
      SCORE_TAB   = 1 << 0
      };

//---------------------------------------------------------
//    ScoreState
//    used also to mask out shortcuts (actions.cpp)
//---------------------------------------------------------

enum ScoreState {
      STATE_INIT                                = 0,
      STATE_DISABLED                            = 1 <<  0,
      STATE_NORMAL                              = 1 <<  1,
   // STATE_NOTE_ENTRY_STAFF_PITCHED
      STATE_NOTE_ENTRY_STAFF_DRUM               = 1 <<  2,
      STATE_NOTE_ENTRY_STAFF_TAB                = 1 <<  3,
      STATE_NOTE_ENTRY_METHOD_STEPTIME          = 1 <<  4,
      STATE_NOTE_ENTRY_METHOD_REPITCH           = 1 <<  5,
      STATE_NOTE_ENTRY_METHOD_RHYTHM            = 1 <<  6,
      STATE_NOTE_ENTRY_METHOD_REALTIME_AUTO     = 1 <<  7,
      STATE_NOTE_ENTRY_METHOD_REALTIME_MANUAL   = 1 <<  8,
      STATE_EDIT                                = 1 <<  9,
      STATE_TEXT_EDIT                           = 1 << 10,
      STATE_LYRICS_EDIT                         = 1 << 11,
      STATE_HARMONY_FIGBASS_EDIT                = 1 << 12,
      STATE_PLAY                                = 1 << 13,
      STATE_FOTO                                = 1 << 14,
      STATE_LOCK                                = 1 << 15,

      STATE_NEVER                               = 1 << 31,

      STATE_NOTE_ENTRY_STAFF_PITCHED = STATE_NOTE_ENTRY_METHOD_STEPTIME
                                        | STATE_NOTE_ENTRY_METHOD_REPITCH
                                        | STATE_NOTE_ENTRY_METHOD_RHYTHM
                                        | STATE_NOTE_ENTRY_METHOD_REALTIME_AUTO
                                        | STATE_NOTE_ENTRY_METHOD_REALTIME_MANUAL,
      STATE_NOTE_ENTRY 		= STATE_NOTE_ENTRY_STAFF_PITCHED | STATE_NOTE_ENTRY_STAFF_DRUM | STATE_NOTE_ENTRY_STAFF_TAB,
      STATE_ALLTEXTUAL_EDIT	= STATE_TEXT_EDIT | STATE_LYRICS_EDIT | STATE_HARMONY_FIGBASS_EDIT,
      STATE_ALL        		= -1
      };

//---------------------------------------------------------
//   MidiRemoteType
//---------------------------------------------------------

enum MidiRemoteType {
      MIDI_REMOTE_TYPE_INACTIVE = -1,
      MIDI_REMOTE_TYPE_NOTEON = 0, MIDI_REMOTE_TYPE_CTRL
      };

//---------------------------------------------------------
//   MidiRemote
//---------------------------------------------------------

struct MidiRemote {
      int channel;
      MidiRemoteType type;
      int data;         // pitch or controller number
      };

extern const char* stateName(ScoreState);

static constexpr qreal DPI_DISPLAY     = 96.0;  // 96 DPI nominal resolution
static constexpr qreal DPMM_DISPLAY    = DPI_DISPLAY / 25.4;
static constexpr qreal PALETTE_SPATIUM = 1.764 * DPMM_DISPLAY;

extern QPaintDevice* pdev;

} // namespace Ms
#endif
