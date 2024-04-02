// Copyright 2024 <tiagovla>

#include "organizer/config.hpp"
#include "organizer/manager.hpp"
#include "organizer/utils.hpp"
#include "organizer/version.hpp"
#include <argparse/argparse.hpp>
#include <filesystem>
#include <loguru.hpp>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char **argv) {

    argparse::ArgumentParser cli("organizer", ORGANIZER_VERSION);
    cli.add_argument("-c", "--config")
        .default_value("~/.config/organizer.toml")
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
    if (cli["--debug"] == false)
        loguru::g_stderr_verbosity = loguru::Verbosity_OFF;
    loguru::init(argc, argv);

    organizer::Config config;
    organizer::InotifyManager manager;

    {
        std::unique_ptr<organizer::ConfigParser> parser =
            std::make_unique<organizer::TOMLConfigParser>();
        config = parser->parse(organizer::get_config_path());
    }

    for (auto &path : config.watch_paths()) {
        manager.add_watcher(path);
    }

    manager.run([&](auto file_name, auto file_path) {
        auto old_file = std::filesystem::path(file_path) / file_name;
        auto folder = config.rules_for_watch(
            std::string(file_path))[old_file.extension()];
        if (!folder.empty()) {
            auto new_file =
                std::filesystem::path(file_path) / folder / file_name;
            new_file = organizer::new_file_path(new_file);
            try {
                fs::rename(old_file, new_file);
            } catch (fs::filesystem_error &e) {
                fs::create_directory(new_file.parent_path());
                fs::rename(old_file, new_file);
            }
            LOG_F(INFO, "Moving %s -> %s", old_file.c_str(), new_file.c_str());
        }
    });
}
