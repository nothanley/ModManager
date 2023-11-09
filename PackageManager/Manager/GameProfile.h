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

    std::string getName(){
        return this->m_ProfileName;
    }

    void setName(const char* name){
        this->m_ProfileName = name;
    }

    std::vector<CGamePackage*> getAllMods(){
        return this->m_PackageLoadOrder;
    }

    void addToRegistry(CGamePackage* contents, int index) {
        //checkExistingItem(contents, index);
        m_PackageLoadOrder.at(index) = contents;
    }

    void addToRegistry(CGamePackage* contents){
        int index = getIndex(contents->getName());
        if (index != -1) {
            /* If this mod exists already, replace current with new */
            m_PackageLoadOrder[index] = contents;
            return;
        }

        this->m_PackageLoadOrder.push_back(contents);
    }

    unsigned int getModCount(){
        return this->m_PackageLoadOrder.size();
    }

    int getIndex(const std::string& name) {
        for (int i = 0; i < this->getModCount(); i++) {
            CGamePackage* mod = this->getAllMods()[i];
            if (mod != nullptr)
                if (toUpper(mod->getName()) == toUpper(name))
                    return i;   }
        return -1;
    }

    void saveToJson(JSON* json) {
        // todo: add more properties
        *json = saveModRegistryToJson(*json);
    }

protected:
    std::vector<CGamePackage*> m_PackageLoadOrder = {};

    void setModCount(int size) {
        this->m_PackageLoadOrder.resize(size);
    }

private:
    std::string m_ProfileName;
    std::vector<std::string> m_ModPaths;

    JSON saveModRegistryToJson(JSON json) {
        json[m_ProfileName]["mod_registry"] = this->getAllModNames();
        for (const auto& mod : this->m_PackageLoadOrder) {
            json[m_ProfileName][mod->getName()]["path"] = mod->getPath();
            json[m_ProfileName][mod->getName()]["status"] = mod->getStatus();
            json[m_ProfileName][mod->getName()]["load_index"] = getIndex(mod->getName()); }
        return json;
    }

    std::vector<std::string> getAllModNames() {
        std::vector<std::string> modNames;
        for (const auto& mod : this->m_PackageLoadOrder) {
            modNames.push_back(mod->getName());
        }
        return modNames;
    }


    void checkExistingItem(CGamePackage* contents, int index) {
        int check = getIndex(contents->getName());
        if (check != -1) {
            /* If this mod exists already, move old to new index */
            moveItem(check, index);
            return;
        }
    }

    void moveItem(int oldIndex, int newIndex) {
        /* Copy item to new index */
        CGamePackage* sourceItem = m_PackageLoadOrder.at(oldIndex);
        m_PackageLoadOrder.insert(m_PackageLoadOrder.begin() + newIndex, sourceItem);

        /* Erase old value from load list */
        m_PackageLoadOrder.erase(m_PackageLoadOrder.begin() + oldIndex);
    }

    friend CGameManager;
};
