#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <unistd.h>

// Function to get process info for the current process (shell)
void get_shell_process_info() {
    pid_t pid = getpid();
    std::cout << "pid -- " << pid << std::endl;

    char status;
    std::ifstream status_file("/proc/" + std::to_string(pid) + "/status");
    if (status_file.is_open() && status_file >> status) {
        std::string status_code;
        switch (status) {
            case 'R':
                status_code = "R";
                break;
            case 'S':
                status_code = "S";
                break;
            case 'Z':
                status_code = "Z";
                break;
            case 'T':
                status_code = "T";
                break;
            default:
                status_code = "?";
        }
        std::cout << "Process Status -- {" << status_code;

        // Check if the process is in the foreground
        if (tcgetpgrp(STDIN_FILENO) == pid) {
            std::cout << "+";
        }

        std::cout << "}" << std::endl;
    }

    std::ifstream statm_file("/proc/" + std::to_string(pid) + "/statm");
    unsigned long virtual_memory;
    if (statm_file.is_open() && statm_file >> virtual_memory) {
        std::cout << "memory -- " << virtual_memory << " {Virtual Memory}" << std::endl;
    }

    char exe_path[1024];
    ssize_t path_length = readlink("/proc/self/exe", exe_path, sizeof(exe_path));
    if (path_length != -1) {
        exe_path[path_length] = '\0';
        std::cout << "Executable Path -- " << exe_path << std::endl;
    }
}

// Function to get process info for a specified PID
void get_process_info(pid_t pid) {
    std::cout << "pid -- " << pid << std::endl;

    // Open the status file for the specified PID
    std::ifstream status_file("/proc/" + std::to_string(pid) + "/status");
    if (!status_file.is_open()) {
        std::cerr << "Error: Process with PID " << pid << " not found." << std::endl;
        return;
    }

    char status;
    if (status_file >> status) {
        std::string status_code;
        switch (status) {
            case 'R':
                status_code = "R";
                break;
            case 'S':
                status_code = "S";
                break;
            case 'Z':
                status_code = "Z";
                break;
            case 'T':
                status_code = "T";
                break;
            default:
                status_code = "?";
        }
        std::cout << "Process Status -- {" << status_code << "}" << std::endl;
    }

    // Get virtual memory size from statm file
    std::ifstream statm_file("/proc/" + std::to_string(pid) + "/statm");
    unsigned long virtual_memory;
    if (statm_file.is_open() && statm_file >> virtual_memory) {
        std::cout << "memory -- " << virtual_memory << " {Virtual Memory}" << std::endl;
    }

    // Get executable path using /proc/<PID>/exe symlink
    char exe_path[1024];
    ssize_t path_length = readlink(("/proc/" + std::to_string(pid) + "/exe").c_str(), exe_path, sizeof(exe_path));
    if (path_length != -1) {
        exe_path[path_length] = '\0';
        std::cout << "Executable Path -- " << exe_path << std::endl;
    }
}

// int main(int argc, char* argv[]) {
//     if (argc == 1) {
//         // No PID provided, get info for the current shell process
//         get_shell_process_info();
//     } else if (argc == 2) {
//         // PID provided as an argument, get info for the specified PID
//         pid_t pid = std::stoi(argv[1]);
//         get_process_info(pid);
//     } else {
//         std::cerr << "Usage: " << argv[0] << " [pid]" << std::endl;
//         return 1;
//     }

//     return 0;
// }
