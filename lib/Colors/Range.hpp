#ifndef RANGE_HPP
#define RANGE_HPP

#include <Arduino.h>

class Range {
public:
    virtual void forEach(std::function<void(uint16_t)> cb) = 0;
};

class StartEndRange: public Range {
    uint16_t start;
    uint16_t end;
public:
    StartEndRange(uint16_t start, uint16_t end) : start(start), end(end) {}

    void forEach(std::function<void(uint16_t)> cb) override {
        for (uint16_t i = start; i < end; i++) cb(i);
    }
};

#endif
