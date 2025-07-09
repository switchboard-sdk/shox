//
//  Platform.hpp
//
//  Created by Ivan Nador on 2025. 07. 09..
//  Copyright (c) 2025 Synervoz Communications Inc. All rights reserved.
//

#pragma once

#if defined(_WIN32)
#include <Windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

namespace shox {

/**
 * @brief A utility struct for platform-specific operations.
 */
struct Platform {

    /**
     * @brief Retrieves the absolute path of the currently running executable.
     *
     * This function provides platform-specific implementations to determine
     * the executable's path. It supports Windows, macOS, and Linux.
     *
     * @return std::filesystem::path The absolute path to the executable.
     * @throws std::runtime_error If the executable path cannot be determined.
     */
    static std::filesystem::path getExecutablePath() {
#if defined(_WIN32)
        // Windows implementation: Uses GetModuleFileNameA to retrieve the path.
        char path[MAX_PATH];
        DWORD length = GetModuleFileNameA(NULL, path, MAX_PATH);
        if (length == 0 || length == MAX_PATH) {
            throw std::runtime_error("Failed to get executable path.");
        }
        return { std::string(path, length) };
#elif defined(__APPLE__)
        // macOS implementation: Uses _NSGetExecutablePath to retrieve the path.
        char path[1024];
        uint32_t size = sizeof(path);
        if (_NSGetExecutablePath(path, &size) != 0) {
            std::vector<char> buf(size);
            if (_NSGetExecutablePath(buf.data(), &size) != 0) {
                throw std::runtime_error("Failed to get executable path.");
            }
            return { std::string(buf.data()) };
        }
        return { std::string(path) };
#elif defined(__linux__)
        // Linux implementation: Uses readlink to retrieve the path from /proc/self/exe.
        char path[1024];
        ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
        if (count == -1) {
            throw std::runtime_error("Failed to get executable path.");
        }
        path[count] = '\0';
        return { std::string(path) };
#else
        // Unsupported platform.
        throw std::runtime_error("Unsupported platform for getExecutablePath.");
#endif
    }
};

} // namespace shox