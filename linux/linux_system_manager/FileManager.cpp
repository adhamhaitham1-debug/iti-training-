#include "FileManager.h"
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <iomanip>

void FileManager::displayFileInfo(const std::string& filepath) const {
    struct stat fileStat;
    if (stat(filepath.c_str(), &fileStat) < 0) {
        perror("Error inspecting file");
        return;
    }

    std::cout << "\n========================================\n";
    std::cout << "           FILE INFORMATION             \n";
    std::cout << "========================================\n";
    std::cout << "File Path    : " << filepath << "\n";
    std::cout << "File Size    : " << fileStat.st_size << " bytes\n";

    // File Type
    std::cout << "File Type    : ";
    if (S_ISREG(fileStat.st_mode))       std::cout << "Regular File\n";
    else if (S_ISDIR(fileStat.st_mode))  std::cout << "Directory\n";
    else if (S_ISLNK(fileStat.st_mode))  std::cout << "Symbolic Link\n";
    else if (S_ISCHR(fileStat.st_mode))  std::cout << "Character Device\n";
    else if (S_ISBLK(fileStat.st_mode))  std::cout << "Block Device\n";
    else if (S_ISFIFO(fileStat.st_mode)) std::cout << "FIFO / Pipe\n";
    else if (S_ISSOCK(fileStat.st_mode)) std::cout << "Socket\n";
    else                                 std::cout << "Unknown\n";

    // Owner & Group lookup
    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group  *gr = getgrgid(fileStat.st_gid);

    std::cout << "Owner UID    : " << fileStat.st_uid << " (" << (pw ? pw->pw_name : "unknown") << ")\n";
    std::cout << "Group GID    : " << fileStat.st_gid << " (" << (gr ? gr->gr_name : "unknown") << ")\n";

    // Permissions string representation (e.g. rwxr-xr-x)
    std::string perms = "";
    perms += (S_ISDIR(fileStat.st_mode))  ? 'd' : '-';
    perms += (fileStat.st_mode & S_IRUSR) ? 'r' : '-';
    perms += (fileStat.st_mode & S_IWUSR) ? 'w' : '-';
    perms += (fileStat.st_mode & S_IXUSR) ? 'x' : '-';
    perms += (fileStat.st_mode & S_IRGRP) ? 'r' : '-';
    perms += (fileStat.st_mode & S_IWGRP) ? 'w' : '-';
    perms += (fileStat.st_mode & S_IXGRP) ? 'x' : '-';
    perms += (fileStat.st_mode & S_IROTH) ? 'r' : '-';
    perms += (fileStat.st_mode & S_IWOTH) ? 'w' : '-';
    perms += (fileStat.st_mode & S_IXOTH) ? 'x' : '-';

    // Octal representation calculation
    int octalPerms = fileStat.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);

    std::cout << "Permissions  : " << perms << " (0" << std::oct << octalPerms << std::dec << ")\n";
}

void FileManager::changePermissions(const std::string& filepath, const std::string& octalModeStr) const {
    try {
        // Convert octal string (e.g. "755") to octal integer base 8
        mode_t mode = std::stoi(octalModeStr, nullptr, 8);

        if (chmod(filepath.c_str(), mode) < 0) {
            perror("chmod failed");
        } else {
            std::cout << "Successfully changed permissions of '" << filepath << "' to 0" << octalModeStr << ".\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Invalid permission format! Please enter a 3-digit octal number (e.g., 755, 644, 700).\n";
    }
}