#include "Simulator.h"
#include <string>
#include <sstream>
#include <iostream>

int main() {
    Simulator sim;
    
    for(int i = 1 ; i < 7 ; i++)
    {
        string index;
        std::stringstream ss;
        ss << i;
        index = ss.str();

        cout << "Input: " << i << endl;

        if (!sim.loadMemory("input/input"+index))
        {
            cerr << "[ERROR] Open file failed!" << endl;
            return 0;
        } 
        sim.simulate();
        if (!sim.storeMemory("output/output"+index))
        {
            cerr << "[ERROR] Output file open failed" << endl;
            return 0;
        }
        sim.storeMemory("output/output"+index);
        cout << "Simulated." << endl;
    }

    return 0;
}
