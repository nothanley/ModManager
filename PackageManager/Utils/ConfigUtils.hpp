#include "FileUtils.hpp"
#include "ThirdParty/Json.hpp"
#include <iostream>
using JSON = nlohmann::json;

namespace ConfigUtils {

    static void SetupExistingJSON( JSON& jsonObj, const std::string& jsonPath) {
        std::ifstream file(jsonPath);
        jsonObj = JSON::parse(file);
        file.close();
    }

    static bool isFileAccessable(const std::string& filePath) {
        HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            CloseHandle(hFile);
            return true;  // File has write access
        }
        return false;
    }

    static bool isValidGameManagerJSON(const std::string& jsonPath) {

        /* Check if file is accessible */
        if (!isFileAccessable(jsonPath)) { return false; }

        /* Check if essential manager properties exist in JSON */
        std::ifstream file(jsonPath.c_str());
        bool test = file.good();
        JSON data;
        try{ 
            data = JSON::parse(file); 
        }
        catch (...) {
            file.close();
            return false; 
        }
        
        file.close();

        if ((data["Game Title"] != "") &&
            (data["Game Root path"] != "")) {
            return true;
        }

        return false;
    }

    static std::vector<std::string> GetAllDirectoryJSONs(const std::string& directory, const char* jsonName) {
        std::vector<std::string> jsonFiles;

        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
            if (entry.is_regular_file() && entry.path().filename() == jsonName) {
                std::string jsonPath = entry.path().string();

                if ( isValidGameManagerJSON(jsonPath) )
                    jsonFiles.push_back(jsonPath);
            }
        }

        return jsonFiles;
    }

    static std::string ConvertToAbsolutePath(const char* relativePath) {
        std::filesystem::path fullPath = std::filesystem::absolute(relativePath);
        return fullPath.string();
    }

    static std::vector<std::string> FindExistingConfigs( const char* directory, const char* configName ) {
        bool isValidPath = DebugUtils::DirectoryExists(directory);
        if (!isValidPath) { return std::vector < std::string > {}; }

        return GetAllDirectoryJSONs(directory, configName);
    }

};