#include "Simulator.h"
#include <iostream>
#include <fstream>
using namespace std;

bool Simulator::loadMemory(string path) {
    ifstream ifile;
    ifile.open(path.c_str(), ios::in);
    string tmp;
    getline(ifile, tmp);
    int a = (int)tmp[0];
    for(int i=0; i<8; i++){
        int t = 0x01;
        t = (t<<i);
        cout<<(a&t)<<endl;
    }
}
bool Simulator::storeMemory(string path) {
    //TODO
}
bool Simulator::simulate() {
    //TODO
}