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

#ifndef FILEDIALOGUTILS_H
#define FILEDIALOGUTILS_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QJSValue>
#include <QJSEngine>
#include <QQmlEngine>
#include <QStandardPaths>

#include <QAndroidActivityResultReceiver>


class ActivityResultReceiver : public QObject , public QAndroidActivityResultReceiver {

    Q_OBJECT
    // QAndroidActivityResultReceiver interface
public:
    ActivityResultReceiver(){}
    void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data);

    QAndroidJniObject getPfd() const;

signals:
    void intentOk(QString path , int fd);
private:
    QAndroidJniObject pfd;


};


class FileDialogUtils : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJSValue __shortcuts READ __shortcuts CONSTANT) // map of details for QML dialog implementations
public:
    explicit FileDialogUtils(QObject *parent = nullptr);

    void addShortcutFromStandardLocation(const QString &name, QStandardPaths::StandardLocation loc, bool local=true);
    void addShortcut(const QString &name, const QString &visibleName, const QString &path);
    QJSValue __shortcuts();
    void setEngine(QQmlEngine *value);
    void populateShortcuts();
    bool isSdcardPath(QString path);

signals:
    void actionCompleted(QString path , int fd);

public slots:
    void androidItent(QString name);
    void IntentCompleted(QString path , int fd);
    void closeFD();

private:
    QJSValue m_shortcuts;
    QJSValue m_shortcutDetails;

    QQmlEngine *engine;
    ActivityResultReceiver recei;
    QString sdcard_name;
    QString sdcard_path;
    QString phone_name;
    QString phone_path;

};



#endif // FILEDIALOGUTILS_H
