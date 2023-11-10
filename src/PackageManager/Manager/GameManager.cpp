#include "GameManager.h"
using namespace ConfigUtils;

CGameManager::CGameManager() {
    this->m_JsonPath = "";
}

CGameManager::CGameManager(const char* path) {
    this->m_JsonPath = path;
    bool canAccessFile = isFileAccessable(this->m_JsonPath);
    if (!canAccessFile) { return; }

    try { CollectJsonValues(); }
    catch (...) {
        throw std::runtime_error("Invalid Manager Configuration.");
    }
}

std::vector<std::string> 
CGameManager::getAllProfileNames() {
    std::vector<std::string> profileNames;
    for (const auto& profile : this->m_GameProfiles)
        profileNames.push_back(profile->getName());
    return profileNames;
}

CGameProfile* 
CGameManager::getActiveProfile() {
    return this->m_activeProfile;
}

void
CGameManager::setJsonPath(const std::string& path) {
    if (!canWriteToFile(path + "/game_config.json")) {
        throw std::runtime_error("JSON configuration is inaccessible.");    }

    this->m_JsonPath = path + "/game_config.json";
}

void 
CGameManager::setGameName(const char* name) {
    this->m_gameTitle = name;
}

std::string 
CGameManager::getGameName() {
    return this->m_gameTitle;
}


unsigned int 
CGameManager::getProfileCount() {
    return this->m_GameProfiles.size();
}

bool 
CGameManager::hasActiveProfile() {
    return (this->m_activeProfile);
}

CGameProfile* 
CGameManager::getProfile(const char* name) {
    CGameProfile* result = nullptr;

    for (auto& profile : this->m_GameProfiles)
        if (toUpper(profile->getName()) == toUpper(name))
            return profile;

    return result;
}

std::string 
CGameManager::getGamePath() {
    return this->m_rootGamePath;
}

std::string
CGameManager::getJsonPath() {
    return this->m_JsonPath;
}

void 
CGameManager::setGamePath(const char* path) {
    this->m_rootGamePath = path;
}

void 
CGameManager::setActiveProfile(CGameProfile* profile) {
    mergeProfile(profile);
    this->m_activeProfile = profile;
}

void 
CGameManager::addProfile(CGameProfile* profile, bool setActive) {
    mergeProfile(profile);
    if (setActive) {
        this->m_activeProfile = profile;
    }
}

void 
CGameManager::createProfile(const char* name, bool setActive) {
    CGameProfile* profile = new CGameProfile(name);
    mergeProfile(profile);
    if (setActive) {
        this->m_activeProfile = profile;
    }
}

void 
CGameManager::replaceProfile(CGameProfile* profile, const std::string& target) {
    int replaceIndex = getProfileIndex(target);
    if (replaceIndex != -1)
        this->m_GameProfiles[replaceIndex] = profile;
}

void 
CGameManager::updateJSON() {
    /* Update's member json with full updated+populated values */
    this->m_ManagerJson.clear();
    this->m_ManagerJson["game_title"] = getGameName();
    this->m_ManagerJson["active_profile"] = getActiveProfile()->getName();
    this->m_ManagerJson["game_path"] = getGamePath();
    this->m_ManagerJson["profile_registry"] = this->getAllProfileNames();

    for (const auto& profile : getAllProfiles()) {
        profile->saveToJson(&m_ManagerJson);
    }
}

void 
CGameManager::save() {
    if (!canWriteToFile(this->m_JsonPath)) {
        throw std::runtime_error("Cannot write to file.");
    }

    std::ofstream file(m_JsonPath, std::ios::out);
    this->updateJSON();
    file << std::setw(4) << this->m_ManagerJson;
    file.close();
}

std::vector<CGameProfile*> 
CGameManager::getAllProfiles() {
    return this->m_GameProfiles;
}

void 
CGameManager::mergeProfile(CGameProfile* profile) {
    /* Pushes back if the target profile
    exists else replace profile with matching title */

    std::string targetName = profile->getName();

    if (isExistingProfile(targetName)) {
        this->replaceProfile(profile, targetName);
    }
    else {
        this->m_GameProfiles.push_back(profile);
    }
}

bool 
CGameManager::isExistingProfile(const std::string& targetName) {
    return this->getProfileIndex(targetName) != -1;
}

std::string
CGameManager::getPath(){
    std::string folderPath = ConfigUtils::extractFolderPath(this->m_JsonPath);
    return ConfigUtils::unifySlashEncoding(folderPath);
}

int 
CGameManager::getProfileIndex(const std::string& targetName) {
    for (int idx = 0; idx < this->getProfileCount(); idx++)
    {
        CGameProfile* profile = m_GameProfiles[idx];
        if (toUpper(profile->getName()) == toUpper(targetName))
            return idx;
    }

    return -1;
}

void 
CGameManager::initProfileRegistry(CGameManager* manager) {
    std::vector<std::string> profileRegistry = manager->m_ManagerJson["profile_registry"];
    std::string activeProfileName = manager->m_ManagerJson["active_profile"];

    for (const auto& profileName : profileRegistry) {
        CGameProfile* profile = new CGameProfile(profileName.c_str());
        initModRegistry(profile);
        manager->addProfile(profile);

        if (activeProfileName == profileName)
            setActiveProfile(profile);
    }
}

void 
CGameManager::initModRegistry(CGameProfile* profile) {
    std::string profileName = profile->getName();
    std::vector<std::string> modRegistry = m_ManagerJson[profileName]["mod_registry"];
    profile->setModCount(modRegistry.size());

    for (const auto& modName : modRegistry) {
        std::string modJsonPath = m_ManagerJson[profileName][modName]["path"];

        try {
            /* Initializes mod item with specified JSON path */
            CGamePackage* mod = new CGamePackage(modJsonPath.c_str());
            int loadIndex = m_ManagerJson[profileName][modName]["load_index"];
            mod->setEnabled(m_ManagerJson[profileName][modName]["status"]);
            profile->addToRegistry(mod, loadIndex);
        }
        catch (...) {
            printf("Failed to load mod configuration: %s", modJsonPath.c_str());
        }
    }
}

void 
CGameManager::CollectJsonValues() {
    SetupExistingJSON(m_ManagerJson, m_JsonPath);

    m_rootGamePath = this->m_ManagerJson["game_path"];
    m_gameTitle = this->m_ManagerJson["game_title"];
    initProfileRegistry(this);
}
