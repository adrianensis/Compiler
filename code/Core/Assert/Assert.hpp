#pragma once

#include "Core/Signal/Signal.hpp"
#include <cstdio>
#include <string>

class AssertUtils
{
public:
	template <typename... T>
    static void checkMsg(bool condition, const std::string& conditionString, const std::string& file, unsigned int line, const std::string& function, const std::string& fmt, T&&... args)
    {
        if (!condition)
        {
            std::string composedFmt = "[" + file + ":" + function + ":" + std::to_string(line) + "]";
            composedFmt += "[" + conditionString + "] ";
            composedFmt += fmt;
            std::printf(composedFmt.c_str(), args...);
            SignalUtils::breakpointTrap();
        }
    }
};

#define CHECK_MSG(condition, ...) AssertUtils::checkMsg((condition), #condition, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);