#include "organizer/config.hpp"
#include "organizer/manager.hpp"
#include <iostream>
#include <string>
#include <memory>

int main() {
    std::unique_ptr<ConfigParser> parser = std::make_unique<TOMLConfigParser>();
    parser->parse("/home/tiagovla/.config/organizer.toml");

    Manager manager;
    manager.watch("/home/tiagovla/Downloads");
    manager.run([](auto file_name, auto file_path) {
        std::cout << "Event: " << file_name << std::endl;
        std::cout << "Path: " << file_path << std::endl;
    });
};
