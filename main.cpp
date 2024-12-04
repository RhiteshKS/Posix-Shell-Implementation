#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "6_search.cpp"
#include "cd_echo_pwd.cpp"
#include "5_pinfo.cpp"

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

void print_prompt() {
    char hostname[128];
    gethostname(hostname, sizeof(hostname));
    char cwd[256];
    char* uname = 
    getcwd(cwd, sizeof(cwd));
    char* username = getenv("USER");
    printf("%s@%s:%s> ", username, hostname, cwd);
}

int main() {
    char input[MAX_INPUT_SIZE];
    while (1) {
        print_prompt();
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit on Ctrl+D or EOF
        }

        // Tokenize input
        char* args[MAX_ARGS];
        int arg_count = 0;
        char* token = strtok(input, " \t\n");
        while (token != NULL) {
            args[arg_count] = token;
            arg_count++;
            token = strtok(NULL, " \t\n");
        }
        args[arg_count] = NULL;

        if (arg_count == 0) {
            continue; // Empty command
        }
        //search
        if (strncmp(input, "search", 6) == 0) {
            char *filename = strtok(input + 7, "\n");  // Extract the filename to search
            if (filename != NULL) {
                if (search_file(filename, ".")) {
                    printf("True\n");
                    continue;
                } else {
                    printf("False\n");
                    continue;
                }
            }
        }
        get_shell_process_info();
        if (input == "exit") {
            break;
        } else if (input == "pwd") {
            pwd();
        } else {
            // Split the input into command and argument
            std::vector<std::string> parts = split(input, ' ');
            std::string cmd = parts[0];
            std::string arg = (parts.size() > 1) ? parts[1] : "";

            if (cmd == "cd") {
                cd(arg);
            } else if (cmd == "echo") {
                echo(arg);
            } else {
                std::cerr << "Command not found: " << cmd << std::endl;
            }
        }
        // Check for background process

        int background = 0;
        if (strcmp(args[arg_count - 1], "&") == 0) {
            background = 1;
            args[arg_count - 1] = NULL;
        }

        // Handle built-in commands
        if (strcmp(args[0], "cd") == 0) {
            if (arg_count > 1) {
                if (chdir(args[1]) != 0) {
                    perror("cd");
                }
            } else {
                // Change to home directory
                chdir(getenv("HOME"));
            }
        } else if (strcmp(args[0], "exit") == 0) {
            exit(0);
        } else {
            // Fork and execute external command
            pid_t pid = fork();
            if (pid == 0) {
                // Child process
                execvp(args[0], args);
                perror("execvp");
                exit(1);
            } else if (pid > 0) {
                // Parent process
                if (!background) {
                    waitpid(pid, NULL, 0); // Wait for foreground process
                }
            } else {
                perror("fork");
            }
        }
    }

    return 0;
}
