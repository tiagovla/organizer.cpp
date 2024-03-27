#include "manager.hpp"
#include "inotify-cxx.h"
#include <iostream>
#include <string>

static constexpr long PERMISSIONS = IN_MOVED_TO | IN_CLOSE_WRITE;

void Manager::watch(std::string path) {
    watches.emplace_back(path, PERMISSIONS);
    notify.add(watches.back());
}

void Manager::run(std::function<void(std::string_view, std::string_view)> handler) {
    try {
        for (;;) {
            notify.wait_for_events();
            size_t count = notify.get_event_count();
            while (count > 0) {
                InotifyEvent event;
                bool got_event = notify.get_event(&event);
                if (got_event) {
                    std::string_view file_name = event.GetName();
                    std::string_view file_path = event.GetWatch()->GetPath();
                    handler(file_name, file_path);
                }
                count--;
            }
        }
    } catch (InotifyException &e) {
        std::cerr << "Inotify exception occured: " << e.GetMessage() << std::endl;
    } catch (std::exception &e) {
        std::cerr << "STL exception occured: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "unknown exception occured" << std::endl;
    }
}
