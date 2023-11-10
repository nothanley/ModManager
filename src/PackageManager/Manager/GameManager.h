#include "GameProfile.h"
#pragma once
using namespace ConfigUtils;

class CGameManager {

public:

    CGameManager();
    CGameManager(const char* path);
    CGameProfile* getActiveProfile();
    CGameProfile* getProfile(const char* name);
    void setJsonPath(const std::string& path);
    void setGameName(const char* name);
    void setGamePath(const char* path);
    void setActiveProfile(CGameProfile* profile);
    void addProfile(CGameProfile* profile, bool setActive = false);
    void createProfile(const char* name, bool setActive = false);
    void replaceProfile(CGameProfile* profile, const std::string& target);
    void updateJSON();
    void save();
    std::string getGameName();
    std::string getGamePath();
    std::string getJsonPath();
    unsigned int getProfileCount();
    bool hasActiveProfile();

protected:
    std::string m_gameTitle;
    std::string m_JsonPath;
    JSON m_ManagerJson;

private:
    std::string m_rootGamePath = "unknown";
    std::vector<CGameProfile*> m_GameProfiles;
    CGameProfile* m_activeProfile = nullptr;

    void mergeProfile(CGameProfile* profile);
    void initProfileRegistry(CGameManager* manager);
    void initModRegistry(CGameProfile* profile);
    void CollectJsonValues();
    bool isExistingProfile(const std::string& targetName);
    int getProfileIndex(const std::string& targetName);
    std::vector<std::string> getAllProfileNames();
    std::vector<CGameProfile*> getAllProfiles();

};
