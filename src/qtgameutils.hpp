#pragma once
#include <QString>

namespace QTGameUtils{

    bool FileExistsOnDisk(const QString &filePath);
    bool CreateUserDirectory(const QString &directoryPath);
    QString getCurrentDate();
    QString GetUserRoamingPath(const QString& gameTitle="");
    void CreateGameJSON(const QString& gamePath, bool* isReadyFile);
    bool SetupGameConfig(const QString& gameTitle);
    void SetupManagerConfig(const QString& roamingPath);
    QString getRandomFilePath(const QString &directoryPath);

};
