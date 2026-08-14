#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>

class FileManager {
public:
    void displayFileInfo(const std::string& filepath) const;
    void changePermissions(const std::string& filepath, const std::string& octalModeStr) const;
};

#endif // FILE_MANAGER_H