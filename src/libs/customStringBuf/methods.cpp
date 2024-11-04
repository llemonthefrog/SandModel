#include "strBuf.h"

stringBuf::stringBuf(uint32_t size) {
    this->start = new char[size];
    this->size = size;
    this->written = 0;
}

stringBuf::stringBuf() {
    this->start = new char[5];
    this->size = 5;
    this->written = 0;
}

stringBuf::~stringBuf() {
    delete[] this->start;
    this->size = 0;
    this->written = 0;
}

char* stringBuf::returnString() {
    return this->start;
}

uint32_t stringBuf::getSize() {
    return this->written;
}

void stringBuf::pushBack(char ch) {
    if(this->start == nullptr) {
        this->start = new char[5];
        this->size = 5;
        this->written = 0;
    }

    if (this->written >= this->size) {
        char* p = new char[size * 2];
        for (uint32_t i = 0; i < this->written; ++i) {
            p[i] = this->start[i];
        }
        delete[] this->start;
        this->start = p;
        this->size = size * 2;
    }

    this->start[this->written++] = ch;
}

void stringBuf::printBuf() {
    for(int i = 0; i < this->written; i++) {
        std::cout << this->start[i];
    }
    std::cout << "\n";
}


void stringBuf::clear() {
    delete[] this->start;
    this->start = new char[5];
    this->written = 0;
    this->size = 5;
}
