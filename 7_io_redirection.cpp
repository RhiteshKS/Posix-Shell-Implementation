#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Function to execute a command with I/O redirection
void execute_with_redirection(const char *command, char *const arguments[]) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child process

        // Input redirection
        if (input_redirect) {
            int input_fd = open(input_redirect, O_RDONLY);
            if (input_fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        // Output redirection
        if (output_redirect) {
            int output_fd;
            if (append_output) {
                output_fd = open(output_redirect, O_WRONLY | O_CREAT | O_APPEND, 0644);
            } else {
                output_fd = open(output_redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            if (output_fd == -1) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        // Execute the command
        if (execvp(command, arguments) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process (shell)
        waitpid(pid, &status, 0);
    }
}

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        std::cout << "YourShell> ";
        if (fgets(input, sizeof(input), stdin) == nullptr) {
            break; // Exit on Ctrl+D or EOF
        }

        // Tokenize input and handle built-in commands (ls, echo, cd, pwd, pinfo) as before

        // Check for I/O redirection
        char *input_redirect = nullptr;
        char *output_redirect = nullptr;
        bool append_output = false;

        char *token = strtok(input, " \t\n");

        while (token) {
            if (strcmp(token, "<") == 0) {
                token = strtok(nullptr, " \t\n");
                if (token) {
                    input_redirect = token;
                }
            } else if (strcmp(token, ">") == 0) {
                token = strtok(nullptr, " \t\n");
                if (token) {
                    output_redirect = token;
                    append_output = false;
                }
            } else if (strcmp(token, ">>") == 0) {
                token = strtok(nullptr, " \t\n");
                if (token) {
                    output_redirect = token;
                    append_output = true;
                }
            }
            token = strtok(nullptr, " \t\n");
        }

        // Remove redirection tokens from the input
        if (input_redirect) {
            input_redirect[-1] = '\0'; // Null-terminate before the "<"
        } else if (output_redirect) {
            output_redirect[-1] = '\0'; // Null-terminate before the ">" or ">>"
        }

        // Execute the command with redirection
        if (arguments.size() > 0) {
            const char *command = arguments[0];
            arguments.erase(arguments.begin()); // Remove the command itself from arguments

            execute_with_redirection(command, arguments.data());
        }
    }

    return 0;
}
