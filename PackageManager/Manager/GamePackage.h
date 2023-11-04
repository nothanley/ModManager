#include "../Utils/ConfigUtils.hpp"
#pragma once
using namespace ConfigUtils;


class CGamePackage {

public:

    CGamePackage( const char* packageName, int index ){
        this->m_JsonPath =  std::string("");
        this->m_PackageName = packageName;
        this->m_ThumbnailPath =  std::string("dummy_thumbnail_path");
        this->m_AssetPath = std::string("dummy_asset_path");
        this->isEnabled = true;
        this->m_LoadIndex = index;
        this->m_AuthorName = "Unknown";
        this->m_CreationDate = "Unknown";
        this->m_PackageType = "Misc";
        this->m_FileSize = 0;
        this->m_GameTitle = "Unknown";
        this->m_ReplacementTitle = "None";
        this->m_PackageDescription = "No Description.";
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
        return this->isEnabled;
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

    unsigned int m_FileSize;
    bool isEnabled = false; /* todo: make this protected or friend class variable*/
    int m_LoadIndex;

    void CollectJsonValues() {

        this->m_PackageName = m_ProfileJson["Package Name"];
        this->m_ThumbnailPath = m_ProfileJson["Thumbnail Path"];
        this->m_AssetPath = m_ProfileJson["Asset Path"];
        this->isEnabled = m_ProfileJson["Is Used"];
        this->m_LoadIndex = m_ProfileJson["Load Index"];

    }

};
