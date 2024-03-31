#include <filesystem>
#include <string>

namespace organizer {
std::string expand_home(const std::string &path);

std::string get_config_path();

std::filesystem::path new_file_path(const std::filesystem::path &old_file);
} // namespace organizer
