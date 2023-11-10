#include "GamePackage.h"
#include "../../qtgameutils.hpp"
#include <QDebug>

using namespace ConfigUtils;


std::string
CGamePackage::getAssetPath(){
    return this->m_AssetPath;
}

void 
CGamePackage::setAssetPath(const std::string& assetPath) {
    this->m_AssetPath = unifySlashEncoding(assetPath);
}

void 
CGamePackage::setName(const char* name) {
    this->m_PackageName = name;
}

void 
CGamePackage::setDescription(const char* desc) {
    this->m_PackageDescription = desc;
}

void
CGamePackage::setDate(const std::string& dateString){
    this->m_CreationDate = dateString;
}

void 
CGamePackage::setType(const char* type) {
    this->m_PackageType = type;
}

void 
CGamePackage::setAuthorName(const char* name) {
    this->m_AuthorName = name;
}

JSON
CGamePackage::serializeJsonObj(){
    JSON jsonConfig;
    jsonConfig["asset_name"] = this->getName();
    jsonConfig["asset_path"] = this->getPath();
    jsonConfig["asset_type"] = this->getType();
    jsonConfig["asset_version"] = this->getFileVersion();
    jsonConfig["asset_md5"] = this->getMD5();;
    jsonConfig["author_name"] = this->getAuthor();
    jsonConfig["replace_name"] = this->getReplaceAssetName();
    jsonConfig["description"] = this->getDescription();
    jsonConfig["creation_date"] = this->getDate();
    jsonConfig["author_link"] = this->getLink();
    jsonConfig["thumbnail_path"] = this->getThumbnailPath();
    return jsonConfig;
}

std::string
CGamePackage::getThumbnailPath(){
    return this->m_ThumbnailPath;
}

void
CGamePackage::setThumbnailPath(const std::string& path){
    this->m_ThumbnailPath = unifySlashEncoding(path);
}

void
CGamePackage::setJsonPath(const std::string& path){
    this->m_JsonPath = unifySlashEncoding(path);
}

bool
CGamePackage::saveJson(){
    std::string configDir = extractFolderPath(getPath());
    std::string configPath = configDir + "mod_config.json";
    if (!canWriteToFile(configPath)) return false;

    /* Generate JSON config */
    std::ofstream file(configPath, std::ios::out);
    file << std::setw(4) << this->serializeJsonObj();
    file.close();
    this->setJsonPath(configPath);
    return true;
}

bool
CGamePackage::moveContentsToRoot(const std::string& dir){
    std::string newAssetPath = (dir+"\\") + extractFileName(getPath());

    if (this->hasThumbnail())
    {   /* Copy thumbnail if exists */
        std::string thumbnailPath = (dir+"\\") +"thumbnail.jpg";
        if (!ConfigUtils::copyFile(getThumbnailPath(),thumbnailPath))
            return false;
        this->setThumbnailPath(thumbnailPath);  }

    if (!ConfigUtils::copyFile(getPath(),newAssetPath)) /* Attempt to copy assets to user path */
        return false;

    this->setAssetPath(newAssetPath); /* All copy operations successful */
    return true;
}

std::string
CGamePackage::getAssetFileName(){
    return ConfigUtils::extractFileName(this->m_AssetPath);
}

bool
CGamePackage::saveTo(const std::string& path){
    // Create and check validity of directory
    if (path == "" || getName() == "") return false;
    std::string modPath = extractFolderPath(path) + this->getName();
    if (!QTGameUtils::CreateUserDirectory(modPath.c_str())) return false;

    if (!this->moveContentsToRoot(modPath)){
        qDebug() << "\nFailed to copy contents";
        return false; }
    if (!this->saveJson()){
        qDebug() << "\nFailed to save json.";
        return false; }

    return true;
}

void 
CGamePackage::addCreationDate() {
    this->m_CreationDate = "MM/DD/YY";
}

bool 
CGamePackage::hasThumbnail() {
    return !(m_ThumbnailPath == "");
}

std::string 
CGamePackage::getPath() {
    return this->m_AssetPath;
}

std::string 
CGamePackage::getName() {
    return this->m_PackageName;
}

std::string
CGamePackage::getJsonPath(){
    return this->m_JsonPath;
}

std::string 
CGamePackage::getType() {
    return this->m_PackageType;
}

std::string 
CGamePackage::getDescription() {
    return this->m_PackageDescription;
}

std::string
CGamePackage::getLink(){
    return this->m_AuthorLink;
}
unsigned int 
CGamePackage::getSize() {
    return this->m_FileSize;
}

int 
CGamePackage::getIndex() {
    return this->m_LoadIndex;
}

bool 
CGamePackage::getStatus() {
    return this->m_IsEnabled;
}

void 
CGamePackage::setEnabled(bool toggle) {
    this->m_IsEnabled = toggle;
}

void
CGamePackage::setReplacements(const char* replacements){
    this->m_ReplacementTitle = replacements;
}

std::string 
CGamePackage::getAuthor() {
    return this->m_AuthorName;
}

std::string
CGamePackage::getDate() {
    return this->m_CreationDate;
}

std::string 
CGamePackage::getReplaceAssetName() {
    return this->m_ReplacementTitle;
}

long long 
CGamePackage::getMD5() {
    return this->m_FileHash;
}

float 
CGamePackage::getFileVersion() {
    return this->m_FileVersion;
}

void 
CGamePackage::CollectJsonValues() {

    SetupExistingJSON(m_ProfileJson, m_JsonPath);

    this->m_PackageName = m_ProfileJson["asset_name"];
    this->m_AssetPath = m_ProfileJson["asset_path"];
    this->m_PackageType = m_ProfileJson["asset_type"];
    this->m_FileVersion = m_ProfileJson["asset_version"];
    this->m_FileHash = m_ProfileJson["asset_md5"];
    this->m_AuthorName = m_ProfileJson["author_name"];
    this->m_ReplacementTitle = m_ProfileJson["replace_name"];
    this->m_PackageDescription = m_ProfileJson["description"];
    this->m_CreationDate = m_ProfileJson["creation_date"];
    this->m_AuthorLink = m_ProfileJson["author_link"];
    this->m_ThumbnailPath = m_ProfileJson["thumbnail_path"];
}
