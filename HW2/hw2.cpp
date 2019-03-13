#include "Simulator.h"

int main() {
    Simulator sim;
    sim.loadMemory("input/input1");
    sim.storeMemory("result1");
    sim.simulate();
    sim.storeMemory("result2");

    return 0;
}