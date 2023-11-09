#pragma once
#include <QString>
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

namespace QTGameUtils{

    bool FileExistsOnDisk(const QString &filePath);

    bool CreateRoamingDirectory(const QString &directoryPath);


    QString GetUserRoamingPath(const QString& gameTitle="");


    void CreateGameJSON(const QString& gamePath, bool* isReadyFile);


    bool SetupGameConfig(const QString& gameTitle);

    void SetupManagerConfig(const QString& roamingPath);

    QString getRandomFilePath(const QString &directoryPath);
};
