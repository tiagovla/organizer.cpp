#include "inotify-cxx.h"
#include <algorithm>
#include <argparse/argparse.hpp>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <string>
#include <toml++/toml.h>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

fs::path create_new_filepath(fs::path &filepath, std::string &foldername) {
    auto parent_path = filepath.parent_path();
    auto filename = filepath.filename();
    return parent_path / foldername / filename;
}

void move_file(fs::path filepath, std::string foldername, bool verbose = true) {
    if (!foldername.length())
        return;
    auto newfilepath = create_new_filepath(filepath, foldername);
    if (verbose) {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        auto time = std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
        std::cout << time << " " << filepath << " -> " << newfilepath << std::endl;
    }
    try {
        fs::rename(filepath, newfilepath);
    } catch (fs::filesystem_error &e) {
        fs::create_directory(newfilepath.parent_path());
        fs::rename(filepath, newfilepath);
    }
}

std::unordered_map<std::string, std::string> invert_map(std::unordered_map<std::string, std::vector<std::string>> &map) {
    std::unordered_map<std::string, std::string> inverse_map;
    for (auto const &[k, v_list] : map)
        for (auto const &v : v_list)
            inverse_map[v] = k;
    return inverse_map;
}

void organize_folder(fs::path &path, std::unordered_map<std::string, std::string> &map) {
    for (const fs::directory_entry &dir_entry : fs::directory_iterator{path}) {
        if (dir_entry.is_directory())
            continue;
        const fs::path &filepath = dir_entry.path();
        const std::string &foldername = map[filepath.extension()];
        move_file(filepath, foldername);
    }
}

void run_watcher(fs::path &path, std::unordered_map<std::string, std::string> &map) {
    try {
        Inotify notify;
        InotifyWatch watch(path, IN_MOVED_TO | IN_CLOSE_WRITE);
        notify.add(watch);
        std::cout << "Watching directory " << path << std::endl;
        for (;;) {
            notify.wait_for_events();
            size_t count = notify.get_event_count();
            while (count > 0) {
                InotifyEvent event;
                bool got_event = notify.get_event(&event);
                std::string strr;
                event.dump_types(strr);
                fs::path filepath = path / event.GetName();
                std::string &foldername = map[filepath.extension()];
                if (got_event) {
                    if (!fs::is_directory(filepath))
                        move_file(filepath, foldername);
                }
                count--;
            }
        }
    } catch (InotifyException &e) {
        std::cerr << "Inotify exception occured: " << e.GetMessage() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "STL exception occured: " << e.what() << std::endl;
    }
}

std::unordered_map<std::string, std::string> config_map(std::string config_path) {
    toml::table tbl;
    try {
        tbl = toml::parse_file(config_path);
    } catch (const toml::parse_error &e) {
        std::cerr << "Error parsing config file " << config_path << ": " << e.what() << std::endl;
        exit(1);
    }
    auto folders = tbl["folders"];
    if (!folders.is_table()) {
        std::cerr << "Error parsing config file " << config_path << ": no [folders] section" << std::endl;
        return {};
    }
    std::unordered_map<std::string, std::vector<std::string>> map;
    for (const auto &folder : *folders.as_table()) {
        auto foldername = std::string(folder.first.str());
        auto extensions = folder.second.as_array();
        map[foldername].reserve(extensions->size());
        std::transform(extensions->begin(), extensions->end(), std::back_inserter(map[foldername]),
                       [](toml::node &e) { return e.as_string()->get(); });
    }
    return invert_map(map);
}

std::string expand_home(const std::string &path) {
    std::string home = std::getenv("HOME");
    if (!path.empty() && path[0] == '~' && !home.empty()) {
        return std::string(home) + path.substr(1);
    }
    return path;
}

std::string get_config_path() {
    std::string config_path = std::getenv("XDG_CONFIG_HOME");
    if (config_path.empty())
        config_path = expand_home("~/.config");
    return config_path + "/organizer.toml";
}

int main(int argc, char **argv) {
    argparse::ArgumentParser cli("organizer");
    cli.add_argument("-p", "--path").default_value(std::string("~/Downloads")).help("path of the folder to be organized").nargs(0);
    try {
        cli.parse_args(argc, argv);
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        std::exit(1);
    }

    auto config_path = get_config_path();
    auto map = config_map(config_path);
    fs::path path(expand_home(cli.get<std::string>("--path")));

    if (fs::is_directory(path)) {
        std::cout << "Organizing folder " << path << std::endl;
        organize_folder(path, map);
        run_watcher(path, map);
    } else {
        std::cerr << "Path " << path << " is not a directory" << std::endl;
    }
    return 0;
}
