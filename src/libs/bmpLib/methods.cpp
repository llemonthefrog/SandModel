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



void SetColor(uint64_t state, std::ofstream& stream) {
    /*
    Каждый цвет кодируется в формате blue - green - red

    То есть любое hex представление кодируется с конца
    */

    switch(state) {
        case 0:
            //white = 0xFFFFF0
            stream.put(0xF0);
            stream.put(0xFF);
            stream.put(0xFF);
            break;
        case 1:
            //green = 0x50C878
            stream.put(0x78);
            stream.put(0xC8);
            stream.put(0x50);
            break;
        case 2:
            //purple = 0x9966CC
            stream.put(0xCC);
            stream.put(0x66);
            stream.put(0x99);
            break;
        case 3:
            //yellow = 0xFFD700
            stream.put(0x00);
            stream.put(0xD7);
            stream.put(0xFF);
            break;
        default:
            //black = 0x000000
            stream.put(0x00);
            stream.put(0x00);
            stream.put(0x00);
            break;
    }
}

void WriteBMPbyGrid(const sandState& st, const char* out) {
    std::ofstream output;
    output.open(out, std::ios::binary);

    if(!output.is_open()) {
        std::cerr << "\n\u001b[31m[ERROR] problem in bpm writting\u001b[0m";
        exit(1);
    }

    stringBuf header = stringBuf();

    info("\u001b[34m{header writting}\u001b[0m\n");
    //заголовок
    header.pushBack('B');
    header.pushBack('M');

    uint32_t sizeOfImg = 54 + st.SizeX * 3 * st.SizeY;
    sizeOfImg += (4 - (st.SizeX * 3) % 4) % 4 * st.SizeY;

    for(int i = 0; i < 4; i++) {
        header.pushBack((sizeOfImg >> (i * 8)) & 0xFF);
    }
    
    //2 зарезервированных поля (2 по 2 байта)
    for(int i = 0; i < 4; i++) {
        header.pushBack(0x00);
    }

    //4 байта, откуда начинается массив значений
    header.pushBack(54);
    for(int i = 0; i < 3; i++) {
        header.pushBack(0x00);
    }

    //размер заголовка с информацией (4 байта)
    header.pushBack(40);
    for(int i = 0; i < 3; i++) {
        header.pushBack(0x00);
    }


    // Ширина
    uint32_t width = st.SizeX;
    for (int i = 0; i < 4; ++i) {
        header.pushBack(width & 0xFF);
        width >>= 8;
    }

    // Высота
    uint32_t height = st.SizeY;
    for (int i = 0; i < 4; ++i) {
        header.pushBack(height & 0xFF);
        height >>= 8;
    }

    //2 байта со значением 1
    header.pushBack(1);
    header.pushBack(0);

    //сколько бит используется для описания каждого пикселя - хранит в себе 24 (2 байта)
    header.pushBack(24);
    header.pushBack(0);    

    //6 полей по 4 пустых байта
    for(int i = 0; i < 24; i++) {
        header.pushBack(0x00);
    }

    output.write(header.returnString(), 54);
    output.flush();
    //

    info("\u001b[32m{done header writting}\u001b[0m\n");
    
    info("\u001b[34m{bits writting}\u001b[0m\n");
    for (int y = st.SizeY - 2; y >= 0; --y) {
        for (int x = 0; x < st.SizeX; ++x) {
            SetColor(st.Grid[y][x].state, output);
        }
        
        for (int padding = 0; padding < (4 - (st.SizeX * 3) % 4) % 4; padding++) {
            output.put(0x00);
        }
    }
    info("\u001b[32m{done bits writting}\u001b[0m\n");

    output.close();
}