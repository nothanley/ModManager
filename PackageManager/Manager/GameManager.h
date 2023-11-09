#include "GameProfile.h"
#pragma once
using namespace ConfigUtils;

class CGameManager {

public:

    CGameManager() {
        this->m_JsonPath = "";
    }

    CGameManager(const char* path) {
        this->m_JsonPath = path;
        bool canAccessFile = isFileAccessable(this->m_JsonPath);
        if (!canAccessFile) { return; }

        try { CollectJsonValues(); }
        catch (...) {
            throw std::runtime_error("Invalid Manager Configuration."); }
    }

    CGameProfile* getActiveProfile() {
        return this->m_activeProfile;
    }

    void setJsonPath(const std::string& path) {
        if (!canWriteToFile(path+"/game_config.json")) {
            throw std::runtime_error("JSON configuration is inaccessible."); }

        this->m_JsonPath = path+"/game_config.json";
    }

    void setGameName(const char* name) {
        this->m_gameTitle = name;
    }

    std::string getGameName(){
        return this->m_gameTitle;
    }

    unsigned int getProfileCount(){
        return this->m_GameProfiles.size();
    }

    bool hasActiveProfile(){
        return (this->m_activeProfile);
    }

    CGameProfile* getProfile(const char* name) {
        CGameProfile* result = nullptr;

        for (auto& profile : this->m_GameProfiles)
            if (toUpper(profile->getName()) == toUpper(name))
                return profile;

        return result;
    }

    std::string getGamePath() {
        return this->m_rootGamePath;
    }

    void setGamePath(const char* path) {
        this->m_rootGamePath = path;
    }

    void setActiveProfile( CGameProfile* profile ){
        mergeProfile(profile);
        this->m_activeProfile = profile;
    }

    void addProfile(CGameProfile* profile, bool setActive = false) {
        mergeProfile(profile);
        if (setActive){
            this->m_activeProfile = profile;}
    }

    void createProfile(const char* name, bool setActive=false){
        CGameProfile* profile = new CGameProfile(name);
        mergeProfile(profile);
        if (setActive) { 
            this->m_activeProfile = profile; }
    }

    void replaceProfile(CGameProfile* profile, const std::string& target ) {
        int replaceIndex = getProfileIndex(target);
        if (replaceIndex != -1)
            this->m_GameProfiles[replaceIndex] = profile;
    }

    void updateJSON() {
        /* Update's member json with full updated+populated values */
        this->m_ManagerJson.clear();
        this->m_ManagerJson["game_title"] = getGameName();
        this->m_ManagerJson["active_profile"] = getActiveProfile()->getName();
        this->m_ManagerJson["game_path"] = getGamePath();
        this->m_ManagerJson["profile_registry"] = this->getAllProfileNames();

        for (const auto& profile : getAllProfiles()) {
            profile->saveToJson(&m_ManagerJson); }
    }

    void save() {
        if (!canWriteToFile(this->m_JsonPath)) {
            throw std::runtime_error("Cannot write to file.");}

        std::ofstream file(m_JsonPath, std::ios::out);
        this->updateJSON();
        file << std::setw(4) << this->m_ManagerJson;
        file.close();
    }

protected:
    std::string m_gameTitle;
    std::string m_JsonPath;
    JSON m_ManagerJson;

private:
    std::string m_rootGamePath = "unknown";
    std::vector<CGameProfile*> m_GameProfiles;
    CGameProfile* m_activeProfile = nullptr;

    std::vector<std::string> getAllProfileNames() {
        std::vector<std::string> profileNames;
        for (const auto& profile : this->m_GameProfiles)
            profileNames.push_back(profile->getName());
        return profileNames;
    }


    std::vector<CGameProfile*> getAllProfiles() {
        return this->m_GameProfiles;
    }

    void mergeProfile(CGameProfile* profile) {
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

    bool isExistingProfile(const std::string& targetName) {
        return this->getProfileIndex(targetName) != -1;
    }

    int getProfileIndex(const std::string& targetName){
        for (int idx = 0; idx < this->getProfileCount(); idx++)
        {
            CGameProfile* profile = m_GameProfiles[idx];
            if ( toUpper(profile->getName()) == toUpper(targetName) )
                return idx;
        }

        return -1;
    }

    void initProfileRegistry(CGameManager* manager) {
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

    void initModRegistry(CGameProfile* profile) {
        std::string profileName = profile->getName();
        std::vector<std::string> modRegistry = m_ManagerJson[profileName]["mod_registry"];

        profile->setModCount(modRegistry.size());

        for (const auto& modName : modRegistry) {
            CGamePackage* mod = new CGamePackage(modName.c_str(),"Character","Desktop");
            int loadIndex = m_ManagerJson[profileName][modName]["load_index"];
            mod->setEnabled(m_ManagerJson[profileName][modName]["status"]);
            mod->setAssetPath(m_ManagerJson[profileName][modName]["path"]);
            profile->addToRegistry(mod, loadIndex);
        }
    }

    void CollectJsonValues() {
        SetupExistingJSON(m_ManagerJson,m_JsonPath);

        m_rootGamePath = this->m_ManagerJson["game_path"];
        m_gameTitle = this->m_ManagerJson["game_title"];
        initProfileRegistry(this);
    }

};
