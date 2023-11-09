#include "../Utils/ConfigUtils.hpp"
#pragma once
using namespace ConfigUtils;


class CGamePackage {

public:

    CGamePackage( const char* name, const char* type, const char* assetPath ){
        setAssetPath(assetPath);
        setName(name);
        setAuthorName("N/A");
        addCreationDate();
        setType(type);
        setDescription("No Description.");
    }

    CGamePackage(const char* path) {
        this->m_JsonPath = path;
        bool canAccessFile = isFileAccessable(this->m_JsonPath);

        try { CollectJsonValues();}
        catch (...) {
            throw std::runtime_error("Invalid Mod Configuration."); }
    }

    void setAssetPath(const std::string& assetPath) {
        this->m_AssetPath = assetPath;
    }

    void setName(const char* name) {
        this->m_PackageName = name;
    }

    void setDescription(const char* desc) {
        this->m_PackageDescription = desc;
    }

    void setType(const char* type) {
        this->m_PackageType = type;
    }

    void setAuthorName(const char* name) {
        this->m_AuthorName = name;
    }

    void addCreationDate() {
        this->m_CreationDate = "MM/DD/YY";
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

    std::string getReplaceAssetName(){
        return this->m_ReplacementTitle;
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

    //void saveToJson(JSON* json, const std::string& profile) {
    //    json->at(profile)[this->getName()]["path"] = this->getPath();
    //}

private:
    std::string m_CreationDate;
    std::string m_PackageName;
    std::string m_PackageType;
    std::string m_ThumbnailPath;
    std::string m_AssetPath;
    std::string m_AuthorName;
    std::string m_ReplacementTitle;
    std::string m_PackageDescription;
    std::string m_AuthorLink;

    long long m_FileHash = 0;
    float m_FileVersion= 0;
    bool m_IsEnabled = true;
    unsigned int m_FileSize = 0;
    int m_LoadIndex = 0;

    void CollectJsonValues() {
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

};



