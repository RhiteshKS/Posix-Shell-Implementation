#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>
#include <vector>

pid_t foreground_process_pid = -1;

// Signal handler for CTRL-Z (SIGTSTP)
void handle_ctrl_z(int signum) {
    if (foreground_process_pid != -1) {
        // Send SIGTSTP to the foreground process
        kill(foreground_process_pid, SIGTSTP);
        std::cout << "Foreground process paused." << std::endl;
        foreground_process_pid = -1; // No foreground process running
    } else {
        std::cout << "No foreground process running." << std::endl;
    }
}

// Signal handler for CTRL-C (SIGINT)
void handle_ctrl_c(int signum) {
    if (foreground_process_pid != -1) {
        // Send SIGINT to the foreground process
        kill(foreground_process_pid, SIGINT);
        std::cout << "Foreground process interrupted." << std::endl;
        foreground_process_pid = -1; // No foreground process running
    } else {
        std::cout << "No foreground process running." << std::endl;
    }
}

int main() {
    struct sigaction ctrl_z_action;
    struct sigaction ctrl_c_action;

    // Set up signal handlers
    ctrl_z_action.sa_handler = handle_ctrl_z;
    sigemptyset(&ctrl_z_action.sa_mask);
    ctrl_z_action.sa_flags = 0;
    sigaction(SIGTSTP, &ctrl_z_action, NULL);

    ctrl_c_action.sa_handler = handle_ctrl_c;
    sigemptyset(&ctrl_c_action.sa_mask);
    ctrl_c_action.sa_flags = 0;
    sigaction(SIGINT, &ctrl_c_action, NULL);

    char input[MAX_INPUT_SIZE];

    while (1) {
        std::cout << "YourShell> ";
        if (fgets(input, sizeof(input), stdin) == nullptr) {
            break; // Exit on Ctrl+D or EOF
        }

        // Tokenize input and handle built-in commands (ls, echo, cd, pwd, pinfo) as before

        // Check for background execution (e.g., 'command &')
        bool run_in_background = false;
        if (arguments.size() > 0 && strcmp(arguments.back(), "&") == 0) {
            run_in_background = true;
            arguments.pop_back(); // Remove the '&' from arguments
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        } else if (pid == 0) {
            // Child process

            // Execute the command
            if (execvp(command, arguments.data()) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process

            // If the command is not run in the background, wait for the child process to complete
            if (!run_in_background) {
                int status;
                foreground_process_pid = pid;
                waitpid(pid, &status, 0);
                foreground_process_pid = -1;
            }
        }
    }

    return 0;
}
