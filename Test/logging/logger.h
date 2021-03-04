#include <chrono>
#include <atomic>

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
} // nameespace badger_logger