#include "Simulator.h"
#include <string>
#include <sstream>
#include <iostream>

int main(int argc, char* argv[]) 
{
    if (argc <= 1)
	{
		cerr << "[ERROR] Missing argument" << endl;
		return 0;
	}
    
    Simulator sim;

    std::stringstream ss;
    ss << argv[1];
    string index = ss.str();

    if (!sim.loadMemory("input/input" + index))
    {
        cerr << "[ERROR] Open file failed!" << endl;
        return 0;
    } 
    sim.simulate();
    if (!sim.storeMemory("output/output" + index))
    {
        cerr << "[ERROR] Output file open failed" << endl;
        return 0;
    }
    return 0;
}