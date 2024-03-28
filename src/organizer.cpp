#include "organizer/config.hpp"
#include "organizer/manager.hpp"
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

namespace fs = std::filesystem;

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

    manager.run([&](auto file_name, auto file_path) {
        auto old_file = std::filesystem::path(file_path) / file_name;
        auto folder = config.rules_for_watch(std::string(file_path))[old_file.extension()];
        if (!folder.empty()) {
            auto new_file = std::filesystem::path(file_path) / folder / file_name;
            try {
                fs::rename(old_file, new_file);
            } catch (fs::filesystem_error &e) {
                fs::create_directory(new_file.parent_path());
                fs::rename(old_file, new_file);
            }
            std::cout << "Moving: " << old_file << " -> " << new_file << std::endl;
        }
    });
};
