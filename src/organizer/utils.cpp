// Copyright 2024 <tiagovla>

#include <filesystem>
#include <string>

namespace organizer {
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

std::filesystem::path new_file_path(const std::filesystem::path &old_file) {
    std::filesystem::path new_file = old_file;
    int count = 1;
    while (std::filesystem::exists(new_file)) {
        new_file = old_file;
        new_file.replace_filename(old_file.stem().string() + "_" +
                                  std::to_string(count) +
                                  old_file.extension().string());
        count++;
    }
    return new_file;
}

} // namespace organizer
