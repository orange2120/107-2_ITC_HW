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
        uint8_t test = 0;
        ifile.get(tmp);
        test = tmp;
        //cout << setfill('0') << setw(2) << hex << uppercase << (int16_t)(test & 0x00FF) << "] ";
        memory[i++] = (uint8_t)(test);
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

    //ofs.write(memory, 256);
    ofs.close();

    return true;
}

bool Simulator::simulate() {
    bool stop = false;
    int16_t pcIdx = 0x00;
    uint8_t *pc = memory; // input[0]
    int8_t opCode = 0;
    int8_t operand = 0;
    int8_t regS = 0;
    int8_t regT = 0;

    while (!stop)
    {
        opCode = (*pc & 0xF0) >> 4; // extract Op-code
        operand = *pc & 0x0F;
        regS = (*(pc + 1) & 0xF0) >> 4;
        regT = *(pc + 1) & 0x0F;
        int16_t t = *(pc + 1);

        // for debug
        cout << "PC: [" << setfill('0') << setw(2) << hex << uppercase << (int16_t)(pcIdx & 0x00FF) << "] ";
        cout << "0x" << uppercase << (long)pc << " ";
        cout << setfill('0') << setw(2) << hex << uppercase << (uint16_t)(*pc & 0x00FF) << '-';
        cout << setfill('0') << setw(2) << hex << uppercase << (uint16_t)(*(pc + 1) & 0x00FF) << endl;

        programCounter.push_back(pcIdx);

        switch (opCode)
        {
            
            case LOAD_MEM_TO_REG:
            {
                //int16_t t = *(pc + 1);
                regis[operand] = memory[t];
                break;
            }

            case LOAD_BIN_TO_REG:
                regis[operand] = *(pc + 1);
                break;

            case STORE:
                memory[t] = regis[operand];
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
                    pc = &memory[t]; // move to memory address XY
                    pcIdx = t;
                    continue;
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
        pcIdx += 0x2;

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

    cout << endl << "----------------------MEM----------------------" << endl;
    for (int i = 0; i < 256; i++)
    {
        cout << setfill('0') << setw(2) << hex << uppercase << (uint16_t)(memory[i] & 0x00FF) << " ";
        cnt++;
        if (cnt >= 16)
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
        cout << setfill('0') << setw(2) << hex << uppercase << (uint16_t)(regis[i] & 0x00FF) << " ";
    cout << endl;
}

inline void Simulator::rotate(uint8_t &regR, uint8_t times)
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

inline void Simulator::addFloat(uint8_t &regR, uint8_t &regS, uint8_t &regT)
{
    uint8_t signRes = 0, signS = (regS & 0x80) >> 7, signT = (regT & 0x80) >> 7;
    uint8_t expRes = 0, expS = (regS & 0x70) >> 4, expT = (regT & 0x70) >> 4;
    uint16_t mantRes = 0, mantS = regS & 0xF, mantT = regT & 0x0F;
    uint8_t delExp = expS - expT;

    // 對齊exponent
    if(delExp > 0) // S is larger, shifts T
    {
        
        mantS <<= delExp;
        expT = expS;
    }
    else
    {
        mantT <<= -delExp;
        expS = expT;
    }

    // S >> T
    if(delExp > 5)
    {
        regR = regS;
        return;
    }
    expRes = expS; // 讓結果位數一樣

    // 判斷正負號
    if (signS ^ signT) // 正負異號
    {
        if (mantS > mantT)
            mantRes = mantS - mantT;
        else
            mantRes = mantT - mantS;

        signRes = 1;
    }
    else
    {
        mantRes = mantS + mantT;
    }

    if (mantRes > 0xF)
    {
        mantRes >>= 1;
        expRes++;
    }

    regR = signRes << 7 | expRes << 4 | mantRes;
}

bool Simulator::loadAsm(const string path)
{
    ifstream ifs;
    /*if()
    {

    }*/
    return 0;
}

void Simulator::assemblySim()
{
    //ostream os;
}

bool Simulator::storeAsm(const string path)
{
    return true;
}