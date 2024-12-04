#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <csignal>
#include <algorithm> 

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

// Function to execute a command in the foreground
void execute_foreground(const char *command, char *const arguments[]) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child process
        if (execvp(command, arguments) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process (shell)
        waitpid(pid, &status, 0);
    }
}

// Function to execute a command in the background
void execute_background(const char *command, char *const arguments[], std::vector<pid_t> &background_pids) {
    pid_t pid;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child process
        if (execvp(command, arguments) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process (shell)
        background_pids.push_back(pid);
        std::cout << "[" << pid << "] Background process started." << std::endl;
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    std::vector<pid_t> background_pids;

    while (1) {
        std::cout << "YourShell> ";
        if (fgets(input, sizeof(input), stdin) == nullptr) {
            break; // Exit on Ctrl+D or EOF
        }

        // Tokenize input and handle other built-in commands (ls, echo, cd, pwd, pinfo) as before

        // Check if the command should run in the background
        bool run_in_background = false;
        char *ampersand = strchr(input, '&');
        if (ampersand != nullptr) {
            *ampersand = '\0';
            run_in_background = true;
        }

        // Tokenize the command and arguments
        std::istringstream iss(input);
        std::string token;
        std::vector<char *> arguments;
        while (iss >> token) {
            arguments.push_back(const_cast<char *>(token.c_str()));
        }
        arguments.push_back(nullptr); // Null-terminate the argument list

        // Execute the command
        if (arguments.size() > 0) {
            const char *command = arguments[0];
            arguments.erase(arguments.begin()); // Remove the command itself from arguments

            if (run_in_background) {
                execute_background(command, arguments.data(), background_pids);
            } else {
                execute_foreground(command, arguments.data());
            }
        }

        // Handle background processes that have finished
        int status;
        pid_t finished_pid = waitpid(-1, &status, WNOHANG);
        while (finished_pid > 0) {
            std::cout << "[" << finished_pid << "] Background process finished." << std::endl;
            background_pids.erase(std::remove(background_pids.begin(), background_pids.end(), finished_pid), background_pids.end());
            finished_pid = waitpid(-1, &status, WNOHANG);
        }
    }

    // Terminate any remaining background processes
    for (pid_t pid : background_pids) {
        kill(pid, SIGTERM);
    }

    return 0;
}
