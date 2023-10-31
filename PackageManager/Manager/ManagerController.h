/* This class is responsible for initializing all the necessary managers based on the provided JSON configuration. 
   This is a high-level orchestrator, creating instances of CGameManager
   and other related managers for each game defined in a specified JSON configuration */

#include "GameManager.h"
#include "../Utils/ConfigUtils.hpp"
#include <iostream>

using namespace ConfigUtils;
using JSON = nlohmann::json;

class CManagerController {

public:
    ~CManagerController() {}

    CManagerController(const char* path) {
        this->m_JsonPath = path;
        bool canAccessFile = isFileAccessable(this->m_JsonPath);

        if (!canAccessFile) {
            /* Perform first time setup on config */
            std::cout << "\nCannot access file.";
            InitializeJSON(this->m_RootJSON);
        }
        else { SetupExistingJSON(); }
    }


private:
    std::string m_JsonPath;
    JSON m_RootJSON;
    std::vector<CGameManager*> pGameManagers;
    int m_NumGames = 0;

    void SetupExistingJSON() {
        std::ifstream file(m_JsonPath);
        this->m_RootJSON = JSON::parse(file);
        file.close();
    }

    void InitializeJSON(JSON& jsonObj) {

        /* todo: call GUI first time setup widget */
        
        // Setup JSON template
        jsonObj["Mods Root path"] = "/Mods/";
        std::vector<std::string> jsonPaths = FindExistingConfigs();

        /* debug output */
        this->m_NumGames = jsonPaths.size();
        jsonObj["Number of Titles"] = this->m_NumGames;
        std::cout << "\nNumber of Titles: " << this->m_NumGames;

        /* Initialize valid game managers with specified jsons */
        for (const auto& path : jsonPaths) {
            CGameManager gameManager;
        }

        // Mod JSON template
        //jsonObj["Profile Name"] = "lorem ipsum";
        //jsonObj["Game Title"] = "lorem ipsum";
        //jsonObj["Game Path"] = "lorem ipsum";
        //jsonObj["Mods Path"] = "lorem ipsum";
        //jsonObj["Mods"] = { "lorem", "ipsum", "ipsum" };
        //jsonObj["Last Modified"] = "lorem ipsum";
    }

};