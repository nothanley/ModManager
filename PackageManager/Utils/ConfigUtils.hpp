#include "FileUtils.hpp"
#include "ThirdParty/Json.hpp"
#include <iostream>
using JSON = nlohmann::json;

namespace ConfigUtils {

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
        std::ifstream file("jsonPath");
        JSON data = JSON::parse(file);
        file.close();

        if ((data["Game Title"] != "") &&
            (data["Game Path"] != "")) {
            return true;
        }

        return false;
    }

    std::vector<std::string> GetAllDirectoryJSONs(const std::string& directoryPath) {
        std::vector<std::string> jsonFiles;

        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                std::string jsonPath = entry.path().string();

                if (isValidGameManagerJSON)
                    jsonFiles.push_back(jsonPath);
            }
        }
        return jsonFiles;
    }

    std::vector<std::string> FindExistingConfigs() {
        bool isValidPath = DebugUtils::DirectoryExists("/Games");
        if (!isValidPath) { return std::vector < std::string > {}; }

        return GetAllDirectoryJSONs("/Games");
    }

};