#include "Simulator.h"
#include <iostream>
#include <fstream>
using namespace std;

bool Simulator::loadMemory(const string path) {
    ifstream ifile;
    ifile.open(path.c_str(), ios::in);

    if (!ifile.is_open())
        return false;

    string tmp;
    getline(ifile, tmp);
    int a = (int)tmp[0];
    for(int i=0; i<8; i++){
        int t = 0x01;
        t = (t<<i);
        cout<<(a&t)<<endl;
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