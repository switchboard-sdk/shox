//
//  RingBuffer.test.cpp
//
//  Created by Ivan Nador on 2025. 07. 10..
//  Copyright (c) 2025 Synervoz Communications Inc. All rights reserved.
//

#include <shox/RingBuffer.hpp>
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <thread>

namespace shox::tests {
TEST_CASE("RingBuffer<int8> getAvailableItemsToConsume returns 0 for empty buf") {
    const RingBuffer<int8_t> sut(1000);
    const uint availableItemsToConsume = sut.getAvailableItemsToConsume();
    CHECK(availableItemsToConsume == 0);
}

TEST_CASE("RingBuffer<float> getAvailableItemsToConsume returns 0 for empty buf") {
    const RingBuffer<float> sut(1000);
    const uint availableItemsToConsume = sut.getAvailableItemsToConsume();
    CHECK(availableItemsToConsume == 0);
}

TEST_CASE("RingBuffer<int8> getAvailableItemsToConsume returns valid after produce") {
    RingBuffer<int8_t> sut(1000);
    int8_t data[42];
    sut.produce(data, 42);
    const uint availableItemsToConsume = sut.getAvailableItemsToConsume();
    CHECK(availableItemsToConsume == 42);
}

TEST_CASE("RingBuffer<float> getAvailableItemsToConsume returns valid after produce") {
    RingBuffer<float> sut(1000);
    float data[42];
    sut.produce(data, 42);
    const uint availableItemsToConsume = sut.getAvailableItemsToConsume();
    CHECK(availableItemsToConsume == 42);
}

TEST_CASE("RingBuffer<int8> getAvailableItemsToConsume returns valid after consume") {
    RingBuffer<int8_t> sut(1000);
    int8_t data[42];
    sut.produce(data, 42);
    sut.consume(data, 3);
    const uint availableItemsToConsume = sut.getAvailableItemsToConsume();
    CHECK(availableItemsToConsume == 39);
}

TEST_CASE("RingBuffer<float> getAvailableItemsToConsume returns valid after consume") {
    RingBuffer<float> sut(1000);
    float data[42];
    sut.produce(data, 42);
    sut.consume(data, 3);
    const uint availableItemsToConsume = sut.getAvailableItemsToConsume();
    CHECK(availableItemsToConsume == 39);
}

TEST_CASE("RingBuffer<int8> getAvailableItemsToProduce >= numberOfItems for empty buf") {
    const RingBuffer<int8_t> sut(1000);
    const uint availableItemsToProduce = sut.getAvailableItemsToProduce();
    CHECK(availableItemsToProduce >= 1000);
}

TEST_CASE("RingBuffer<float> getAvailableItemsToProduce >= numberOfItems for empty buf") {
    const RingBuffer<float> sut(1000);
    const uint availableItemsToProduce = sut.getAvailableItemsToProduce();
    CHECK(availableItemsToProduce >= 1000);
}

TEST_CASE("RingBuffer<int8> getAvailableItemsToProduce returns valid after produce") {
    RingBuffer<int8_t> sut(1000);
    const uint availableItemsToProduceBeforeProduce = sut.getAvailableItemsToProduce();
    int8_t data[42];
    sut.produce(data, 42);
    const uint availableItemsToProduceAfterProduce = sut.getAvailableItemsToProduce();
    CHECK(availableItemsToProduceBeforeProduce - availableItemsToProduceAfterProduce == 42);
}

TEST_CASE("RingBuffer<float> getAvailableItemsToProduce returns valid after produce") {
    RingBuffer<float> sut(1000);
    const uint availableItemsToProduceBeforeProduce = sut.getAvailableItemsToProduce();
    float data[42];
    sut.produce(data, 42);
    const uint availableItemsToProduceAfterProduce = sut.getAvailableItemsToProduce();
    CHECK(availableItemsToProduceBeforeProduce - availableItemsToProduceAfterProduce == 42);
}

TEST_CASE("RingBuffer<int8> getAvailableItemsToProduce returns valid after consume") {
    RingBuffer<int8_t> sut(1000);
    int8_t data[42];
    sut.produce(data, 42);
    const uint availableItemsToProduceBeforeConsume = sut.getAvailableItemsToProduce();
    sut.consume(data, 3);
    const uint availableItemsToProduceAfterConsume = sut.getAvailableItemsToProduce();
    CHECK(availableItemsToProduceAfterConsume - availableItemsToProduceBeforeConsume == 3);
}

TEST_CASE("RingBuffer<float> getAvailableItemsToProduce returns valid after consume") {
    RingBuffer<float> sut(1000);
    float data[42];
    sut.produce(data, 42);
    const uint availableItemsToProduceBeforeConsume = sut.getAvailableItemsToProduce();
    sut.consume(data, 3);
    const uint availableItemsToProduceAfterConsume = sut.getAvailableItemsToProduce();
    CHECK(availableItemsToProduceAfterConsume - availableItemsToProduceBeforeConsume == 3);
}

TEST_CASE("RingBuffer<int8> clear resets buffer") {
    RingBuffer<int8_t> sut(1000);
    const uint originalAvailableItemsToProduce = sut.getAvailableItemsToProduce();
    int8_t data[42];
    sut.produce(data, 42);
    sut.consume(data, 3);
    sut.clear();
    CHECK(sut.getAvailableItemsToConsume() == 0);
    CHECK(sut.getAvailableItemsToProduce() == originalAvailableItemsToProduce);
}

TEST_CASE("RingBuffer<float> clear resets buffer") {
    RingBuffer<float> sut(1000);
    const uint originalAvailableItemsToProduce = sut.getAvailableItemsToProduce();
    float data[42];
    sut.produce(data, 42);
    sut.consume(data, 3);
    sut.clear();
    CHECK(sut.getAvailableItemsToConsume() == 0);
    CHECK(sut.getAvailableItemsToProduce() == originalAvailableItemsToProduce);
}

TEST_CASE("RingBuffer<int8> the same buffer can be used multiple times") {
    auto* buffer = new int8_t[1000];
    int8_t data[42];

    RingBuffer sut1(1000, buffer);
    uint originalAvailableItemsToProduce = sut1.getAvailableItemsToProduce();
    sut1.produce(data, 42);
    sut1.consume(data, 3);
    sut1.clear();
    CHECK(sut1.getAvailableItemsToConsume() == 0);
    CHECK(sut1.getAvailableItemsToProduce() == originalAvailableItemsToProduce);

    RingBuffer sut2(500, buffer);
    originalAvailableItemsToProduce = sut2.getAvailableItemsToProduce();
    sut2.produce(data, 42);
    sut2.consume(data, 3);
    sut2.clear();
    CHECK(sut2.getAvailableItemsToConsume() == 0);
    CHECK(sut2.getAvailableItemsToProduce() == originalAvailableItemsToProduce);

    delete[] buffer;
}

TEST_CASE("RingBuffer<float> the same buffer can be used multiple times") {
    auto* buffer = new float[1000];
    float data[42];

    RingBuffer sut1(1000, buffer);
    uint originalAvailableItemsToProduce = sut1.getAvailableItemsToProduce();
    sut1.produce(data, 42);
    sut1.consume(data, 3);
    sut1.clear();
    CHECK(sut1.getAvailableItemsToConsume() == 0);
    CHECK(sut1.getAvailableItemsToProduce() == originalAvailableItemsToProduce);

    RingBuffer sut2(500, buffer);
    originalAvailableItemsToProduce = sut2.getAvailableItemsToProduce();
    sut2.produce(data, 42);
    sut2.consume(data, 3);
    sut2.clear();
    CHECK(sut2.getAvailableItemsToConsume() == 0);
    CHECK(sut2.getAvailableItemsToProduce() == originalAvailableItemsToProduce);

    delete[] buffer;
}

// This is a hard failure (crash)
//TEST_CASE("RingBuffer<int8> consume called on empty buffer") {
//    RingBuffer<int8> sut(1000);
//    int8 data[1];
//    sut.consume(data, 1);
//}

// This is a hard failure (crash)
//TEST_CASE("RingBuffer<int8> produce called on full buffer") {
//    RingBuffer<int8> sut(1000);
//    int8 data[1000];
//    sut.produce(data, 1000);
//    sut.produce(data, 1);
//}

TEST_CASE("RingBuffer<int8> produce consume single value") {
    RingBuffer<int8_t> sut(1000);
    int inValue = 42;
    sut.produce((int8_t*)&inValue, sizeof(int));
    int outValue;
    sut.consume((int8_t*)&outValue, sizeof(int));
    CHECK(inValue == outValue);
}

TEST_CASE("RingBuffer<float> produce consume single value") {
    RingBuffer<float> sut(1000);
    float inValue = 42;
    sut.produce(&inValue, 1);
    float outValue;
    sut.consume(&outValue, 1);
    CHECK(inValue == outValue);
}

TEST_CASE("RingBuffer<int8> produce consume multiple values") {
    RingBuffer<int8_t> sut(1000);
    for (int i = 0; i < 100; i++) {
        int inValue = i;
        sut.produce((int8_t*)&inValue, sizeof(int));
        int outValue;
        sut.consume((int8_t*)&outValue, sizeof(int));
        CHECK(inValue == outValue);
    }
}

TEST_CASE("RingBuffer<float> produce consume multiple values") {
    RingBuffer<float> sut(1000);
    for (int i = 0; i < 100; i++) {
        auto inValue = static_cast<float>(i);
        sut.produce(&inValue, 1);
        float outValue;
        sut.consume(&outValue, 1);
        CHECK(inValue == outValue);
    }
}

TEST_CASE("RingBuffer<int8> produce consume write and read one-by-one at the end") {
    RingBuffer<int8_t> sut(1000);
    int8_t data[1000];
    for (int8_t i = 0; i < 10; i++) {
        data[i] = i;
    }
    sut.produce(data, sut.getAvailableItemsToProduce() - 5);
    sut.consume(data, sut.getAvailableItemsToConsume());
    for (int8_t i = 0; i < 10; i++) {
        sut.produce(&i, 1);
    }
    for (int i = 0; i < 10; i++) {
        int8_t outValue;
        sut.consume(&outValue, 1);
        CHECK(outValue == i);
    }
}

TEST_CASE("RingBuffer<float> produce consume write and read one-by-one at the end") {
    RingBuffer<float> sut(1000);
    float data[1000];
    for (int8_t i = 0; i < 10; i++) {
        data[i] = i;
    }
    sut.produce(data, sut.getAvailableItemsToProduce() - 5);
    sut.consume(data, sut.getAvailableItemsToConsume());
    for (int i = 0; i < 10; i++) {
        auto val = static_cast<float>(i);
        sut.produce(&val, 1);
    }
    for (int i = 0; i < 10; i++) {
        float outValue;
        sut.consume(&outValue, 1);
        CHECK(outValue == i);
    }
}

TEST_CASE("RingBuffer<int8> produce consume write block at the end read one-by-one") {
    RingBuffer<int8_t> sut(1000);
    int8_t data[1000];
    for (int8_t i = 0; i < 10; i++) {
        data[i] = i;
    }
    sut.produce(data, sut.getAvailableItemsToProduce() - 5);
    sut.consume(data, sut.getAvailableItemsToConsume());
    sut.produce(data, 10);
    for (int i = 0; i < 10; i++) {
        int8_t outValue;
        sut.consume(&outValue, 1);
        CHECK(outValue == i);
    }
}

TEST_CASE("RingBuffer<float> produce consume write block at the end read one-by-one") {
    RingBuffer<float> sut(1000);
    float data[1000];
    for (int i = 0; i < 10; i++) {
        data[i] = static_cast<float>(i);
    }
    sut.produce(data, sut.getAvailableItemsToProduce() - 5);
    sut.consume(data, sut.getAvailableItemsToConsume());
    sut.produce(data, 10);
    for (int i = 0; i < 10; i++) {
        float outValue;
        sut.consume(&outValue, 1);
        CHECK(outValue == i);
    }
}

TEST_CASE("RingBuffer<int8> produce consume write one-by-one at the end read block") {
    RingBuffer<int8_t> sut(1000);
    int8_t data[1000];
    for (int8_t i = 0; i < 10; i++) {
        data[i] = i;
    }
    sut.produce(data, sut.getAvailableItemsToProduce() - 5);
    sut.consume(data, sut.getAvailableItemsToConsume());
    for (int8_t i = 0; i < 10; i++) {
        sut.produce(&i, 1);
    }
    int8_t outValue[10];
    sut.consume(outValue, 10);
    for (int i = 0; i < 10; i++) {
        CHECK(outValue[i] == i);
    }
}

TEST_CASE("RingBuffer<float> produce consume write one-by-one at the end read block") {
    RingBuffer<float> sut(1000);
    float data[1000];
    for (int i = 0; i < 10; i++) {
        data[i] = static_cast<float>(i);
    }
    sut.produce(data, sut.getAvailableItemsToProduce() - 5);
    sut.consume(data, sut.getAvailableItemsToConsume());
    for (int i = 0; i < 10; i++) {
        auto val = static_cast<float>(i);
        sut.produce(&val, 1);
    }
    float outValue[10];
    sut.consume(outValue, 10);
    for (int i = 0; i < 10; i++) {
        CHECK(outValue[i] == i);
    }
}

TEST_CASE("RingBuffer<int8> produce consume write and read block at the end") {
    RingBuffer<int8_t> sut(1000);
    int8_t data[1000];
    for (int8_t i = 0; i < 10; i++) {
        data[i] = i;
    }
    sut.produce(data, sut.getAvailableItemsToProduce() - 5);
    sut.consume(data, sut.getAvailableItemsToConsume());
    sut.produce(data, 10);
    int8_t outValue[10];
    sut.consume(outValue, 10);
    for (int i = 0; i < 10; i++) {
        CHECK(outValue[i] == i);
    }
}

TEST_CASE("RingBuffer<float> produce consume write and read block at the end") {
    RingBuffer<float> sut(1000);
    float data[1000];
    for (int i = 0; i < 10; i++) {
        data[i] = static_cast<float>(i);
    }
    sut.produce(data, sut.getAvailableItemsToProduce() - 5);
    sut.consume(data, sut.getAvailableItemsToConsume());
    sut.produce(data, 10);
    float outValue[10];
    sut.consume(outValue, 10);
    for (int i = 0; i < 10; i++) {
        CHECK(outValue[i] == i);
    }
}

TEST_CASE("RingBuffer<float> Writing and reading from ring buffer - MT") {
    RingBuffer<float> sut(1000);

    constexpr uint chunkSize = 30;
    constexpr std::size_t cycles = 10000;

    std::thread writerThread([&] {
        float i = 0;
        float inBuffer[chunkSize];
        while (i < cycles) {
            if (sut.getAvailableItemsToProduce() >= chunkSize) {
                std::fill_n(inBuffer, chunkSize, i);
                sut.produce(inBuffer, chunkSize);
                i++;
            }
        }
    });

    std::thread readerThread([&] {
        int expectedValue = 0;
        float outBuffer[chunkSize];
        while (expectedValue < cycles) {
            if (sut.getAvailableItemsToConsume() >= chunkSize) {
                sut.consume(outBuffer, chunkSize);
                for (const float i : outBuffer) {
                    CHECK(i == expectedValue);
                }
                expectedValue++;
            }
        }
    });

    writerThread.join();
    readerThread.join();

    CHECK(sut.getAvailableItemsToConsume() == 0);
}

}