#pragma once

#include <cinttypes>
#include <fstream>
#include <iostream>
#include <cstring>
#include <filesystem>

#include "parser.h"
#include "strBuf.h"

struct sand {
    uint64_t state = 0;
};

struct sandState {
    sand** Grid = nullptr;
    uint16_t SizeY = 0;
    uint16_t SizeX = 0;

    void ParseTsv(const char* input);
    void LifeCycle(const args&);

    ~sandState() {
        for(int16_t i = 0; i < SizeY; i++) {
            delete[] Grid[i];
        }

        delete[] Grid;
    }
private:
    void Topple(uint16_t& x, uint16_t& y);
};
