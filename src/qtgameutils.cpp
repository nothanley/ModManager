#include "qtgameutils.hpp"

bool
QTGameUtils::FileExistsOnDisk(const QString &filePath) {
    QFile file(filePath);
    return file.exists();
}

bool
QTGameUtils::CreateRoamingDirectory(const QString &directoryPath) {
    QDir dir(directoryPath);
    if (!dir.exists())
        return dir.mkpath(directoryPath);
    return true;
}

QString
QTGameUtils::GetUserRoamingPath(const QString& gameTitle){
    QString roamingPath = (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (gameTitle.size() == 0){ return roamingPath; }
    return roamingPath + "/" + gameTitle;
}


void
QTGameUtils::CreateGameJSON(const QString& gamePath, bool* isReadyFile){
   if ( FileExistsOnDisk(gamePath+"/game_config.json") ){
        /* todo: check json integrity */
        *isReadyFile = true;
        return; }

    // Create default json config
   QJsonObject jsonObject;
   jsonObject["Game Title"] = gamePath.split("/").last();
   jsonObject["Game Root Path"] = "";
   jsonObject["Active Profile"] = "";
   jsonObject["Total Profiles"] = 0;

   // Save to user roaming path
   QJsonDocument jsonDocument(jsonObject);
   QFile file(gamePath+"/game_config.json");
   *isReadyFile = file.open(QIODevice::WriteOnly);

   if (isReadyFile) {
       file.write(jsonDocument.toJson());
       file.close(); }
}

bool
QTGameUtils::SetupGameConfig(const QString& gameTitle){
    QString gamePath = GetUserRoamingPath("Games/" + gameTitle);
    bool isValidDirectory = CreateRoamingDirectory(gamePath);
    if (!isValidDirectory) return false;

    bool isGameJsonReady = false;
    CreateGameJSON(gamePath,&isGameJsonReady);
    return isGameJsonReady && isValidDirectory;
}

void
QTGameUtils::SetupManagerConfig(const QString& roamingPath){
    bool isValidDirectory = CreateRoamingDirectory(roamingPath);
    if (!isValidDirectory)
    {
        qDebug() << "Failed to setup MANAGER config.";
        Q_ASSERT(isValidDirectory);
        return;
    }

    SetupGameConfig("WWE 2K23");
    SetupGameConfig("WWE 2K22");
}


#include <QDir>
#include <QRandomGenerator>
#include <QImage>


QString
QTGameUtils::getRandomFilePath(const QString &directoryPath) {
    QDir directory(directoryPath);
    QStringList files = directory.entryList(QDir::Files);

    if (files.isEmpty()) {
        return QString(); // Return an empty QString if the directory is empty
    }

    int randomIndex = QRandomGenerator::global()->bounded(files.size());
    QString randomFile = files[randomIndex];

    return directoryPath + "/" + randomFile;
}
