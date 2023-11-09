/* This class is responsible for initializing all the necessary managers based on the provided JSON configuration. 
   This is a high-level orchestrator, creating instances of CGameManager
   and other related managers for each game defined in a specified JSON configuration */

#include "GameManager.h"
#pragma once
using namespace ConfigUtils;

class CManagerController {

public:
    ~CManagerController() {
        for (auto game:this->pGameManagers)
            delete game;
    }

    CManagerController(const char* path) {
        this->m_RootPath = path;
        if (!DebugUtils::DirectoryExists(this->m_RootPath)) { return;  }

        InitializeManagers();
    }

    CGameManager* getManager(const char* gameTitle){
        for (const auto& manager : this->pGameManagers ){
            if (manager->getGameName() == gameTitle)
                return manager;
        }
        return nullptr;
    }

    int getGameCount() {
        return this->pGameManagers.size();
    }

    void addManager(CGameManager* manager) {
        this->pGameManagers.push_back(manager);
    }


    CGameManager* createManager(const char* gameName) {
        if (isExistingManager(gameName)) { return getManager(gameName); }
        CGameManager* manager = new CGameManager();
        std::string managerPath = (m_RootPath + "/Games/" + gameName);

        manager->setJsonPath( managerPath.c_str() );
        manager->setGameName(gameName);
        manager->createProfile( "Default", true );

        addManager(manager);
        return manager;
    }

protected:
    std::string m_RootPath;
  
private:
    std::vector<CGameManager*> pGameManagers;

    bool isExistingManager( std::string name) {
        for (const auto& manager : this->pGameManagers)
            if (toUpper(name) == toUpper(manager->getGameName()))
                return true;

        return false;
    }

    void InitializeManagers() {

        // Setup JSON template
        std::string configPath = m_RootPath + "/Games";
        std::vector<std::string> jsonPaths = FindExistingConfigs(configPath.c_str(),"game_config.json");

        /* Initialize valid game managers with specified jsons */
        for (const auto& path : jsonPaths) {
            try
            {
                CGameManager* gameManager = new CGameManager(path.c_str());
                this->pGameManagers.push_back(gameManager);
            }
            catch (...) { std::cout << "invalid json"; }
        }

    }
};
