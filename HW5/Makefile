CXXFLAGS= = -O3 -Wall -std=c++11
CXXFLAGS = -g -Wall -std=c++11

testHeap: heap/testHeap.cpp heap/heap.h
	@echo "Compiling testHeap..."
	g++ -o $@ heap/testHeap.cpp heap/heap.h $(CXXFLAGS)
	@echo "Done."

bonus: bonus/bonus.cpp
	@echo "Compiling bonus..."
	g++ -o bonus/$@ bonus/bonus.cpp $(CXXFLAGS)
	@echo "Done."

bonus_map: bonus/bonus_map.cpp
	@echo "Compiling bonus (map version)..."
	g++ -o bonus/$@ bonus/bonus_map.cpp $(CXXFLAGS)
	@echo "Done."

clean:
	rm -f bonus/bonus bonus/bonus_map testHeap heap/*.o heap/*.h.gch
	@echo "Done."