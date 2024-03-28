#include <iostream>
#include <string>
#include "config.hpp"
#include <toml++/toml.h>

Config::Config() = default;
Config::Config(auto rules) : _rules(rules) {}
Config::Config(auto &rules) : _rules(std::move(rules)) {}
std::unordered_map<std::string, std::string> Config::rules_for_watch(std::string watch_path) {
    if (_rules.contains(watch_path)) {
        return _rules[watch_path];
    }
    return {};
}

Config TOMLConfigParser::parse(std::string path)  {
    toml::table tbl;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> rules;
    try {
        tbl = toml::parse_file(path);
    } catch (const toml::parse_error &e) {
        std::cerr << "Error parsing config file " << path << ": " << e.what() << std::endl;
        exit(1);
    }
    for (const auto &[watch, folders] : tbl) {
        std::cout << watch << std::endl;
        if (!folders.is_table()) {
            std::cerr << "Error parsing config file " << path << ": no [folders] section" << std::endl;
            return {};
        }
        std::unordered_map<std::string, std::string> sub_rules;
        for (const auto &[folder_name, extensions] : *folders.as_table()) {
            std::cout << folder_name << std::endl;
            for (const auto &ext : *extensions.as_array()) {
                std::cout << ext.as_string()->get() << std::endl;
                sub_rules[ext.as_string()->get()] = std::string(folder_name.str());
            }
        }
        rules[std::string(watch.str())] = std::move(sub_rules);
    }
    return Config(std::move(rules));
}
