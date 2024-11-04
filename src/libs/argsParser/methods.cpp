#include "parser.h"

void args::Flag(char* arg, char* value) {
    if(strcmp(arg, "-i") == 0) {
        this->Input = value;
    } else if(strcmp(arg, "-o") == 0) {
        this->Output = value;
    } else if(strcmp(arg, "-m") == 0) {
        this->MaxItter = atoi(value);
    } else if(strcmp(arg, "-f") == 0)  {
        this->Freq = atoi(value);
    }
}

void args::LongFlag(char* arg) {

    uint32_t size;
    uint32_t equalIndex = 0;
    for(size = 0; arg[size] != '\0'; size++){
        if(arg[size] == '=') {
            equalIndex = size;
        }
    }

    char subStrArg[equalIndex + 1];
    subStrArg[equalIndex] = '\0';
    char* subStrVal = new char[(size - equalIndex) + 1];
    subStrVal[(size - equalIndex) + 1] = '\0';

    for(int i = 0; i < equalIndex; i++) {
        subStrArg[i] = arg[i];
    }

    for(int i = 0; i < size - equalIndex; i++) {
        subStrVal[i] = arg[equalIndex + 1 + i];
    }

    if(strcmp(subStrArg, "--input") == 0) {
        this->setInput(subStrVal, true);
    } else if(strcmp(subStrArg, "--output") == 0) {
        this->setOutput(subStrVal, true);
    } else if(strcmp(subStrArg, "--max-iter") == 0) {
        this->MaxItter = atol(subStrVal);
        delete[] subStrVal;
    } else if(strcmp(subStrArg, "--freq") == 0) {
        this->Freq = atol(subStrVal);
        delete[] subStrVal;
    }
}

void args::ParseArgs(uint32_t argc, char* argv[]) {
    for(int i = 1; i < argc; i++) {
        if(argv[i][0] == '-' && argv[i][1] == '-') {
            this->LongFlag(argv[i]);
        } else if(argv[i][0] == '-') {
            this->Flag(argv[i], argv[i + 1]);
            i++;
        }
    }
    
};
