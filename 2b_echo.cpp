#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <string>" << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        std::cout << argv[i];
        if (i < argc - 1) {
            std::cout << ' ';
        }
    }
    std::cout << std::endl;

    return 0;
}

