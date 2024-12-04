#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

int change_directory(const char *path) {
    if (path == nullptr || path[0] == '\0') {
        // No argument provided, change to the home directory
        const char *home_dir = getenv("HOME");
        if (home_dir != nullptr) {
            if (chdir(home_dir) != 0) {
                perror("cd");
            }
        }
    } else if (strcmp(path, "-") == 0) {
        // Change to the previous directory
        const char *previous_dir = getenv("OLDPWD");
        if (previous_dir != nullptr) {
            if (chdir(previous_dir) != 0) {
                perror("cd");
            }
        }
    } else if (strcmp(path, ".") == 0) {
        // Do nothing (current directory)
    } else if (strcmp(path, "..") == 0) {
        // Move to the parent directory
        if (chdir("..") != 0) {
            perror("cd");
        }
    } else if (strcmp(path, "~") == 0) {
        // Change to the home directory
        const char *home_dir = getenv("HOME");
        if (home_dir != nullptr) {
            if (chdir(home_dir) != 0) {
                perror("cd");
            }
        }
    } else {
        // Change to the specified directory
        if (chdir(path) != 0) {
            perror("cd");
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <directory>" << std::endl;
        return 1;
    }
    
    const char *path = argv[1];
    return change_directory(path);
}
