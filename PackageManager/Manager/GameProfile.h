#include "GamePackage.h"
#pragma once
using namespace ConfigUtils;

class CGameManager;
class CGameProfile {

public:
    CGameProfile(const char* name) {
        this->setName(name);
    }

    CGameProfile(const JSON* parentJSON, const char* name) {
        this->setName(name);
    }

    void setName(const char* name);
    void saveToJson(JSON* json);
    void addToRegistry(CGamePackage* contents, int index);
    void addToRegistry(CGamePackage* contents);
    unsigned int getModCount();
    int getIndex(const std::string& name);
    std::vector<CGamePackage*> getAllMods();
    std::string getName();

protected:
    std::vector<CGamePackage*> m_PackageLoadOrder = {};
    void setModCount(int size);

private:
    std::string m_ProfileName;
    std::vector<std::string> m_ModPaths;

    JSON saveModRegistryToJson(JSON json);
    std::vector<std::string> getAllModNames();
    void checkExistingItem(CGamePackage* contents, int index);
    void moveItem(int oldIndex, int newIndex);

    friend CGameManager;
};
