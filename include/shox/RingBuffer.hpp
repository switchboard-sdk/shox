//
//  RingBuffer.hpp
//
//  Created by Ivan Nador on 2025. 07. 10..
//  Copyright (c) 2025 Synervoz Communications Inc. All rights reserved.
//

#pragma once

#include <algorithm>
#include <atomic>
#include <cassert>

namespace shox {

/**
 * RingBuffer class.
 * @brief Class providing ring buffer functionality. Supports single-threaded and multi-threaded (single producer, single consumer) use cases.
 */
template <typename T>
class RingBuffer {
public:
    /**
     * @brief Creates a RingBuffer instance. Allocates the underlying buffer, not suitable for use in the real-time thread.
     *
     * @param numberOfItems The number of items that the ring buffer can hold.
     */
    explicit RingBuffer(uint32_t numberOfItems) : RingBuffer(numberOfItems, new T[numberOfItems + 1]) {
        ownsBuffer = true;
    }

    /**
     * @brief Creates a RingBuffer instance without allocating. Suitable for resizing a ring buffer in the real-time thread.
     * @details Does not delete the buffer on destruction. Make sure to handle deletion.
     *
     * @param numberOfItems The number of items that the ring buffer can hold.
     * @param buffer The underlying buffer the ring buffer is operating on.
     */
    RingBuffer(const uint32_t numberOfItems, T* buffer) :
        maxSize { numberOfItems },
        // One extra space was added for thread safe read/write pointer handling
        size { maxSize + 1 },
        buffer { buffer },
        ownsBuffer { false },
        readPointerIndex { 0 },
        writePointerIndex { 0 } {}

    /**
     * @brief RingBuffer destructor.
     */
    ~RingBuffer() {
        if (!ownsBuffer) {
            return;
        }
        delete[] buffer;
    }

    /**
     * @brief Clears all data in the ring buffer.
     */
    void clear() {
        readPointerIndex = 0;
        writePointerIndex = 0;
    }

    /**
     * @brief Returns the maximum number of items that can be added to the ring buffer.
     *
     * @returns The number of items that can be produced.
     */
    uint32_t getAvailableItemsToProduce() const {
        const uint32_t wp = writePointerIndex.load(std::memory_order_acquire);
        const uint32_t rp = readPointerIndex.load(std::memory_order_acquire);
        // Note that the one extra space is subtracted which was added for thread safety reasons.
        if (rp <= wp) {
            return size - (wp - rp) - 1;
        } else {
            return rp - wp - 1;
        }
    }

    /**
     * @brief Adds items to the ring buffer.
     *
     * @param sourceBuffer The buffer where the items are copied from.
     * @param numberOfItems The number of items that should be added to the ring buffer.
     */
    void produce(const T* sourceBuffer, const uint32_t numberOfItems) {
        if (numberOfItems == 0) {
            return;
        }
        assert(getAvailableItemsToProduce() >= numberOfItems);
        const uint32_t availableItemsTillEndOfBuffer = size - writePointerIndex;
        if (availableItemsTillEndOfBuffer >= numberOfItems) {
            memcpy(getWritePointer(), sourceBuffer, numberOfItems * sizeof(T));
        } else {
            memcpy(getWritePointer(), sourceBuffer, availableItemsTillEndOfBuffer * sizeof(T));
            const uint32_t leftOverItems = numberOfItems - availableItemsTillEndOfBuffer;
            const T* leftOverBuffer = sourceBuffer + availableItemsTillEndOfBuffer;
            memcpy(buffer, leftOverBuffer, leftOverItems * sizeof(T));
        }
        writePointerIndex = (writePointerIndex + numberOfItems) % size;
    }

    /**
     * @brief Returns the maximum number of items that can be read from the ring buffer.
     *
     * @returns The number items that can be consumed.
     */
    uint32_t getAvailableItemsToConsume() const {
        const uint32_t wp = writePointerIndex.load(std::memory_order_acquire);
        const uint32_t rp = readPointerIndex.load(std::memory_order_acquire);
        if (rp <= wp) {
            return wp - rp;
        } else {
            return size - (rp - wp);
        }
    }

    /**
     * @brief Reads items from the ring buffer and updates the read pointer.
     *
     * @param destBuffer [inout] The buffer where the read items will be copied to.
     * @param numberOfItems The number of items that should be consumed.
     */
    void consume(T* destBuffer, const uint32_t numberOfItems) {
        read(destBuffer, numberOfItems);
        discard(numberOfItems);
    }

    /**
     * @brief Reads items from the ring buffer without updating the read pointer.
     *
     * @param destBuffer [inout] The buffer where the read items will be copied to.
     * @param numberOfItems The number of items that should be consumed.
     */
    void read(T* destBuffer, const uint32_t numberOfItems) {
        assert(getAvailableItemsToConsume() >= numberOfItems);
        const uint32_t availableItemsTillEndOfBuffer = size - readPointerIndex;
        if (availableItemsTillEndOfBuffer >= numberOfItems) {
            memcpy(destBuffer, getReadPointer(), numberOfItems * sizeof(T));
        } else {
            memcpy(destBuffer, getReadPointer(), availableItemsTillEndOfBuffer * sizeof(T));
            const uint32_t leftOverItems = numberOfItems - availableItemsTillEndOfBuffer;
            T* leftOverBuffer = destBuffer + availableItemsTillEndOfBuffer;
            memcpy(leftOverBuffer, buffer, leftOverItems * sizeof(T));
        }
    }

    /**
     * @brief Discards items from the ring buffer. Steps the read pointer without reading the items.
     *
     * @param numberOfItems The number of items that should be discarded.
     */
    void discard(const uint32_t numberOfItems) {
        const uint32_t itemsToDiscard = std::min(getAvailableItemsToConsume(), numberOfItems);
        readPointerIndex = (readPointerIndex + itemsToDiscard) % size;
    }

private:
    uint32_t maxSize;
    uint32_t size;
    T* buffer;
    bool ownsBuffer;
    std::atomic<uint32_t> readPointerIndex;
    std::atomic<uint32_t> writePointerIndex;

    T* getWritePointer() {
        return buffer + writePointerIndex;
    }

    T* getReadPointer() {
        return buffer + readPointerIndex;
    }
};

}
