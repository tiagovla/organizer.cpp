#include "organizer/config.hpp"
#include "organizer/version.hpp"
#include "organizer/manager.hpp"
#include <argparse/argparse.hpp>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char **argv) {

    argparse::ArgumentParser cli("organizer", ORGANIZER_VERSION);
    cli.add_argument("-c", "--config")
        .default_value(std::string("~/.config/organizer.toml"))
        .help("path of the config file")
        .nargs(1);
    cli.add_argument("-d", "--debug")
        .default_value(false)
        .implicit_value(true)
        .help("enable debugging");
    try {
        cli.parse_args(argc, argv);
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        std::exit(1);
    }

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
