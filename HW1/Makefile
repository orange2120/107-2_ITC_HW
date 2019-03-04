CFLAGS = -O3 -Wall -std=c++11
CFLAGS = -g -Wall -std=c++11

bmp: main.o
	@echo "Compiling..."
	g++ -o $@ main.o $(CFLAGS)
	@echo "Done."

main.o: main.cpp BMPImg.h
	g++ -c main.cpp $(CFLAGS)

test: test.o
	@echo "Compiling test..."
	g++ -o $@ test.o $(CFLAGS)
	@echo "Done."

test.o: test.cpp BMPImg.h
	g++ -c test.cpp $(CFLAGS)

clean:
	rm -f bmp test *.o
	@echo "Done." 