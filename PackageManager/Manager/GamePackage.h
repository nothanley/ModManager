#include "../Utils/ConfigUtils.hpp"
#pragma once
using namespace ConfigUtils;


class CGamePackage {

public:

    CGamePackage( const char* packageName, int index ){
        this->m_PackageName = packageName;
        this->m_IsEnabled = false;
        this->m_LoadIndex = index;
        this->m_AuthorName = "Unknown";
        this->m_CreationDate = "Unknown";
        this->m_PackageType = "Misc";
        this->m_GameTitle = "Unknown";
        this->m_PackageDescription = "No Description.";
        this->m_FileHash = 0;
        this->m_FileSize = 0;
    }

    CGamePackage(const char* path) {
        this->m_JsonPath = path;
        bool canAccessFile = isFileAccessable(this->m_JsonPath);
        if (!canAccessFile) { 
            /* Perform first time setup on config */
            std::cout << "\nCannot access mod package.";
            return; }

        CollectJsonValues();
    }

    bool hasThumbnail(){
        return !(m_ThumbnailPath == "");
    }

    std::string getPath(){
        return this->m_AssetPath;
    }

    std::string getName(){
        return this->m_PackageName;
    }

    std::string getType(){
        return this->m_PackageType;
    }

    std::string getDescription(){
        return this->m_PackageDescription;
    }

    unsigned int getSize(){
        return this->m_FileSize;
    }

    int getIndex(){
        return this->m_LoadIndex;
    }

    bool getStatus(){
        return this->m_IsEnabled;
    }

    void setEnabled(bool toggle){
        this->m_IsEnabled = toggle;
    }

    std::string getAuthor(){
        return this->m_AuthorName;
    }

    std::string getGameName(){
        return this->m_GameTitle;
    }

    std::string getReplaceAssetName(){
        return this->m_ReplacementTitle;
    }

    void setDescription(const char* description){
        this->m_PackageDescription = description;
    }

    long long getMD5(){
        return this->m_FileHash;
    }

    float getFileVersion(){
        return this->m_FileVersion;
    }

protected:
    std::string m_JsonPath;
    JSON m_ProfileJson;

private:
    std::string m_CreationDate;
    std::string m_PackageName;
    std::string m_PackageType;
    std::string m_ThumbnailPath;
    std::string m_AssetPath;
    std::string m_AuthorName;
    std::string m_GameTitle;
    std::string m_ReplacementTitle;
    std::string m_PackageDescription;


    long long m_FileHash;
    float m_FileVersion;
    bool m_IsEnabled;


    unsigned int m_FileSize;
    int m_LoadIndex;

    void CollectJsonValues() {

        this->m_PackageName = m_ProfileJson["Package Name"];
        this->m_ThumbnailPath = m_ProfileJson["Thumbnail Path"];
        this->m_AssetPath = m_ProfileJson["Asset Path"];
        this->m_IsEnabled = m_ProfileJson["Is Used"];
        this->m_LoadIndex = m_ProfileJson["Load Index"];

    }

};
