#include "GamePackage.h"
#pragma once
using namespace ConfigUtils;

class CGameProfile {

public:
    int m_NumMods = 0;

    CGameProfile(const char* path) {
        this->m_JsonPath = path;
        bool canAccessFile = isFileAccessable(this->m_JsonPath);

        if (!canAccessFile) {
            /* Perform first time setup on config */
            std::cout << "\nCannot access profile json.";
            CreateNewProfileConfig();
            return;
        }

        CollectJsonValues();
    }

    std::string getProfileName(){
        return this->m_ProfileName;
    }

    std::vector<CGamePackage*> getAllMods(){
        return this->m_PackageLoadOrder;
    }

    void addModPackage(CGamePackage* contents){
        this->m_PackageLoadOrder.push_back(contents);
    }

protected:
    std::vector<CGamePackage*> m_PackageLoadOrder = {nullptr};
    std::string m_JsonPath;
    JSON m_ProfileJson;

private:
    std::string m_ProfileName;
    std::vector<std::string> m_ModPaths;
    void CreateNewProfileConfig() {
        m_ProfileName = "Default";
        m_ModPaths = {};
    }

    void CollectJsonValues() { 

        this->m_ProfileName = m_ProfileJson["Profile Name"];
        this->m_NumMods = m_ProfileJson["Total Mods"];
        
        for (int i = 0; i < m_NumMods; i++) {
            std::string modKey = "ModPath_" + std::to_string(i);
            std::string modPath = m_ProfileJson[modKey.c_str()];
            m_ModPaths.push_back(modPath);
            
            CGamePackage* gameMod = new CGamePackage(modPath.c_str());
            if (gameMod->getPath() != "")
                { this->m_PackageLoadOrder.push_back(gameMod); }
        }

    }

};
