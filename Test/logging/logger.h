#include <chrono>
#include <atomic>
#include <string>
#include <iostream>

#pragma once

#define LOGGER_LEVEL_INFO 0
#define LOGGER_LEVEL_WARN 1
#define LOGGER_LEVEL_ERROR 2

#if !defined(LOGGER_ACTIVE_LEVEL)
#define LOGGER_ACTIVE_LEVEL LOGGER_LEVEL_INFO
#endif



// Log Level Enum
namespace level {
    enum level_enum : int
    {
        info = LOGGER_LEVEL_INFO,
        warn = LOGGER_LEVEL_WARN,
        error = LOGGER_LEVEL_ERROR
    };

// #ifndef LOGGER_LEVEL_NAMES
// #define LOGGER_LEVEL_NAMES
//     {
//         "info", "warn", "error"
//     }
// #endif


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
        }

        void log(std::string message, level::level_enum lvl)
        {
            std::wcout << message.c_str();
        }
    };
} // nameespace badger_logger