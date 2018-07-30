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

#ifndef KEYRECIVER_H
#define KEYRECIVER_H

#include <QObject>
#include <QEvent>
#include <QKeyEvent>

class KeyReciver : public QObject
{
    Q_OBJECT
public:
    explicit KeyReciver(QObject *parent = nullptr);

signals:
    void backpressed();

public slots:

protected:
    bool eventFilter(QObject* obj, QEvent* event);

};

#endif // KEYRECIVER_H
