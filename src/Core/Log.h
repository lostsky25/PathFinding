#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class Log
{
public:
    static void init();

    inline static std::shared_ptr<spdlog::logger> getCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger> getClientLogger() { return s_ClientLogger; }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

// Core log macros
#define PF_CORE_TRACE(...)    ::Log::getCoreLogger()->trace(__VA_ARGS__)
#define PF_CORE_INFO(...)     ::Log::getCoreLogger()->info(__VA_ARGS__)
#define PF_CORE_WARN(...)     ::Log::getCoreLogger()->warn(__VA_ARGS__)
#define PF_CORE_ERROR(...)    ::Log::getCoreLogger()->error(__VA_ARGS__)
#define PF_CORE_FATAL(...)    ::Log::getCoreLogger()->fatal(__VA_ARGS__)
                                     
// Client log macros                 
#define PF_TRACE(...)	      ::Log::getClientLogger()->trace(__VA_ARGS__)
#define PF_INFO(...)	      ::Log::getClientLogger()->info(__VA_ARGS__)
#define PF_WARN(...)	      ::Log::getClientLogger()->warn(__VA_ARGS__)
#define PF_ERROR(...)	      ::Log::getClientLogger()->error(__VA_ARGS__)
#define PF_FATAL(...)	      ::Log::getClientLogger()->fatal(__VA_ARGS__)
