#include "Simulator.h"
#include <iostream>
#include <fstream>
#include <iomanip> 
#include <cstdlib>
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
    char c = 0;

    // Determine the file size
    ifile.seekg(0, ios::end);
    fileSize = ifile.tellg();
    cout << "File Size: " << fileSize << " bytes." << endl;
    ifile.seekg(0);

    // Read file in
    while (!ifile.eof())
    {
        ifile.get(c);
        memory[i++] = (uint8_t)(c);
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
    // Write file in
    for (int i = 0; i < 256; i++)
        ofs.put((char)memory[i]);
    
    ofs.close();

    return true;
}

bool Simulator::simulate() {
    bool stop = false;
    int16_t pcIdx = 0x00;
    int16_t memAddr = 0x00;
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
        memAddr = *(pc + 1);

        // for debug
        cout << "PC: [" << setfill('0') << setw(2) << hex << uppercase << (int16_t)(pcIdx & 0x00FF) << "] ";
        cout << "0x" << uppercase << (long)pc << " ";
        cout << setfill('0') << setw(2) << hex << uppercase << (uint16_t)(*pc & 0x00FF) << '-';
        cout << setfill('0') << setw(2) << hex << uppercase << (uint16_t)(*(pc + 1) & 0x00FF) << endl;

        programCounter.push_back(pcIdx); // Record program counter

        switch (opCode)
        {
            
            case LOAD_MEM_TO_REG:
                regis[operand] = memory[memAddr];
                break;

            case LOAD_BIN_TO_REG:
                regis[operand] = *(pc + 1);
                break;

            case STORE:
                memory[memAddr] = regis[operand];
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
                    pc = &memory[memAddr]; // move to memory address XY
                    pcIdx = memAddr;
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
    for (int i = 0; i < 256; ++i)
    {
        cout << setfill('0') << setw(2) << hex << uppercase << (uint16_t)(memory[i] & 0x00FF) << " ";
        cnt++;
        if (cnt >= 16)
        {
            cout << endl;
            cnt = 0;
        }
    }
    cout << "----------------------END----------------------" << endl;
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
    else // T is larger
    {
        mantT <<= -delExp;
        expS = expT;
    }

    // S >> T
    if(delExp > 5)
    {
        regR = regS;
        expRes = expS;
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

    // 相加之後 > 2 捨去一位
    if (mantRes > 0xF)
    {
        mantRes >>= 1;
        expRes++;
    }

    regR = signRes << 7 | expRes << 4 | mantRes;
}

bool Simulator::loadAsm(const string path)
{
    reset();

    ifstream ifs;
    ifs.open(path.c_str(), ifstream::in);

    if (!ifs.is_open())
        return false;

    vector<string> input;
    string str_s = "";
    char str[128] = {0};
    unsigned int n = 0;

    while (ifs.getline(str, 256 ,'\n'))
    {
        str_s.assign(str);
        input.push_back(str);
    }

    n = input.size();

    //cout << "n: " << n << endl;

    for (unsigned int i = 0; i < n; ++i)
    {
        string pch = "";
        size_t strN = 0;
        uint8_t r = 0, s = 0, t = 0;
        uint8_t xy = 0;

        cout << input[i] << endl;

       strN = myStrGetTok(input[i], pch , 0, ' '); // Get instruction 
        
        if(myStrNCmp("lw", pch, 2) == 0)
        {
            strN = myStrGetTok(input[i], pch , strN, ' ');
            r = (uint8_t)strtoul(pch.c_str(), 0, 16);

            strN = myStrGetTok(input[i], pch , strN, ' ');
            xy = (uint8_t)strtoul(pch.c_str(), 0, 16);

            memory[2 * i] = 0x10 | r;
            memory[2 * i + 1] = xy;
        }
        else if (myStrNCmp("lb", pch, 2) == 0)
        {
            strN = myStrGetTok(input[i], pch , strN, ' ');
            r = (uint8_t)strtoul(pch.c_str(), 0, 16);

            strN = myStrGetTok(input[i], pch , strN, ' ');
            xy = (uint8_t)strtoul(pch.c_str(), 0, 16);

            memory[2 * i] = 0x20 | r;
            memory[2 * i + 1] = xy;

            cout << "r:" << (uint16_t)(r & 0xFF) << ",xy:" <<(uint16_t)(xy & 0xFF) << endl;
        }
        else if (myStrNCmp("sw", pch, 2) == 0)
        {
            strN = myStrGetTok(input[i], pch , strN, ' ');
            r = (uint8_t)strtoul(pch.c_str(), 0, 16);

            strN = myStrGetTok(input[i], pch , strN, ' ');
            xy = (uint8_t)strtoul(pch.c_str(), 0, 16);

            memory[2 * i] = 0x30 | r;
            memory[2 * i + 1] = xy;
        }
        else if (myStrNCmp("mv", pch, 2) == 0)
        {
            strN = myStrGetTok(input[i], pch , strN, ' ');
            r = (uint8_t)strtoul(pch.c_str(), 0, 16);

            strN = myStrGetTok(input[i], pch , strN, ' ');
            s = (uint8_t)strtoul(pch.c_str(), 0, 16);

            memory[2 * i] = 0x40;
            memory[2 * i + 1] = (r << 4) | s;
        }
        else if (myStrNCmp("add", pch, 3) == 0)
        {
            getRST(strN, input[i], pch, r, s, t);

            memory[2 * i] = 0x50 | r;
            memory[2 * i + 1] = (s << 4) | t;
        }
        else if (myStrNCmp("addf", pch, 4) == 0)
        {
            getRST(strN, input[i], pch, r, s, t);

            memory[2 * i] = 0x60 | r;
            memory[2 * i + 1] = (s << 4) | t;
        }
        else if (myStrNCmp("or", pch, 2) == 0)
        {
            getRST(strN, input[i], pch, r, s, t);

            memory[2 * i] = 0x70 | r;
            memory[2 * i + 1] = (s << 4) | t;
        }
        else if (myStrNCmp("and", pch, 3) == 0)
        {
            getRST(strN, input[i], pch, r, s, t);

            memory[2 * i] = 0x80 | r;
            memory[2 * i + 1] = (s << 4) | t;

        }
        else if (myStrNCmp("xor", pch, 3) == 0)
        {
            getRST(strN, input[i], pch, r, s, t);

            memory[2 * i] = 0x90 | r;
            memory[2 * i + 1] = (s << 4) | t;

        }
        else if (myStrNCmp("srl", pch, 3) == 0)
        {
            strN = myStrGetTok(input[i], pch , strN, ' ');
            strN = myStrGetTok(input[i], pch , strN, ' ');
            r = (uint8_t)strtoul(pch.c_str(), 0, 16);

            strN = myStrGetTok(input[i], pch , strN, ' ');
            xy = (uint8_t)strtoul(pch.c_str(), 0, 16);

            memory[2 * i] = 0xA0 | r;
            memory[2 * i + 1] = xy;
        }
        else if (myStrNCmp("beq", pch, 3) == 0)
        {
            strN = myStrGetTok(input[i], pch , strN, ' ');
            strN = myStrGetTok(input[i], pch , strN, ' ');
            r = (uint8_t)strtoul(pch.c_str(), 0, 16);

            strN = myStrGetTok(input[i], pch , strN, ' ');
            xy = (uint8_t)strtoul(pch.c_str(), 0, 16);

            memory[2 * i] = 0xB0 | r;
            memory[2 * i + 1] = xy;
        }
        else if (myStrNCmp("halt\r", pch, 4) == 0)
        {
            memory[2 * i] = 0xC0;
            memory[2 * i + 1] = 0x00;
        }
    }

    return true;
}


// Private member functions
int Simulator::myStrNCmp(const string& s1, const string& s2, unsigned n)
{
   unsigned n2 = s2.size();
   if (n2 == 0) return -1;
   unsigned n1 = s1.size();
   for (unsigned i = 0; i < n1; ++i) {
      if (i == n2)
         return (i < n)? 1 : 0;
      char ch1 = (isupper(s1[i]))? tolower(s1[i]) : s1[i];
      char ch2 = (isupper(s2[i]))? tolower(s2[i]) : s2[i];
      if (ch1 != ch2)
         return (ch1 - ch2);
   }
   return (n1 - n2);
}

size_t Simulator::myStrGetTok(const string& str, string& tok, size_t pos = 0, const char del = ' ')
{
   size_t begin = str.find_first_not_of(del, pos);
   if (begin == string::npos) { tok = ""; return begin; }
   size_t end = str.find_first_of(del, begin);
   tok = str.substr(begin, end - begin);
   return end;
}

inline void Simulator::getRST(size_t &strN, const string &input,string &pch, uint8_t &r, uint8_t &s, uint8_t &t)
{
    strN = myStrGetTok(input, pch , strN, ' ');
    r = (uint8_t)strtoul(pch.c_str(), 0, 16);

    strN = myStrGetTok(input, pch , strN, ' ');
    s = (uint8_t)strtoul(pch.c_str(), 0, 16);

    strN = myStrGetTok(input, pch , strN, ' ');
    t = (uint8_t)strtoul(pch.c_str(), 0, 16);
}