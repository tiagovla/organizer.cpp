#pragma once

#include <string>
#include <unordered_map>

class Config {
  public:
    Config();
    Config(auto rules);
    Config(auto &rules);
    std::unordered_map<std::string, std::string> rules_for_watch(std::string watch_path);

  private:
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
