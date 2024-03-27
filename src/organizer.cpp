#include "organizer/manager.hpp"
#include <iostream>
#include <string>

int main() {
    Manager manager;
    manager.watch("/home/tiagovla/Downloads");
    manager.run([](auto file_name, auto file_path) {
        std::cout << "Event: " << file_name << std::endl;
        std::cout << "Path: " << file_path << std::endl;
    });
};
