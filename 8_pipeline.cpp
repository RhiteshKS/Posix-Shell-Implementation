#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <algorithm> 

#define MAX_INPUT_SIZE 1024

int main() {
    char input[MAX_INPUT_SIZE];

    while (1) {
        std::cout << "YourShell> ";
        if (fgets(input, sizeof(input), stdin) == nullptr) {
            break; // Exit on Ctrl+D or EOF
        }

        // Tokenize input and handle built-in commands (ls, echo, cd, pwd, pinfo) as before

        // Check for pipes (|)
        std::vector<char*> commands;
        char* token = strtok(input, "|");
        while (token) {
            commands.push_back(token);
            token = strtok(nullptr, "|");
        }

        // Create pipes
        int num_pipes = commands.size() - 1;
        int pipes[num_pipes * 2]; // Two file descriptors (read and write) for each pipe
        for (int i = 0; i < num_pipes; i++) {
            if (pipe(pipes + i * 2) == -1) {
                perror("pipe");
                return 1;
            }
        }

        // Execute commands in the pipeline
        pid_t pid;
        int input_fd = STDIN_FILENO; // Input for the first command is stdin
        for (int i = 0; i < commands.size(); i++) {
            char* command = commands[i];

            pid = fork();
            if (pid == -1) {
                perror("fork");
                return 1;
            } else if (pid == 0) {
                // Child process

                // Redirect input from the previous command's output (except for the first command)
                if (i > 0) {
                    dup2(input_fd, STDIN_FILENO);
                }

                // Redirect output to the next command's input (except for the last command)
                if (i < num_pipes) {
                    dup2(pipes[i * 2 + 1], STDOUT_FILENO);
                }

                // Close all pipe file descriptors in the child process
                for (int j = 0; j < num_pipes * 2; j++) {
                    close(pipes[j]);
                }

                // Execute the command
                if (execvp(command, arguments.data()) == -1) {
                    perror("execvp");
                    return 1;
                }
            } else {
                // Parent process
                waitpid(pid, nullptr, 0);

                // Close the input file descriptor of the previous command
                if (i > 0) {
                    close(input_fd);
                }

                // Close the output file descriptor of the current command
                close(pipes[i * 2 + 1]);

                // The next command reads from the current command's output
                input_fd = pipes[i * 2];
            }
        }

        // Close all remaining pipe file descriptors in the parent process
        for (int i = 0; i < num_pipes * 2; i++) {
            close(pipes[i]);
        }
    }

    return 0;
}
