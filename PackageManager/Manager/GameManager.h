#include "GameProfile.h"
#pragma once
using namespace ConfigUtils;

class CGameManager {

public:

    CGameManager(const char* path) {
        this->m_JsonPath = path;
        bool canAccessFile = isFileAccessable(this->m_JsonPath);

        if (!canAccessFile) {
            /* Perform first time setup on config */
            std::cout << "\nCannot access submanager json.";
            CreateNewManagerConfig();
            return;
        }

        CollectJsonValues();
    }

protected:
    std::string m_JsonPath;
    JSON m_ManagerJson;

private:
    int m_NumProfiles;
    std::string m_rootGamePath;
    std::string m_gameTitle;
    std::string m_activeProfile;
    std::vector<CGameProfile> m_GameProfiles;

    void CreateNewManagerConfig() {
        //todo: query GUI for values
        this->m_rootGamePath = "";
        this->m_gameTitle = "";
        this->m_NumProfiles = 0;
        this->m_activeProfile = "";
    }

    void CollectJsonValues() {
        SetupExistingJSON(m_ManagerJson,m_JsonPath);

        m_rootGamePath = this->m_ManagerJson["Game Root Path"];
        m_gameTitle = this->m_ManagerJson["Game Title"];
        m_activeProfile = this->m_ManagerJson["Active Profile"];
        m_NumProfiles = this->m_ManagerJson["Total Profiles"];

        for (int i = 0; i < m_NumProfiles; i++) {
            std::string key = "Profile_" + std::to_string(i) + " Path";
            std::string path = m_ManagerJson[key.c_str()];
            CGameProfile newProfile( path.c_str() );

            if (newProfile.m_NumMods > 0)
                this->m_GameProfiles.push_back(newProfile);
        }

    }

};