#include "../Utils/ConfigUtils.hpp"

class CGameManager {

public:

    /* todo: setup json properties + profiles */

    CGameManager(const char* path) {
        this->m_JsonPath = path;
        bool canAccessFile = isFileAccessable(this->m_JsonPath);

        if (canAccessFile) {
            /* Perform first time setup on config */
            std::cout << "\nCannot access submanager json.";
            CollectJsonValues();
        }
    }

private:
    std::string m_JsonPath;
    JSON m_ManagerJSON;


    void CollectJsonValues() {

    }

};