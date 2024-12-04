#include <iostream>
#include <cstring>
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>

// Function to set up tab completion
void setup_tab_completion() {
    rl_bind_key('\t', rl_complete); // Bind the TAB key to rl_complete
}

int main() {
    using_history(); // Enable command history

    while (1) {
        char* input = readline("YourShell> ");
        if (!input) {
            break; // Exit on Ctrl+D or EOF
        }

        // Add the entered command to history
        add_history(input);

        // Process and execute the command
        // Handle built-in commands (ls, echo, cd, pwd, pinfo) and other user commands

        // Implement tab completion
        rl_attempted_completion_function = [](const char* text, int start, int end) -> char** {
            // Get a list of files and directories in the current directory
            char* dir = strdup("."); // Current directory
            char** matches = (char**)NULL;

            // Use scandir to list files and directories
            int num_entries = scandir(dir, &matches, NULL, alphasort);

            // Create a vector to store matching completions
            std::vector<std::string> completions;
            for (int i = 0; i < num_entries; i++) {
                if (strncmp(matches[i], text, end - start) == 0) {
                    completions.push_back(matches[i]);
                }
                free(matches[i]);
            }
            free(matches);

            // Allocate and return completions as a char** array
            char** rl_completions = (char**)NULL;
            if (!completions.empty()) {
                rl_completions = (char**)malloc((completions.size() + 1) * sizeof(char*));
                for (size_t i = 0; i < completions.size(); i++) {
                    rl_completions[i] = strdup(completions[i].c_str());
                }
                rl_completions[completions.size()] = (char*)NULL;
            }

            return rl_completions;
        };

        // Process and execute the command

        // Free the input string
        free(input);
    }

    return 0;
}
