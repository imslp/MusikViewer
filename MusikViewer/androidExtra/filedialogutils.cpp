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


#include "filedialogutils.h"
#include <QJSValue>
#include <QJSEngine>
#include <QQmlEngine>
#include <QGuiApplication>
#include <QDir>
#include <QDebug>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QAndroidActivityResultReceiver>
#include <QMessageBox>
#endif



FileDialogUtils::FileDialogUtils(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_ANDROID
    QStringList perms;
    perms << "android.permission.WRITE_EXTERNAL_STORAGE";
    QtAndroid::requestPermissionsSync(perms);
    QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission.WRITE_EXTERNAL_STORAGE");

    if(r == QtAndroid::PermissionResult::Denied){
        QMessageBox::critical(nullptr , "" , "Could not get permission for sdcard");
        qDebug() << "Android permisions false!!!";
    }

    qDebug() << "Android Permisions done";

    //    androidItent();


    connect(&recei , &ActivityResultReceiver::intentOk , this , &FileDialogUtils::IntentCompleted);

#endif
}

void FileDialogUtils::addShortcutFromStandardLocation(const QString &name, QStandardPaths::StandardLocation loc, bool local)
{
    QStringList readPaths = QStandardPaths::standardLocations(loc);
    QString path = readPaths.isEmpty() ? QString() : local ? readPaths.first() : readPaths.last();
    addShortcut(name, QStandardPaths::displayName(loc), path);
}


void FileDialogUtils::addShortcut(const QString &name, const QString &visibleName, const QString &path)
{

    QUrl url = QUrl::fromLocalFile(path);

    // Since the app can have bindings to the shortcut, we always add it
    // to the public API, even if the directory doesn't (yet) exist.
    m_shortcuts.setProperty(name, url.toString());

    // ...but we are more strict about showing it as a clickable link inside the dialog
    if (visibleName.isEmpty() || !QDir(path).exists())
        return;

    QJSValue o = engine->newObject();
    o.setProperty("name", visibleName);
    // TODO maybe some day QJSValue could directly store a QUrl
    o.setProperty("url", url.toString());

    int length = m_shortcutDetails.property(QLatin1String("length")).toInt();
    m_shortcutDetails.setProperty(length, o);
}


QJSValue FileDialogUtils::__shortcuts()
{
    if (m_shortcutDetails.isUndefined())
        populateShortcuts();

    return m_shortcutDetails;
}

void FileDialogUtils::populateShortcuts()
{
    m_shortcutDetails = engine->newArray();
    m_shortcuts = engine->newObject();

    addShortcutFromStandardLocation(QLatin1String("Downloads"), QStandardPaths::DownloadLocation);
    addShortcutFromStandardLocation(QLatin1String("Music"), QStandardPaths::MusicLocation);
    addShortcutFromStandardLocation(QLatin1String("Movies"), QStandardPaths::MoviesLocation);
    addShortcutFromStandardLocation(QLatin1String("Documents"), QStandardPaths::DocumentsLocation);



    addShortcutFromStandardLocation(QLatin1String("home"), QStandardPaths::HomeLocation);

    //    addShortcutFromStandardLocation(QLatin1String("Data"), QStandardPaths::GenericDataLocation);


#ifdef Q_OS_ANDROID

    if(QtAndroid::androidSdkVersion()<21){

        QStringList readPaths = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
        QString path = readPaths.isEmpty() ? QString() : false ? readPaths.first() : readPaths.last();
        addShortcut(tr("Phone"), tr("Phone"), path);
        return;
    }

    //    QString sd = qgetenv("EXTERNAL_STORAGE");

    //    if(!sd.isEmpty()){
    //        addShortcut("sdcard" , "sdcard" , sd);
    //    }


    QAndroidJniObject ctx =  QtAndroid::androidContext();

    QAndroidJniObject mediaDir =  ctx.callObjectMethod("getExternalFilesDirs" , "(Ljava/lang/String;)[Ljava/io/File;" , NULL );



    //    QAndroidJniObject mediaDir = QAndroidJniObject::callStaticObjectMethod("android/content/Context", "getExternalFilesDirs", "(Ljava/lang/String;)[Ljava/io/File;" , NULL);
    jobjectArray mediaarr = mediaDir.object<jobjectArray>();

    QAndroidJniEnvironment jniEnv;
    jint size = jniEnv->GetArrayLength(mediaarr);

    for(int i=0 ; i<size ; i++ ){
        QAndroidJniObject tmp = QAndroidJniObject::fromLocalRef(jniEnv->GetObjectArrayElement(mediaarr , i));
        QAndroidJniObject mediaPath = tmp.callObjectMethod( "getAbsolutePath", "()Ljava/lang/String;" );
        QString dataAbsPath = mediaPath.toString();
        qDebug() << "JNI PATH " << i << dataAbsPath;

    }



    QAndroidJniObject tmp1 = QAndroidJniObject::fromLocalRef(jniEnv->GetObjectArrayElement(mediaarr , 0));
    QAndroidJniObject mediaPath1 = tmp1.callObjectMethod( "getAbsolutePath", "()Ljava/lang/String;" );
    QString phonePathDirty = mediaPath1.toString();
    QString phonePath = phonePathDirty.left(phonePathDirty.indexOf("Android"));
    addShortcut(tr("Phone"), tr("Phone"), phonePath);
    phone_path = phonePath;

    if(size>=2){

        QAndroidJniObject tmp = QAndroidJniObject::fromLocalRef(jniEnv->GetObjectArrayElement(mediaarr , 1));
        QAndroidJniObject mediaPath = tmp.callObjectMethod( "getAbsolutePath", "()Ljava/lang/String;" );
        QString sdPathDirty = mediaPath.toString();
        QString sdPath = sdPathDirty.left(sdPathDirty.indexOf("Android"));
        addShortcut("sdcard" , "sdcard" , sdPath);
        QDir dir(sdPath);
        sdcard_name = dir.dirName();
        sdcard_path = sdPath;
    }

    qDebug() << "getExternalFilesDirs size" << size;



#endif
}

bool FileDialogUtils::isSdcardPath(QString path)
{

    qDebug() << Q_FUNC_INFO << path << sdcard_path;

    return path.startsWith(sdcard_path);
}

void FileDialogUtils::setEngine(QQmlEngine *value)
{
    engine = value;
}

void FileDialogUtils::androidItent(QString name)
{
    qDebug() << "1";

    QAndroidJniObject intent("android/content/Intent" , "()V");
    qDebug() << "2";
    QAndroidJniObject ACTION_CREATE_DOCUMENT = QAndroidJniObject::fromString("android.intent.action.CREATE_DOCUMENT");
    qDebug() << "3";

    if (ACTION_CREATE_DOCUMENT.isValid() && intent.isValid())
    {
        intent.callObjectMethod("setAction","(Ljava/lang/String;)Landroid/content/Intent;",ACTION_CREATE_DOCUMENT.object<jstring>());
        qDebug() << "4";
        intent.callObjectMethod("addCategory" , "(Ljava/lang/String;)Landroid/content/Intent;" ,QAndroidJniObject::fromString("android.intent.category.OPENABLE").object<jstring>());
        qDebug() << "5";
        //        intent.callObjectMethod("setType","(Ljava/lang/String;)Landroid/content/Intent;",QAndroidJniObject::fromString("application/pdf").object<jstring>());
        intent.callObjectMethod("setType","(Ljava/lang/String;)Landroid/content/Intent;",QAndroidJniObject::fromString("file/*").object<jstring>());
        qDebug() << "6";
        intent.callObjectMethod("putExtra","(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",QAndroidJniObject::fromString("android.intent.extra.TITLE").object<jstring>(),QAndroidJniObject::fromString(name).object<jstring>());
        qDebug() << "QtAndroid::startIntentSender";
        QtAndroid::startActivity(intent.object<jobject>(),58,&recei);
    }
}

void FileDialogUtils::IntentCompleted(QString path, int fd)
{
    //    qDebug() << "sdcard name" << sdcard_name;


    if(path.contains("/primary:")){
        path = path.right(path.indexOf("/primary:"));
        path.prepend(phone_path +"/");
    }
    if(path.contains("/"+sdcard_name+":")){
        //        qDebug() << "Contains sdcard name";

        QString mpath = path;
        QString name("/"+sdcard_name+":");
        int index = path.indexOf( name, Qt::CaseInsensitive) + name.size();
        //        qDebug() << "Index" << index;

        mpath = path.mid(index);
        mpath.prepend(sdcard_path);
        path = mpath;
    }

    emit actionCompleted(path, fd);
}

void FileDialogUtils::closeFD()
{
    disconnect(this, SLOT(closeFD()));
    qDebug() << Q_FUNC_INFO;

    QAndroidJniObject pfd = recei.getPfd();
    if(pfd.isValid())
        pfd.callMethod<void>("close" , "()V");

}

void ActivityResultReceiver::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data)
{


    if(receiverRequestCode == 58 && resultCode!=0){
        qDebug() << "Activity result!";
        QAndroidJniObject uri = data.callObjectMethod("getData" ,"()Landroid/net/Uri;");

        QAndroidJniObject jstr = uri.callObjectMethod( "getPath", "()Ljava/lang/String;" );


        //         QAndroidJniObject file("java/io/File" ,"(Ljava/lang/String;)V", jstr.object<jstring>());


        qDebug() << "Path" <<  jstr.toString();//file.callObjectMethod( "getAbsolutePath", "()Ljava/lang/String;" ).toString();

        QString path = jstr.toString();
        QAndroidJniObject activity = QtAndroid::androidContext();
        QAndroidJniObject contentResolver = activity.callObjectMethod("getContentResolver" , "()Landroid/content/ContentResolver;") ;

        pfd = contentResolver.callObjectMethod("openFileDescriptor" , "(Landroid/net/Uri;Ljava/lang/String;)Landroid/os/ParcelFileDescriptor;" , uri.object<jobject>() , QAndroidJniObject::fromString("rw").object<jstring>());


        //        int fd = pfd.callMethod<jint>("detachFd" , "()I"); ///This works

        int fd = pfd.callMethod<jint>("getFd" , "()I");


        qDebug() << "File descryptor" << fd;

        emit intentOk(path , fd);

    }

}

QAndroidJniObject ActivityResultReceiver::getPfd() const
{
    return pfd;
}
