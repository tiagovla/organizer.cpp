#include "organizer/config.hpp"
#include "organizer/manager.hpp"
#include <iostream>
#include <memory>
#include <string>


int main() {
    Config config;
    Manager manager;

    {
        std::unique_ptr<ConfigParser> parser = std::make_unique<TOMLConfigParser>();
        config = parser->parse("/home/tiagovla/.config/organizer.toml");
    }

    for (auto &path : config.watch_paths()) {
        manager.watch(path);
    }

    manager.run([](auto file_name, auto file_path) {
        std::cout << "Event: " << file_name << std::endl;
        std::cout << "Path: " << file_path << std::endl;
    });
};
