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

    static bool canWriteToFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::out | std::ios::app);

        if (file.is_open()) {
            file.close();
            return true;}

        return false;
    }

    static std::string toUpper( std::string input ) {
        for (auto& c : input) c = toupper(c);
        return input;
    }

    static bool isValidGameManagerJSON(const std::string& jsonPath) {

        /* Check if file is accessible */
        if (!isFileAccessable(jsonPath)) { return false; }

        /* Check if essential manager properties exist in JSON */
        std::ifstream file(jsonPath.c_str());
        bool test = file.good();

        /* Check if json properties are present */
        JSON data;
        try{ 
            data = JSON::parse(file); 
            if (data.at("game_path") != "" &&
                data.at("game_title") != ""){
                return true; }  }
        catch (...) {
            file.close();
            return false;  }
        
        file.close();
        return false;
    }

    static std::vector<std::string> GetAllDirectoryJSONs(const std::string& directory, const char* jsonName) {
        std::vector<std::string> jsonFiles;

        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) 
            if (entry.is_regular_file() && entry.path().filename() == jsonName) {
                std::string jsonPath = entry.path().string();

                if ( isValidGameManagerJSON(jsonPath) )
                    jsonFiles.push_back(jsonPath);
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

    static
    std::string extractFileName(const std::string& filepath) {
        size_t found = filepath.find_last_of("/\\");

        if (found != std::string::npos) {
            return filepath.substr(found + 1);
        } else {
            return filepath;
        }
    }

    static
    std::string extractFolderPath(const std::string& filepath) {
        size_t found = filepath.find_last_of("/\\");

        if (found != std::string::npos) {
            return filepath.substr(0, found + 1);
        } else {
            return "";
        }
    }

    static
    bool copyFile(const std::string& oldPath, const std::string& newPath) {
        std::ifstream sourceFile(oldPath, std::ios::binary);
        std::ofstream destinationFile(newPath, std::ios::binary);
        if (!sourceFile.is_open() || !destinationFile.is_open())
            return false;

        /* Copy binary contents*/
        destinationFile << sourceFile.rdbuf();

        /* Validate successful copy */
        if (sourceFile.fail() || destinationFile.fail())
            return false;
        sourceFile.close();
        destinationFile.close();
        return true;
    }

    static
    std::string unifySlashEncoding(const std::string& filePath) {
        std::string fixedPath = filePath;

        // Replace backslashes with forward slashes
        size_t found = fixedPath.find("\\");
        while (found != std::string::npos) {
            fixedPath.replace(found, 1, "/");
            found = fixedPath.find("\\", found + 1);
        }

        return fixedPath;
    }

    static bool
    removeDirectory(const std::string& dirPath) {
        try {
            std::filesystem::remove_all(dirPath);
            return true;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error removing directory: " << e.what() << std::endl;
            return false;
        }
    }


    static bool
    moveDirectory(const std::string& inDir, const std::string& outDir) {
        try {
            std::filesystem::rename(inDir, outDir);
            return true;
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error moving directory: " << e.what() << std::endl;
            return false;
        }
    }
};
