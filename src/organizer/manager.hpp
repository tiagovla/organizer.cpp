// Copyright 2024 <tiagovla>

#pragma once

#include <functional>
#include <inotify-cxx.h>

namespace organizer {
/**
 * @class Manager
 * @brief Representation of a manager to watch paths for changes
 *
 */
class Manager {
  public:
    /**
     * @brief Watch a path for changes
     *
     * @param path
     */
    void watch(std::string path);

    /**
     * @brief Run the manager with a handler function
     *
     * @param handler
     */
    void run(std::function<void(std::string_view, std::string_view)> handler);

  private:
    Inotify notify;
    std::vector<InotifyWatch> watches;
};
} // namespace organizer
