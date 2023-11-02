/* This class is responsible for initializing all the necessary managers based on the provided JSON configuration. 
   This is a high-level orchestrator, creating instances of CGameManager
   and other related managers for each game defined in a specified JSON configuration */

#include "GameManager.h"
#pragma once
using namespace ConfigUtils;

class CManagerController {

public:
    int m_NumGames = 0;
    ~CManagerController() {
        for (auto game:this->pGameManagers)
            delete game;
    }

    CManagerController(const char* path) {
        this->m_RootPath = path;
        if (!DebugUtils::DirectoryExists(this->m_RootPath)) { return;  }

        InitializeManagers();
        if (this->m_NumGames < 0) { SetupController(); }
    }

    CGameManager* GetGameManager(const char* gameTitle){
        for (const auto& manager : this->pGameManagers ){
            if (manager->m_gameTitle == gameTitle)
                return manager;
        }
        return nullptr;
    }

protected:
    std::string m_RootPath;
  
private:
    std::vector<CGameManager*> pGameManagers;

    void InitializeManagers() {

        // Setup JSON template
        std::string configPath = m_RootPath + "\\Games";
        std::vector<std::string> jsonPaths = FindExistingConfigs(configPath.c_str(),"game_config.json");

        this->m_NumGames = jsonPaths.size();
//        std::cout << "\nNumber of Titles: " << this->m_NumGames; /* Debug output */

        /* Initialize valid game managers with specified jsons */
        for (const auto& path : jsonPaths) {
            CGameManager* gameManager = new CGameManager( path.c_str() );
            this->pGameManagers.push_back(gameManager);
        }

    }

    void SetupController() {}
};
