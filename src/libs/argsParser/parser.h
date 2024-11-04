#pragma once

#include <iostream>
#include <cinttypes>
#include <cstring>


struct args {
    char* Input = nullptr; // Можно использовать указатели, существуют весь цикл жизни программы, так как находятся в argv
    char* Output = nullptr; // 
    uint32_t MaxItter = 0;
    uint32_t Freq = 0;

    void ParseArgs(uint32_t, char**);

    void setInput(char* input, bool clean) {
        this->Input = input;
        this->CleanInput = clean;
    }

    void setOutput(char* output, bool clean) {
        this->Output = output;
        this->CleanOutput = clean;
    }

    void operator=(args arguments) {
        this->Input = arguments.Input;
        this->Freq = arguments.Freq;
        this->Output = arguments.Output;
        this->MaxItter = arguments.MaxItter;
    };

    ~args() {
        if(CleanInput) {
            delete[] this->Input;
        }

        if(CleanOutput) {
            delete[] this->Output;
        }
    }
    
private:
    void Flag(char* arg, char* value);
    void LongFlag(char*);

    bool CleanOutput = false;
    bool CleanInput = false;
};
