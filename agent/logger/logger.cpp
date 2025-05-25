#include "logger.hpp"


#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

static std::shared_ptr<spdlog::logger> logger;

constexpr std::string LOG_FILENAME = "win_agent.log";
constexpr std::string LOGGER_NAME = "winagentlogger";

std::shared_ptr<spdlog::logger> get_logger() {
    return spdlog::get(LOGGER_NAME);
}


void setup_logger() {
    const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    const auto basic_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOG_FILENAME);
    std::vector<spdlog::sink_ptr> sinks{console_sink, basic_sink};
    logger = std::make_shared<spdlog::logger>(LOGGER_NAME, sinks.begin(), sinks.end());

    spdlog::register_logger(logger);

    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::warn);
}
