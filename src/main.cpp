#include "inotify-cxx.h"
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <stdio.h>
#include <string>
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

int main(int argc, char **argv) {
    std::unordered_map<std::string, std::vector<std::string>> file_types{
        {"documents", {".pdf", ".mobi", ".epub"}},
        {"music", {".mp3"}},
        {"videos", {".mp4"}},
        {"images", {".jpg", ".jpeg"}},
        {"compressed", {".zip", ".rar", ".gz", ".bz2", ".7z"}},
    };
    auto map = invert_map(file_types);

    fs::path path;
    if (argc > 1) {
        path = fs::path(argv[1]);
    } else {
        path = fs::path(std::getenv("HOME")) / "Downloads";
    }

    if (fs::is_directory(path)) {
        std::cout << "Organizing folder " << path << std::endl;
        organize_folder(path, map);
        run_watcher(path, map);
    } else {
        std::cerr << "Path " << path << " is not a directory" << std::endl;
    }
    return 0;
}
