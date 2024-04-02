// Copyright 2024 <tiagovla>

#pragma once

#include <functional>
#include <inotify-cxx.h>

namespace organizer {
/**
 * @class Manager
 * @brief Representation of an inotify manager to watch paths for changes
 *
 */
class InotifyManager {
  public:
    /**
     * @brief Watch a path for changes
     *
     * @param path
     */
    void add_watcher(std::string path);

    /**
     * @brief Run the manager with a handler function
     *
     * @param handler
     */
    void run(std::function<void(std::string_view, std::string_view)> handler);

  private:
    Inotify notify;
    std::vector<InotifyWatch> _watches;
};
} // namespace organizer
