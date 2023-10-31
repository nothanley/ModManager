/* This class is responsible for initializing all the necessary managers based on the provided JSON configuration. 
   This is a high-level orchestrator, creating instances of CGameManager
   and other related managers for each game defined in a specified JSON configuration */

#include "GameManager.h"
#pragma once
using namespace ConfigUtils;

class CManagerController {

public:
    ~CManagerController() {}

    CManagerController(const char* path) {
        this->m_RootPath = path;
        if (!DebugUtils::DirectoryExists(this->m_RootPath)) { return;  }

        InitializeManagers();
        if (this->m_NumGames < 0) { SetupController(); }
    }

protected:
    std::string m_RootPath;
  
private:
    std::vector<CGameManager*> pGameManagers;
    int m_NumGames = 0;

    void SetupController() {
        //query GUI for values
    }

    void InitializeManagers() {

        // Setup JSON template
        std::string configPath = m_RootPath + "\\Games";
        std::vector<std::string> jsonPaths = FindExistingConfigs(configPath.c_str(),"game_config.json");

        /* Debug output */
        this->m_NumGames = jsonPaths.size();
        std::cout << "\nNumber of Titles: " << this->m_NumGames;

        /* Initialize valid game managers with specified jsons */
        for (const auto& path : jsonPaths) {
            CGameManager* gameManager = new CGameManager( path.c_str() );
            this->pGameManagers.push_back(gameManager);
        }

    }

};