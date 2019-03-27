#include "Simulator.h"
#include <string>
#include <sstream>
#include <iostream>

int main(int argc, char* argv[]) 
{
    /*if (argc <= 1)
	{
		cerr << "[ERROR] Missing argument" << endl;
		return 0;
	}
    */
    Simulator sim;
    std::stringstream ss;
    ss << argv[1];
    string index = ss.str();

    if (!sim.loadAsm("input/bonus.txt"))
    {
        cerr << "[ERROR] Open file failed!" << endl;
        return 0;
    }
    //sim.simulate();
    /*if (!sim.storeMemory("output/asm_bonus"))
    {
        cerr << "[ERROR] Output file open failed" << endl;
        return 0;
    }
    */
    
    return 0;
}