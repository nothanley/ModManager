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
    CManagerController(const char* path);

    int getGameCount();
    void addManager(CGameManager* manager);
    CGameManager* createManager(const char* gameName); /* Creates and adds new manager given specified tag */
    CGameManager* getManager(const char* gameTitle);

protected:
    std::string m_RootPath;
  
private:
    std::vector<CGameManager*> pGameManagers;
    bool isExistingManager(std::string name);
    void InitializeManagers();

};
