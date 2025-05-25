#include "logger.hpp"


#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

static std::shared_ptr<spdlog::logger> logger;

constexpr std::string LOG_FILENAME = "win_agent.log";

std::shared_ptr<spdlog::logger> get_logger() {
    return spdlog::get("winagentlogger");
}


void setup_logger() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto basic_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOG_FILENAME);
    std::vector<spdlog::sink_ptr> sinks{console_sink, basic_sink};
    logger = std::make_shared<spdlog::logger>("main", sinks.begin(), sinks.end());

    spdlog::register_logger(logger);

    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::warn);
}
