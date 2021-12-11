#pragma once

#include "render.h"
#include "glutils.h"

#include <chrono>
#include <unordered_map>
#include <string>

#include <xmmintrin.h>
#include <pmmintrin.h>

struct Shortcuts
{
    std::unordered_map<std::string, bool> m_Map;
};

int application(int argc, char** argv);

inline auto get_time()
{
    auto start = std::chrono::system_clock::now();
    return start;
}