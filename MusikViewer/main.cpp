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

#include <QGuiApplication>
#include <QApplication>
#include <QQmlEngine>
#include <QQuickView>
#include <libmscore/mscore.h>
#include <QQmlContext>
#include <QQuickStyle>

#include <applicationui.h>
#include <keyreciver.h>

#ifdef Q_OS_ANDROID
#include <androidExtra/filedialogutils.h>
#endif

#include "QString"
#include "QFile"
#include "QDir"

#if defined(Q_OS_IOS)
 //Extra includes here
#elif defined(Q_OS_ANDROID)
#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QAndroidActivityResultReceiver>

#endif

//namespace Ms {
//QString revision;
//bool enableTestMode = false;
//}

bool copy_dir_recursive(QString from_dir, QString to_dir, bool replace_on_conflit)
{
    QDir dir;
    dir.setPath(from_dir);

    from_dir += QDir::separator();
    to_dir += QDir::separator();

    foreach (QString copy_file, dir.entryList(QDir::Files))
    {
        QString from = from_dir + copy_file;
        QString to = to_dir + copy_file;

        if (QFile::exists(to))
        {
            if (replace_on_conflit)
            {
                if (QFile::remove(to) == false)
                {
                    return false;
                }
            }
            else
            {
                continue;
            }
        }

        if (QFile::copy(from, to) == false)
        {
            return false;
        }
    }

    foreach (QString copy_dir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        QString from = from_dir + copy_dir;
        QString to = to_dir + copy_dir;

        if (dir.mkpath(to) == false)
        {
            return false;
        }

        if (copy_dir_recursive(from, to, replace_on_conflit) == false)
        {
            return false;
        }
    }

    return true;
}


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQuickStyle::setStyle("Material");

    QApplication  app(argc, argv);


    QQmlEngine *engine;


    QDir dir(".");
    dir.mkdir("sound");
//    dir.cd()
    QFile soundfont(":/sound/FluidR3Mono_GM.sf3");
    soundfont.copy("./sound/FluidR3Mono_GM.sf3");

//    copy_dir_recursive("assets:/instruments" , "./instruments" , false);
//    copy_dir_recursive("assets:/locale" , "./locale" , false);
//    copy_dir_recursive("assets:/manual" , "./manual" , false);
//    copy_dir_recursive("assets:/plugins" , "./plugins" , false);
//    copy_dir_recursive("assets:/sound" , "./sound" , false);
//    copy_dir_recursive("assets:/styles" , "./styles" , false);
//    copy_dir_recursive("assets:/templates" , "./templates" , false);
//    copy_dir_recursive("assets:/wallpaper" , "./wallpaper" , false);
//    copy_dir_recursive("assets:/workspaces" , "./workspaces" , false);

    Ms::MScore::init();

    engine = Ms::MScore::qml();

    ApplicationUI appui;

    QQuickView view(engine , Q_NULLPTR);



    QScreen *screen = qApp->primaryScreen();
    int dpi = screen->physicalDotsPerInch() * screen->devicePixelRatio();
    bool isMobile = false;

#if defined(Q_OS_IOS)
    // iOS integration of scaling (retina, non-retina, 4K) does itself.
    dpi = screen->physicalDotsPerInch();
    isMobile = true;
#elif defined(Q_OS_ANDROID)
    // https://bugreports.qt-project.org/browse/QTBUG-35701
    // recalculate dpi for Android

    QAndroidJniEnvironment env;
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject resources = activity.callObjectMethod("getResources", "()Landroid/content/res/Resources;");
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();

        return EXIT_FAILURE;
    }

    QAndroidJniObject displayMetrics = resources.callObjectMethod("getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();

        return EXIT_FAILURE;
    }
    dpi = displayMetrics.getField<int>("densityDpi");
    isMobile = true;
#else
    // standard dpi
    dpi = screen->logicalDotsPerInch() * screen->devicePixelRatio();
#endif

    // now calculate the dp ratio
    qreal dp = dpi / 160.f;

    dp = 1;

    view.rootContext()->setContextProperty("dp", dp);
    view.rootContext()->setContextProperty("isMobile", isMobile);
    view.rootContext()->setContextProperty("app", &appui);
    //    view.setHeight(400);
    //    view.setWidth(400);

    KeyReciver reci;
    view.installEventFilter(&reci);
    #ifdef Q_OS_ANDROID
    FileDialogUtils utils;
    utils.setEngine(engine);
#endif
    appui.setEngine(engine);
    view.rootContext()->setContextProperty("ckeys" , &reci);
    #ifdef Q_OS_ANDROID
    view.rootContext()->setContextProperty("futils" , &utils);
#endif

    view.setSource(QUrl("qrc:/main.qml"));
//    view.setSource(QUrl("qrc:/flickexample.qml"));


    view.show();

    //    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    //    if (engine.rootObjects().isEmpty())
    //        return -1;
    int ret;
    ret= app.exec();

    delete engine;

    return ret;
}
