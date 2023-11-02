#include "GameProfile.h"
#pragma once
using namespace ConfigUtils;

class CGameManager {

public:

    CGameManager(const char* path) {
        this->m_JsonPath = path;
        bool canAccessFile = isFileAccessable(this->m_JsonPath);

        if (!canAccessFile) {
            std::cout << "\nCannot access submanager json.";
            CreateNewManagerConfig();
            return;  }

        CollectJsonValues();
    }

    std::string getGameName(){
        return this->m_gameTitle;
    }

    CGameProfile getActiveProfile(){
        return *this->m_activeProfile;
    }

    std::string getActiveProfileName(){
        return this->m_gameTitle;
    }

    unsigned int getProfileCount(){
        return this->m_NumProfiles;
    }

    bool hasActiveProfile(){
        return (this->m_activeProfile);
    }

protected:
    std::string m_gameTitle;
    std::string m_activeProfileID;
    std::string m_JsonPath;
    JSON m_ManagerJson;

private:
    int m_NumProfiles;
    std::string m_rootGamePath;
    std::vector<CGameProfile> m_GameProfiles;
    CGameProfile* m_activeProfile = nullptr;

    void CreateNewManagerConfig() {
        this->m_rootGamePath = "";
        this->m_gameTitle = "";
        this->m_NumProfiles = 0;
        this->m_activeProfileID = "";
    }

    void CollectJsonValues() {
        SetupExistingJSON(m_ManagerJson,m_JsonPath);

        m_rootGamePath = this->m_ManagerJson["Game Root Path"];
        m_gameTitle = this->m_ManagerJson["Game Title"];
        m_activeProfileID = this->m_ManagerJson["Active Profile"];
        m_NumProfiles = this->m_ManagerJson["Total Profiles"];

        for (int i = 0; i < m_NumProfiles; i++) {
            std::string key = "Profile_" + std::to_string(i) + " Path";
            std::string path = m_ManagerJson[key.c_str()];
            CGameProfile gameProfile( path.c_str() );

            if (gameProfile.m_NumMods > 0)
            {
                this->m_GameProfiles.push_back(gameProfile);

                if ( gameProfile.getProfileName() == m_activeProfileID )
                    this->m_activeProfile = &gameProfile;
            }
        }

    }

};
