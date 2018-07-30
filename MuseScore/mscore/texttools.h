//Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>
//=============================================================================
//  MuseScore
//  Music Composition & Notation
//  $Id:$
//
//  Copyright (C) 2002-2011 Werner Schweer
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENCE.GPL
//=============================================================================

#ifndef __TEXTTOOLS_H__
#define __TEXTTOOLS_H__

namespace Ms {

class Text;

//---------------------------------------------------------
//   TextTools
//---------------------------------------------------------

class TextTools : public QDockWidget {
      Q_OBJECT

      Text* _textElement;

      QDoubleSpinBox* typefaceSize;
      QFontComboBox* typefaceFamily;
      QAction* typefaceBold;
      QAction* typefaceItalic;
      QAction* typefaceUnderline;
      QAction* typefaceSubscript;
      QAction* typefaceSuperscript;
      QAction* showKeyboard;

      void blockAllSignals(bool val);
      void updateText();
      void layoutText();

   private slots:
      void sizeChanged(double value);
      void fontChanged(const QFont&);
      void boldClicked(bool);
      void italicClicked(bool);
      void underlineClicked(bool);
      void subscriptClicked(bool);
      void superscriptClicked(bool);
      void showKeyboardClicked(bool);

   public:
      TextTools(QWidget* parent = 0);
      void setText(Text* te);
      void updateTools();
      QAction* kbAction() const { return showKeyboard; }
      void toggleBold();
      void toggleItalic();
      void toggleUnderline();
      };
}

#endif

