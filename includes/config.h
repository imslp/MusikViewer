//Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>
//=============================================================================
//  MusE
//  Linux Music Editor
//  $Id:$
//
//  Copyright (C) 2002-2010 by Werner Schweer and others
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

#include "all.h"

/* #undef USE_ALSA */
/* #undef USE_JACK */
//#define USE_PORTAUDIO
// #define USE_PORTMIDI
/* #undef USE_PULSEAUDIO */

//#define USE_JACK

#define USE_LAME
/* #undef USE_WEBKIT */

/* #undef FOR_WINSTORE */

#define MSCORE_UNSTABLE

/* #undef HAS_MIDI */
#define SCRIPT_INTERFACE
#define HAS_AUDIOFILE
#define USE_SSE

#define MUSESCORE_NAME_VERSION  "MuseScore 2 (2.2.0 unstable)"
#define INSTALL_NAME            "mscore-2.2/"
#define INSTPREFIX              "/usr/local"
#define VERSION                 "2.2.0"

/* #undef AEOLUS */
#define ZERBERUS
/* #undef OMR */
/* #undef OCR */
#define OSC
/* #undef OPENGL */
#define SOUNDFONT3

/* #undef Q_WS_UIKIT */

#define USE_BSP     true

// does not work on windows/mac:
//#define USE_GLYPHS  true

