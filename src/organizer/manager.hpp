#pragma once

#include <inotify-cxx.h>
#include <functional>
#include <vector>

class Manager {
  public:
    void watch(std::string path);

    void run(std::function<void(std::string_view, std::string_view)> handler);

  private:
    Inotify notify;
    std::vector<InotifyWatch> watches;
};
