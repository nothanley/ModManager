#include "../Utils/ConfigUtils.hpp"
#pragma once
using namespace ConfigUtils;


class CGamePackage {

public:
    std::string m_PackageName;
    std::string m_ThumbnailPath;
    std::string m_AssetPath;
    bool isEnabled = false;
    int m_LoadIndex;

    CGamePackage(const char* path) {
        this->m_JsonPath = path;
        bool canAccessFile = isFileAccessable(this->m_JsonPath);
        if (!canAccessFile) { 
            /* Perform first time setup on config */
            std::cout << "\nCannot access mod package.";
            return; }

        CollectJsonValues();
    }

protected:
    std::string m_JsonPath;
    JSON m_ProfileJson;

private:

    void CollectJsonValues() {

        this->m_PackageName = m_ProfileJson["Package Name"];
        this->m_ThumbnailPath = m_ProfileJson["Thumbnail Path"];
        this->m_AssetPath = m_ProfileJson["Asset Path"];
        this->isEnabled = m_ProfileJson["Is Used"];
        this->m_LoadIndex = m_ProfileJson["Load Index"];

    }

};