//
//  Platform.test.cpp
//
//  Created by Ivan Nador on 2025. 07. 10..
//  Copyright (c) 2025 Synervoz Communications Inc. All rights reserved.
//

#include <shox/Platform.hpp>
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Platform::getExecutablePath returns a valid path", "[Platform]") {
    const auto path = shox::Platform::getExecutablePath();
    REQUIRE(!path.empty());
    REQUIRE(std::filesystem::exists(path));
}