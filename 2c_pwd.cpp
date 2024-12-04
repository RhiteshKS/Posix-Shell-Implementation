#include <iostream>
#include <limits.h>
#include <unistd.h>

int main() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << cwd << std::endl;
    } else {
        perror("pwd");
        return 1;
    }

    return 0;
}
