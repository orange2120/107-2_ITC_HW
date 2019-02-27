CFLAGS = -O3 -Wall -std=c++11
CFLAGS = -g -Wall -std=c++11

bmp: main.o
	@echo "Compiling..."
	g++ -o $@ main.o $(CFLAGS)
	@echo "Done."

main.o: main.cpp BMPImg.h
	g++ -c main.cpp $(CFLAGS)

clean:
	rm -f bmp *.o
	@echo "Done." 