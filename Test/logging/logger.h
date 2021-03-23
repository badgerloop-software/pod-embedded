#include <chrono>
#include <atomic>
#include <string>
#include <iostream>

#pragma once

using namespace std::string_literals;

// #define LOGGER_LEVEL_INFO 0
// #define LOGGER_LEVEL_WARN 1
// #define LOGGER_LEVEL_ERROR 2

// #if !defined(LOGGER_ACTIVE_LEVEL)
// #define LOGGER_ACTIVE_LEVEL LOGGER_LEVEL_INFO
// #endif

// #ifndef LOGGER_LEVEL_NAMES
// #define LOGGER_LEVEL_NAMES = ["info"s, "warn"s, "error"s]
// #endif


// Log Level Enum
namespace level {
    enum level_enum : int
    {
        info,
        warn,
        error
    };
    std::string LOGGER_LEVEL_NAMES[3]{"info", "warn", "error"};

} // namespace level

namespace badger_logger {
    class stopwatch {
        using log_clock = std::chrono::steady_clock;
        std::chrono::time_point<log_clock> start_timepoint;

        public:
            stopwatch() : start_timepoint{log_clock::now()}
            {}
        std::chrono::duration<double> elapsed() const
        {
            return std::chrono::duration<double>(log_clock::now() - start_timepoint);
        }
        void reset()
        {
            start_timepoint = log_clock::now();
        }
    };

    class logger {
        public: logger(std::string name)
        {
            std::string logger_name = name;
            logger_clock = stopwatch();
        }

        void log(std::string message, level::level_enum lvl)
        {
            auto time = logger_clock.elapsed().count();
            auto level = level::LOGGER_LEVEL_NAMES[(int)lvl].c_str();
            printf("[%f] [%s] %s\n", time, level, message.c_str());
        }

        private:
            std::string logger_name;
            badger_logger::stopwatch logger_clock;
    };
} // nameespace badger_logger