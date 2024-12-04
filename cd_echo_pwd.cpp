#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

// Function to split a string into tokens
std::vector<std::string> split(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = input.find(delimiter);
    while (end != std::string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + 1;
        end = input.find(delimiter, start);
    }
    tokens.push_back(input.substr(start, end));
    return tokens;
}

// Function to implement the built-in "cd" command
void cd(const std::string& arg) {
    if (arg.empty()) {
        // If no argument provided, change to the home directory
        const char* home = getenv("HOME");
        if (home == nullptr) {
            std::cerr << "HOME environment variable is not set." << std::endl;
            return;
        }
        if (chdir(home) != 0) {
            perror("chdir");
        }
    } else if (arg == "-") {
        // If argument is "-", change to the previous directory
        const char* oldpwd = getenv("OLDPWD");
        if (oldpwd == nullptr) {
            std::cerr << "OLDPWD environment variable is not set." << std::endl;
            return;
        }
        if (chdir(oldpwd) != 0) {
            perror("chdir");
        }
    } else if (arg == "..") {
        // If argument is "..", move up one directory
        if (chdir("..") != 0) {
            perror("chdir");
        }
    } else if (arg == "~") {
        // If argument is "~", change to the home directory
        const char* home = getenv("HOME");
        if (home == nullptr) {
            std::cerr << "HOME environment variable is not set." << std::endl;
            return;
        }
        if (chdir(home) != 0) {
            perror("chdir");
        }
    } else {
        // Otherwise, change to the specified directory
        if (chdir(arg.c_str()) != 0) {
            perror("chdir");
        }
    }
}

// Function to implement the built-in "echo" command
void echo(const std::string& arg) {
    std::cout << arg << std::endl;
}

// Function to implement the built-in "pwd" command
void pwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << cwd << std::endl;
    } else {
        perror("getcwd");
    }
}

// int main() {
//     while (true) {
//         std::string input;
//         std::cout << "> ";
//         std::getline(std::cin, input);

//         if (input == "exit") {
//             break;
//         } else if (input == "pwd") {
//             pwd();
//         } else {
//             // Split the input into command and argument
//             std::vector<std::string> parts = split(input, ' ');
//             std::string cmd = parts[0];
//             std::string arg = (parts.size() > 1) ? parts[1] : "";

//             if (cmd == "cd") {
//                 cd(arg);
//             } else if (cmd == "echo") {
//                 echo(arg);
//             } else {
//                 std::cerr << "Command not found: " << cmd << std::endl;
//             }
//         }
//     }

//     return 0;
// }
