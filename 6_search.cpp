#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
//#include "main.cpp"
#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

// void print_prompt() {
//     char hostname[128];
//     gethostname(hostname, sizeof(hostname));
//     char cwd[256];
//     getcwd(cwd, sizeof(cwd));
//     char* username = getenv("USER");
//     printf("%s@%s:%s> ", username, hostname, cwd);
// }

int search_file(const char *filename, const char *directory) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(directory)) == NULL) {
        perror("opendir");
        return 0;  // Cannot open directory
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, filename) == 0) {
            closedir(dir);
            return 1;  // File found
        }

        // Check if the entry is a subdirectory (but not . or ..)
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char subdir_path[MAX_INPUT_SIZE];
            snprintf(subdir_path, sizeof(subdir_path), "%s/%s", directory, entry->d_name);

            if (search_file(filename, subdir_path)) {
                closedir(dir);
                return 1;  // File found in subdirectory
            }
        }
    }

    closedir(dir);
    return 0;  // File not found
}



// int main() {
//     char input[MAX_INPUT_SIZE];
//     while (1) {
//         print_prompt();
//         if (fgets(input, sizeof(input), stdin) == NULL) {
//             break; // Exit on Ctrl+D or EOF
//         }

//         // Tokenize input and handle other commands (cd, exit, etc.) as before
//         // Tokenize input
//         char* args[MAX_ARGS];
//         int arg_count = 0;
//         char* token = strtok(input, " \t\n");
//         while (token != NULL) {
//             args[arg_count] = token;
//             arg_count++;
//             token = strtok(NULL, " \t\n");
//         }
//         args[arg_count] = NULL;

//         if (arg_count == 0) {
//             continue; // Empty command
//         }

//         //New command: search
//         if (strncmp(input, "search", 6) == 0) {
//             char *filename = strtok(input + 7, "\n");  // Extract the filename to search
//             if (filename != NULL) {
//                 if (search_file(filename, ".")) {
//                     printf("True\n");
//                 } else {
//                     printf("False\n");
//                 }
//             }
//         }

//         // ...rest of your shell code...
//     }

//     return 0;
// }
