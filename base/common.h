#pragma once

#include <filesystem>

#define CHECK(expr) assert(expr)

using Offset = int64_t;
using Pointer = char*;

using NodeId = int32_t;

constexpr static int32_t Null = -1;

namespace fs = std::filesystem;

constexpr uint64_t MMAP_SIZE = 1L << 32;
constexpr uint32_t PAGE_SIZE = 4096;
