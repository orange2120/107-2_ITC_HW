#include "Simulator.h"
#include <iostream>
#include <fstream>
#include <iomanip> 
using namespace std;

Simulator::Simulator()
{
    reset();
}

bool Simulator::loadMemory(const string path) {

    reset();

    ifstream ifile;
    ifile.open(path.c_str(), ifstream::in | ifstream::binary);

    if (!ifile.is_open())
        return false;

    unsigned int fileSize;
    unsigned int i = 0;
    char tmp = 0;

    // Determine file size
    ifile.seekg(0, ios::end);
    fileSize = ifile.tellg();
    cout << "File Size: " << fileSize << " bytes." << endl;
    ifile.seekg(0);

    while (!ifile.eof())
    {
        ifile.get(tmp);
        //memory[i++] = (char)tmp;
        input[i++] = (char)tmp;
    }

    printMemory();

    ifile.close();

    return true;
}

bool Simulator::storeMemory(const string path) {

    printMemory(); // For debug

    ofstream ofs;
    ofs.open(path.c_str(), ofstream::trunc | ofstream::binary);

    if(!ofs.is_open())
        return false;

    ofs.write(memory, 256);
    ofs.close();

    return true;
}

bool Simulator::simulate() {
    bool stop = false;
    int8_t pcIdx = 0x00;
    char *pc = input; // input[0]
    char opCode = 0;
    int8_t operand = 0;
    int8_t regS = 0;
    int8_t regT = 0;

    while (!stop)
    {
        memory[pcIdx] = *pc;
        memory[pcIdx + 1] = *(pc + 1);
        opCode = (*pc & 0xF0) >> 4; // extract Op-code
        operand = *pc & 0x0F;
        regS = (*(pc + 1) & 0xF0) >> 4;
        regT = *(pc + 1) & 0x0F;

        // for debug
        cout << "PC:[" << setfill('0') << setw(2) << hex << uppercase << (int16_t)(pcIdx & 0x00FF) << "] ";
        cout << setfill('0') << setw(2) << hex << uppercase << (int16_t)(*pc & 0x00FF) << '-';
        cout << setfill('0') << setw(2) << hex << uppercase << (int16_t)(*(pc + 1) & 0x00FF) << endl;

        programCounter.push_back(pcIdx);

        switch (opCode)
        {
            case LOAD_MEM_TO_REG:
                regis[operand] = memory[(int8_t)*(pc + 1)];
                break;

            case LOAD_BIN_TO_REG:
                regis[operand] = *(pc + 1);
                break;

            case STORE:
                memory[(int8_t)*(pc + 1)] = regis[operand];
                break;

            case MOVE:
                regis[regT] = regis[regS];
                break;

            case ADD_TWO_COMPL:
                regis[operand] = regis[regS] + regis[regT];
                break;

            case ADD_FLOAT:
                addFloat(regis[operand], regis[regS], regis[regT]);
                break;

            case OR:
                regis[operand] = regis[regS] | regis[regT];
                break;
            
            case AND:
                regis[operand] = regis[regS] & regis[regT];
                break;

            case EXCLUSIVE_OR:
                regis[operand] = regis[regS] ^ regis[regT];
                break;

            case ROTATE:
                rotate(regis[operand], regT);
                break;
            
            case JUMP:
                if(regis[operand] == regis[0])
                {
                    pc = &memory[(int8_t)*(pc + 1)]; // move to memory address XY
                    pcIdx = *(pc + 1) - 2;
                }
                break;
            
            case HALT:
                if (operand == 0 && (*(pc + 1) == 0x00))
                {
                    stop = true;
                    cout << "HALT" << endl;
                }
                break;
        }
        pc += 2; // Move to next instruction
        pcIdx += 2;

        printReg();
    }

    return true;
}

void Simulator::reset()
{
    // reset memory and register
    for (int i = 0; i < 256; ++i)
        memory[i] = 0;
    for (int i = 0; i < 16; ++i)
        regis[i] = 0;
}

void Simulator::printMemory() const 
{
    int cnt = 0;

    cout << endl << "------------------------MEM------------------------" << endl;
    for (int i = 0; i < 256; i++)
    {
        cout << setfill('0') << setw(2) << hex << uppercase << (int16_t)(memory[i] & (0x00FF)) << " ";
        cnt++;
        if (cnt > 16)
        {
            cout << endl;
            cnt = 0;
        }
    }
    cout << endl;
}

void Simulator::printReg() const
{
    cout << "REG: ";
    for (int i = 0; i < 16; i++)
        cout << setfill('0') << setw(2) << hex << uppercase << (int16_t)(regis[i] & 0x00FF) << " ";
    cout << endl;
}

inline void Simulator::rotate(char &regR, uint8_t times)
{
    char lsb = 0;
    for (uint8_t i = 0; i < times; ++i)
    {
        lsb = regR & 0x01;
        regR >>= 1;
        lsb <<= 8;
        regR |= lsb;
    }
}

inline void Simulator::addFloat(char &regR, char &regS, char &regT)
{
    char signS = regS & 0x80, signT = regT & 0x80;
    char expS = regS & 0x70, expT = regT & 0x70;
    if(expS > expT)
    {
        
    }

}

void Simulator::assemblySim()
{
    //ostream os;
}

bool Simulator::storeAsm(const string path)
{
    return true;
}