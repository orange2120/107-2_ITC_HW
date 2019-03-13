import Simulator;

def main():
	sim = Simulator.Simulator();
	sim.loadMemory ("input/input1");
	sim.storeMemory ("result1");
	sim.simulate ();
	sim.storeMemory ("result2");

if __name__ == '__main__':
	main();
