// (c) 2017 Ekkehard Gentz (ekke) @ekkescorner
// my blog about Qt for mobile: http://j.mp/qt-x

#ifndef ANDROIDSHAREUTILS_H
#define ANDROIDSHAREUTILS_H

#include <QtAndroid>
#include <QAndroidActivityResultReceiver>

#include "shareutils.hpp"

class AndroidShareUtils : public PlatformShareUtils, public QAndroidActivityResultReceiver
{
public:
    AndroidShareUtils(QObject* parent = 0);
    bool checkMimeTypeView(const QString &mimeType);
    bool checkMimeTypeEdit(const QString &mimeType);
    void share(const QString &text, const QUrl &url) override;
    void sendFile(const QString &filePath, const QString &title, const QString &mimeType, const int &requestId, const bool &altImpl) override;
    void viewFile(const QString &filePath, const QString &title, const QString &mimeType, const int &requestId, const bool &altImpl) override;
    void editFile(const QString &filePath, const QString &title, const QString &mimeType, const int &requestId, const bool &altImpl) override;

    void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data);
    void onActivityResult(int requestCode, int resultCode);

    void checkPendingIntents(const QString workingDirPath);

    static AndroidShareUtils* getInstance();

public slots:
    void setFileUrlReceived(const QString &url);
    void setFileReceivedAndSaved(const QString &url);
    bool checkFileExits(const QString &url);

private:
    bool mIsEditMode;
    qint64 mLastModified;
    QString mCurrentFilePath;

    static AndroidShareUtils* mInstance;

    void processActivityResult(int requestCode, int resultCode);

};


#endif // ANDROIDSHAREUTILS_H
