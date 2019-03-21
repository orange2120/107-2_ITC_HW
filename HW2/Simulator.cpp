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

    char tmp = 0;
    int count = 0;

    //string tmp;
    //getline(ifile, tmp);

   

    while(!ifile.eof())
    {
        ifile.get(tmp);
        if (tmp < 0)
            tmp = ~tmp;
        cout << setw(2) << hex << (int)tmp << " ";
        count++; 
        if(count > 16) {  // 換行 
            cout << endl; 
            count = 0; 
        } 


        
    }   

    ifile.close();

    return true;
}

bool Simulator::storeMemory(const string path) {
    //TODO

    ofstream ofs;
    ofs.open(path.c_str(), ios::out | ios::trunc);

    ofs.close();
    return true;
}

bool Simulator::simulate() {
    //TODO

    return true;
}