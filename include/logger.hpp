#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <filesystem>
#include <fstream>
#include <unistd.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace Logger {
    void init(std::string log_directory, std:: string log_file);
    std::shared_ptr<spdlog::logger> getLogger();
}

#endif // LOGGER_H

