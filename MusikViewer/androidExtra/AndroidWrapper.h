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

#ifndef ANDROIDWRAPPER_H
#define ANDROIDWRAPPER_H

#include <QObject>
#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QAndroidActivityResultReceiver>

/*/
#include <android/log.h>
#define TAG "org.firebird.emu"
//*/

class AndroidWrapper : public QObject, public QAndroidActivityResultReceiver {
    Q_OBJECT
public:
    explicit AndroidWrapper(QObject *parent = 0);
    void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data);
    Q_INVOKABLE static int is_content_provider_supported();
    Q_INVOKABLE void selectFile(bool selectExisting);
    void shareIntent(QString path  , QString mime);

private:
    const static int SDCARD_DOCUMENT_REQUEST = 2;
    const static int API__STORAGE_ACCESS_FRAMEWORK = 21;

signals:
    void filePicked(QString fileUrl);
};

extern "C" int is_android_provider_file(const char *path);
extern "C" int android_get_fd_for_uri(const char *path, const char *mode);

#endif // ANDROIDWRAPPER_H
