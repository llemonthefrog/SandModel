#include <iostream>
#include <stdio.h>

#include "parser.h"
#include "sand.h"
#include "bmp.h"


#if __DEBUG__
    #define debug(format, ...) printf(format, __VA_ARGS__)
    #define printParam(paramName, param) std::cout << info << " " << param << "\n"
    #define info(info) std::cout << info << "\n"
#else 
    #define debug(format, ...) 
    #define info(info) 
    #define printParam(paramName, param)
#endif


int main(int argc, char* argv[]) {
    args arguments{};
    arguments.ParseArgs(argc, argv);

    info("\u001b[34m{parsed args}\u001b[0m\n");
    printParam("input:", arguments.Input);
    printParam("output:", arguments.Output);
    printParam("max-itters:", arguments.MaxItter);
    printParam("freq:", arguments.Freq);

    if(arguments.Output == nullptr) {
        std::cerr << "\n\u001b[31m[ERROR] outpud doesn't provided\u001b[0m\n";
        exit(1);
    }

    sandState state{};
    state.ParseTsv(arguments.Input);

    info("\n\u001b[34m{creating a grid}\u001b[0m");
    state.LifeCycle(arguments);
    info("\n\u001b[32m{done creating grid}\u001b[0m");

    info("\n\u001b[34m{writing to the bmp}\u001b[0m\n");
    WriteBMPbyGrid(state, arguments.Output);  
}
