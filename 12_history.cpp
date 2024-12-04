#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>

// Maximum number of commands to store in history
#define MAX_HISTORY_SIZE 100

// Function to load command history from a file
void load_history(std::vector<std::string>& history) {
    std::ifstream history_file("history.txt");
    if (history_file.is_open()) {
        std::string line;
        while (getline(history_file, line) && history.size() < MAX_HISTORY_SIZE) {
            history.push_back(line);
        }
        history_file.close();
    }
}

// Function to save command history to a file
void save_history(const std::vector<std::string>& history) {
    std::ofstream history_file("history.txt");
    if (history_file.is_open()) {
        for (const std::string& command : history) {
            history_file << command << std::endl;
        }
        history_file.close();
    }
}

int main() {
    std::vector<std::string> history;
    load_history(history);

    while (1) {
        std::cout << "YourShell> ";
        std::string input;
        if (!std::getline(std::cin, input)) {
            break; // Exit on Ctrl+D or EOF
        }

        // Handle built-in commands (ls, echo, cd, pwd, pinfo) and other user commands

        // Add the entered command to history
        history.push_back(input);

        // Check if the history size exceeds the maximum limit
        if (history.size() > MAX_HISTORY_SIZE) {
            history.erase(history.begin()); // Remove the oldest command
        }

        // Process and execute the command

        // Save the updated history to a file
        save_history(history);
    }

    return 0;
}
