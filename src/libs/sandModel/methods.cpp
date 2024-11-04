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


constexpr uint32_t MAX_BUF_SIZE = 1024;

struct sandParser{
    uint16_t y = 0;
    uint16_t x = 0;
    uint64_t state = 0;
    sandParser* prev = nullptr;
};

struct sandQueue {
private:
    sandParser* head = nullptr;
    sandParser* tail = nullptr;
public:
    void enque(uint16_t x, uint16_t y, uint64_t state) {
        sandParser* elem = new sandParser();

        elem->x = x;
        elem->y = y;
        elem->state = state;

        if(head == nullptr) {
            head = elem;
            tail = elem;
        } else {
            elem->prev = nullptr;
            tail->prev = elem;
            tail = elem;
        }
    }

    sandParser* dequeue() {
        if(head == tail) {
            sandParser* elem = head;
            head = nullptr;
            tail = nullptr;

            return elem;
        }

        sandParser* elem = head;
        head = head->prev;

        return elem;
    }
    
    bool isEmpty() {
        return(head == nullptr);
    }
};


void sandState::LifeCycle(const args& arguments) {
    bool unstable = true;

    info("\n\u001b[34m{start lifecycle}\u001b[0m\n");
    for(int i = 0; i < arguments.MaxItter && unstable; i++) {
        #if __DEBUG__
        if(i % 10000 == 0) {
            debug("iteration number: %d\n", i);
        }
        #endif
        unstable = false;
        for(uint16_t y = 0; y < this->SizeY; y++) {
            for(uint16_t x = 0; x < this->SizeX; x++) {
                if(this->Grid[y][x].state > 3) {
                    this->Topple(x, y);
                    unstable = true;
                }
            }
        }

        if(arguments.Freq != 0 && i % arguments.Freq == 0) {
            std::filesystem::path dir("bmpImages");
            if (!std::filesystem::exists(dir)) {
                std::filesystem::create_directory(dir);
            }

            char buf[256];
            snprintf(buf, (sizeof(buf)/sizeof(buf[0])) - 1 ,"bmpImages/%d-%s", i, arguments.Output);

            WriteBMPbyGrid(*this, buf);
        }
    }
    info("\n\u001b[32m{end lifecycle}\u001b[0m");
}

void sandState::Topple(uint16_t& x, uint16_t& y) {
    if (this->Grid[y][x].state > 3) {
        if(x == 0 || x == this->SizeX - 1 || y == 0 || y == this->SizeY - 1) {
            int newSizeX = this->SizeX + 2;
            int newSizeY = this->SizeY + 2;
            sand** p = new sand*[newSizeY];
            for(int i = 0; i < newSizeY; i++) {
                p[i] = new sand[newSizeX]();
            }

            for(int y1 = 0; y1 < this->SizeY; y1++) {
                for(int x1 = 0; x1 < this->SizeX; x1++) {
                    p[y1 + 1][x1 + 1].state = this->Grid[y1][x1].state;
                }
            }

            for(int i = 0; i < this->SizeY; i++) {
                delete[] this->Grid[i];
            }
            delete[] this->Grid;

            this->Grid = p;
            this->SizeX = newSizeX;
            this->SizeY = newSizeY;

            x++;
            y++;
        }

        this->Grid[y + 1][x].state++;
        this->Grid[y - 1][x].state++;
        this->Grid[y][x + 1].state++;
        this->Grid[y][x - 1].state++;

        this->Grid[y][x].state -= 4;
    }
}

void sandState::ParseTsv(const char* inputPath) {
    std::ifstream input;
    input.open(inputPath);
    if(!input.is_open()) {
        std::cerr << "\n\u001b[31m[ERROR] problem in opening file\u001b[0m\n";
        exit(1);
    }

    sandQueue snd{};
    uint16_t maxX = 0;
    uint16_t maxY = 0;

    info("\n\u001b[34m{tsv file}\u001b[0m");

    char buf[MAX_BUF_SIZE];
    while(!input.eof()) {
        uint64_t rows[3];
        for(int i = 0; i < 3; i++) {
            rows[i] = 0;
        }

        input.getline(buf, MAX_BUF_SIZE);
        uint32_t size = input.gcount();

        uint32_t i = 0;
        uint32_t rowsCount = 0;

        stringBuf strBf = stringBuf();
        while(i < size) {
            if(buf[i] == '\t') {
                rows[rowsCount] = atol(strBf.returnString());
                rowsCount++;
                strBf.clear();
            } else {
                strBf.pushBack(buf[i]);
            }
            i++;
        }

        if(strBf.getSize() != 0) {
            rows[rowsCount] = atol(strBf.returnString());
        }

        if(static_cast<uint16_t>(rows[0]) > maxX) {
            maxX = static_cast<uint16_t>(rows[0]);
        }

        if(static_cast<uint16_t>(rows[1]) > maxY) {
            maxY = static_cast<uint16_t>(rows[1]);
        }

        snd.enque(static_cast<uint16_t>(rows[0]), static_cast<uint16_t>(rows[1]), rows[2]);
    }
    debug("\nmax x - %d, max y - %d\n\n", maxX + 1, maxY + 1);

    this->SizeX = maxX + 1;
    this->SizeY = maxY + 1;

    this->Grid = new sand*[maxY + 1];
    for(int i = 0; i < maxY + 1; i++) {
        this->Grid[i] = new sand[maxX + 1]();
    }

    info("\u001b[34m{convert}\u001b[0m");
    while(!snd.isEmpty()) {
        sandParser* elem = snd.dequeue();

        this->Grid[elem->y][elem->x].state = elem->state;

        delete elem;
    }
    info("\n\u001b[32m{done convert}\u001b[0m");

    input.close();
}
