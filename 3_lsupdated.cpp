#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>
#include <vector>
#include <algorithm> 
#include "main.cpp"

// Function to print file details
void print_file_details(const char *filename, const struct stat &file_stat) {
    // Print file type
    if (S_ISREG(file_stat.st_mode)) {
        std::cout << "-";
    } else if (S_ISDIR(file_stat.st_mode)) {
        std::cout << "d";
    } else if (S_ISLNK(file_stat.st_mode)) {
        std::cout << "l";
    } else {
        std::cout << "?";
    }

    // Print permissions
    const char *permissions = "rwxrwxrwx";
    for (int i = 0; i < 9; i++) {
        if ((file_stat.st_mode >> (8 - i)) & 1) {
            std::cout << permissions[i];
        } else {
            std::cout << "-";
        }
    }

    // Print owner and group
    struct passwd *owner_info = getpwuid(file_stat.st_uid);
    struct group *group_info = getgrgid(file_stat.st_gid);
    if (owner_info != nullptr) {
        std::cout << " " << owner_info->pw_name;
    } else {
        std::cout << " " << file_stat.st_uid;
    }
    if (group_info != nullptr) {
        std::cout << " " << group_info->gr_name;
    } else {
        std::cout << " " << file_stat.st_gid;
    }

    // Print file size
    std::cout << " " << file_stat.st_size;

    // Print last modification time
    char time_buffer[80];
    strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", localtime(&file_stat.st_mtime));
    std::cout << " " << time_buffer;

    // Print filename
    std::cout << " " << filename << std::endl;
}

int main(int argc, char *argv[]) {
    const char *dir_path = ".";
    int flags = 0; // Flags for -a and -l

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            flags |= 1; // Set the first bit for -a
        } else if (strcmp(argv[i], "-l") == 0) {
            flags |= 2; // Set the second bit for -l
        } else if (argv[i][0] != '-') {
            dir_path = argv[i]; // The argument is a directory name
        }
    }

    // Open the directory
    DIR *dir = opendir(dir_path);
    if (dir == nullptr) {
        perror("opendir");
        return 1;
    }

    // Read directory entries
    struct dirent *entry;
    std::vector<std::string> filenames;
    while ((entry = readdir(dir)) != nullptr) {
        if (!(flags & 1) && entry->d_name[0] == '.') {
            continue; // Skip hidden files if -a flag is not set
        }
        filenames.push_back(entry->d_name);
    }

    // Sort filenames alphabetically
    std::sort(filenames.begin(), filenames.end());

    // Display file details if -l flag is set, otherwise just print filenames
    if (flags & 2) {
        for (const std::string &filename : filenames) {
            struct stat file_stat;
            std::string full_path = std::string(dir_path) + "/" + filename;
            if (lstat(full_path.c_str(), &file_stat) == 0) {
                print_file_details(filename.c_str(), file_stat);
            } else {
                perror("lstat");
            }
        }
    } else {
        for (const std::string &filename : filenames) {
            std::cout << filename << " ";
        }
        std::cout << std::endl;
    }

    closedir(dir);
    return 0;
}
