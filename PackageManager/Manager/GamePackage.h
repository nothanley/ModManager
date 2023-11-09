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

    void setAssetPath(const std::string& assetPath);
    void setName(const char* name);
    void setDescription(const char* desc);
    void setType(const char* type);
    void setAuthorName(const char* name);
    void addCreationDate();
    void setEnabled(bool toggle);
    bool hasThumbnail();
    bool getStatus();
    unsigned int getSize();
    int getIndex();
    long long getMD5();
    float getFileVersion();
    std::string getAuthor();
    std::string getReplaceAssetName();
    std::string getPath();
    std::string getName();
    std::string getType();
    std::string getDescription();

protected:
    std::string m_JsonPath;
    JSON m_ProfileJson;

private:
    void CollectJsonValues();
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

};



