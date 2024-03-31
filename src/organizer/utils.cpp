#include <string>

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
