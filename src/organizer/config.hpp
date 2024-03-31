#pragma once

#include <ranges>
#include <string>
#include <unordered_map>

/**
 * @class Config
 * @brief Representation of a configuration for the organizer
 *
 */
class Config {
  public:
    Config();
    Config(auto rules);
    Config(auto &rules);
    std::unordered_map<std::string, std::string> rules_for_watch(std::string watch_path);
    auto watch_paths() { return std::views::keys(_rules); }

  public:
    // {"watch_path": {"extension": "folder_name"}}
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> _rules;
};

class ConfigParser {
  public:
    virtual Config parse(std::string path) = 0;
};

class TOMLConfigParser : public ConfigParser {
  public:
    Config parse(std::string path) override;
};
