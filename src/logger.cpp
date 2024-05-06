#include "../include/logger.hpp"

namespace Logger {
    std:: string log_name = "Quad Tree Device";

    void init(std::string log_directory, std:: string log_file) {
        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) == 0) {
            log_name = std::string(hostname);
        } 
        // Create a rotating logger with max file size of 1MB and keeping 5 files
        auto rotating_logger = spdlog::rotating_logger_mt(log_name , log_directory + "/"+ log_file , 1024 * 1024, 5);
        spdlog::set_default_logger(rotating_logger);
    }

    std::shared_ptr<spdlog::logger> getLogger() {
        return spdlog::get(log_name);
    }
}
