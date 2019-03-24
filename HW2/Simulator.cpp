#include "Simulator.h"
#include <iostream>
#include <fstream>
#include <iomanip> 
using namespace std;

bool Simulator::loadMemory(const string path) {
    ifstream ifile;
    ifile.open(path.c_str(), ios::in | ios::binary);

    if (!ifile.is_open())
        return false;

    unsigned int fileSize;
    unsigned int i = 0;
    char tmp = 0;
    int count = 0;

    // Determine file size
    ifile.seekg(0, ios::end);
    fileSize = ifile.tellg();
    cout << "File Size: " << fileSize << " bytes." << endl;
    ifile.seekg(0);

    input.resize(fileSize);

    while (!ifile.eof())
    {
        ifile.get(tmp);
        //cout << "i: " << i << endl;
        input[i] = (char)tmp;
        i++;
        
        int16_t a = tmp & (0x00FF);
        cout << setw(2) << hex << a << " ";
        count++;
        if (count > 16)
        { // 換行
            cout << endl;
            count = 0;
        }
    }

    ifile.close();

    cout << endl;

    //printMemory(input);

    return true;
}

bool Simulator::storeMemory(const string path) {
    //TODO

    ofstream ofs;
    ofs.open(path.c_str(), ios::trunc | ios::binary);
    for (int i = 0; i < 16;i++)
    {
        //ofs.write((char)regis[i]);
    }
    for (int i = 0; i < 240; i++)
    {
        //ofs.write(0x0, );
    }

    ofs.close();
    return true;
}

bool Simulator::simulate() {
    //TODO
    unsigned int inputIdx = 0;
    char regIdx = 0;
    char pc[2] = {0};
    char operand = 0;
    char opCode = 0;

    //while (pc[0] != 0xC0 )
    pc[0] = input[0];
    pc[1] = input[1];
    opCode = ((pc[0] & 0xF0) >> 4); // extract Op-code
    operand = pc[0] & 0x0F;

    /*int16_t a = pc[0] & (0x00FF);
    cout << setw(2) << hex << a << " " << endl;

    int16_t b = instruction & (0x00FF);
    cout  << "in" << setw(2) << hex << b << " " << endl;
    */
   //TODO
   //照順序執行

    programCounter.push_back(pc[i]);

    switch (opCode)
    {
        case LOAD_MEM_TO_REG:
            //loadMemToReg(pc[i], pc[i+1]);
            regIdx = getRegIndex(pc[i]);
            regis[regIdx] = memory[pc[i + 1]];
            break;

        case LOAD_BIN_TO_REG:
            //loadBinToReg(pc[i], pc[i+1]);
            regIdx = getRegIndex(pc[i]);
            regis[regIdx] = pc[i + 1];
            break;

        case STORE:
        /* code */
        break;

        case MOVE:
        /* code */
        break;

        case ADD_TWO_COMPL:
        /* code */
        break;
        case ADD_FLOAT:
        /* code */
        break;

        case OR:
        /* code */
        break;
        
        case AND:
        /* code */
        break;

        case EXCLUSIVE_OR:
        /* code */
        break;

        case ROTATE:
        /* code */
        break;
        
        case JUMP:
        /* code */
        break;
        
        case HALT:
        /* code */
        break;
        
        case 0x0C:
        /* code */
        break;
    
        default:
            break;
    }



    return true;
}

void Simulator::printMemory(const vector<char> &v)
{
    int cnt = 0;
    for (int i = 0; i < v.size(); i++)
    {
        int16_t a = v[i] & (0x00FF);
        cout << setw(2) << hex << a << " ";
        cnt++;
        if (cnt > 16)
        {
            cout << endl;
            cnt = 0;
        }
    }
    cout << endl;
}

inline char Simulator::getRegIndex(const char &reg)
{
    return (reg & 0x0F);
}

inline void Simulator::loadMemToReg(char &reg, const char &mem)
{
    reg = mem;
}

inline void Simulator::loadBinToReg(char &reg, const char bin)
{
    reg = bin;
}

inline void Simulator::StoreToMem(const char &reg, char &mem)
{
    mem = reg;
}

inline void Simulator::Move(char &regR, char &regS)
{
    regS = regR;
}

inline void Simulator::Add2sComp(char &regS, char &regT, char &regR)
{
    regR = regS + regT;
}

void Simulator::AddFloat(char &regS, char &regT, char &regR)
{

}

inline void Simulator::OR(char &regS, char &regT, char &regR)
{

}

void Simulator::AND(char &regS, char &regT, char &regR)
{

}

void Simulator::ExclusiveOR(char &regS, char &regT, char &regR)
{

}

void Simulator::rotate(char &regR, char times)
{

}

bool Simulator::jump(const char &regR, const char &mem)
{

}