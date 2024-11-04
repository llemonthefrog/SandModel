#pragma once

#include <cstring>
#include <cinttypes>
#include <iostream>

struct stringBuf {
private:
    char* start = nullptr;
    uint32_t size = 0;
    uint32_t written = 0;
public:
    stringBuf(uint32_t);
    stringBuf();

    ~stringBuf();

    char operator[](uint32_t index) {
        if(index < written) {
            return this->start[index];
        } 

        return ' ';
    }

    uint32_t getSize();
    void pushBack(char);
    void printBuf();
    char* returnString();
    void clear();
};
