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
      Simulator();

      ~Simulator() {}

      bool loadMemory(const string);
      bool storeMemory(const string);
      bool simulate();
      void reset();
      void printMemory() const;
      void printReg() const;

      void rotate(char &, uint8_t);
      void addFloat(char &, char &, char &);

      void assemblySim();
      bool storeAsm(const string);

    private:
      vector<char> programCounter;
      char regis[16] = {0};
      char input[256] = {0};
      char memory[256] = {0};
        
};

#endif