#ifndef __SIMULATOR__
#define __SIMULATOR__

#include <string>
#include <vector>
using namespace std;

enum Instructions
{
    LOAD_MEM_TO_REG = 0x1,
    LOAD_BIN_TO_REG = 0x2,
    STORE = 0x3,
    MOVE = 0x4,
    ADD_TWO_COMPL = 0x5,
    ADD_FLOAT = 0x6,
    OR = 0x7,
    AND = 0x8,
    EXCLUSIVE_OR = 0x9,
    ROTATE = 0xA,
    JUMP = 0xB,
    HALT = 0xC
};

class Simulator
{
    public:
        Simulator()
        {
            programCounter.resize(8, 0);
            
            //regis.resize(5);
            //for(uint32_t i=0; i<regis.size(); i++){
            //    regis[i].resize(8, 0);
            //}
        }

        ~Simulator() {}

        bool loadMemory(const string);
        bool storeMemory(const string);
        bool simulate();
        void printMemory(const vector<char> &);

        void loadMemToReg();
        void loadBinToReg();
        void StoreToMem();
        void Move();

      private:
        vector<char> programCounter;
        vector<char> input;
        //vector< vector<bool> > regis;
        char regis[16];
        //vector< vector<bool> > memory;
        char memory[256] = {0};
        
};

#endif