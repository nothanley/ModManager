#include "GamePackage.h"
using namespace ConfigUtils;


void 
CGamePackage::setAssetPath(const std::string& assetPath) {
    this->m_AssetPath = assetPath;
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
CGamePackage::setType(const char* type) {
    this->m_PackageType = type;
}

void 
CGamePackage::setAuthorName(const char* name) {
    this->m_AuthorName = name;
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
CGamePackage::getType() {
    return this->m_PackageType;
}

std::string 
CGamePackage::getDescription() {
    return this->m_PackageDescription;
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

std::string 
CGamePackage::getAuthor() {
    return this->m_AuthorName;
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
    //this->m_PackageName = m_ProfileJson["asset_name"];
    //this->m_AssetPath = m_ProfileJson["asset_path"];
    //this->m_PackageType = m_ProfileJson["asset_type"];
    //this->m_FileVersion = m_ProfileJson["asset_version"];
    //this->m_FileHash = m_ProfileJson["asset_md5"];
    //this->m_ThumbnailPath = m_ProfileJson["thumbnail_path"];
    //this->m_AuthorName = m_ProfileJson["author_name"];
    //this->m_ReplacementTitle = m_ProfileJson["replace_name"];
    //this->m_PackageDescription = m_ProfileJson["description"];
    //this->m_CreationDate = m_ProfileJson["creation_date"];
    //this->m_AuthorLink = m_ProfileJson["author_link"];
}