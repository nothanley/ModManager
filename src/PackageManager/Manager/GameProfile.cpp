#include "GameProfile.h"
using namespace ConfigUtils;


std::string 
CGameProfile::getName() {
    return this->m_ProfileName;
}

void 
CGameProfile::setName(const char* name) {
    this->m_ProfileName = name;
}

std::vector<CGamePackage*> 
CGameProfile::getAllMods() {
    return this->m_PackageLoadOrder;
}

void 
CGameProfile::addToRegistry(CGamePackage* contents, int index) {
    //checkExistingItem(contents, index);
    m_PackageLoadOrder.at(index) = contents;
}

void 
CGameProfile::addToRegistry(CGamePackage* contents) {
    int index = getIndex(contents->getName());
    if (index != -1) {
        /* If this mod exists already, replace current with new */
        m_PackageLoadOrder[index] = contents;
        return;
    }

    this->m_PackageLoadOrder.push_back(contents);
}

unsigned int 
CGameProfile::getModCount() {
    return this->m_PackageLoadOrder.size();
}

void
CGameProfile::setRegistry(const std::vector<CGamePackage*> registry){
    this->m_PackageLoadOrder = registry;
}

CGamePackage*
CGameProfile::getMod(const std::string& name){
    int index = this->getIndex(name);
    if (index == -1) return nullptr;

    return m_PackageLoadOrder[index];
}

int 
CGameProfile::getIndex(const std::string& name) {
    for (int i = 0; i < this->getModCount(); i++) {
        CGamePackage* mod = this->getAllMods()[i];
        if (mod != nullptr)
            if (toUpper(mod->getName()) == toUpper(name))
                return i;
    }
    return -1;
}

void 
CGameProfile::saveToJson(JSON* json) {
    *json = saveModRegistryToJson(*json);
}

void 
CGameProfile::setModCount(int size) {
    this->m_PackageLoadOrder.resize(size);
}

JSON 
CGameProfile::saveModRegistryToJson(JSON json) {
    json[m_ProfileName]["mod_registry"] = this->getAllModNames();
    for (const auto& mod : this->m_PackageLoadOrder) {
        json[m_ProfileName][mod->getName()]["path"] = mod->getJsonPath();
        json[m_ProfileName][mod->getName()]["status"] = mod->getStatus();
        json[m_ProfileName][mod->getName()]["load_index"] = getIndex(mod->getName());
    }
    return json;
}

std::vector<std::string> 
CGameProfile::getAllModNames() {
    std::vector<std::string> modNames;
    for (const auto& mod : this->m_PackageLoadOrder) {
        modNames.push_back(mod->getName());
    }
    return modNames;
}


void 
CGameProfile::checkExistingItem(CGamePackage* contents, int index) {
    int check = getIndex(contents->getName());
    if (check != -1) {
        /* If this mod exists already, move old to new index */
        moveItem(check, index);
        return;
    }
}

void 
CGameProfile::moveItem(int oldIndex, int newIndex) {
    /* Copy item to new index */
    CGamePackage* sourceItem = m_PackageLoadOrder.at(oldIndex);
    m_PackageLoadOrder.insert(m_PackageLoadOrder.begin() + newIndex, sourceItem);

    /* Erase old value from load list */
    m_PackageLoadOrder.erase(m_PackageLoadOrder.begin() + oldIndex);
}

void
CGameProfile::removeFromRegistry(int index){
    CGamePackage* sourceItem = m_PackageLoadOrder.at(index);
    /* todo: check for other dependencies before deleting contents */
    sourceItem->deleteContents();
    delete sourceItem;
    this->m_PackageLoadOrder.erase(m_PackageLoadOrder.begin() + index);
}

void
CGameProfile::removeFromRegistry(CGamePackage* mod){
    int index = getIndex(mod->getName());
    if (index == -1){
        std::cerr << "\nRequested mod deletion doesn't exist in registry.";
        return; }

    removeFromRegistry(index);
}












