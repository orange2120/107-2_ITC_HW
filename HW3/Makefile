CFLAGS = -O3 -Wall -std=c++11
CFLAGS = -g -Wall -std=c++11

cipher: src/cipher.o src/rsa_encrypt.o
	@echo "Compiling cipher..."
	g++ -o bin/$@ src/cipher.o src/rsa_encrypt.o $(CFLAGS)
	@echo "Done."

decipher: src/decipher.o src/rsa_encrypt.o
	@echo "Compiling decipher..."
	g++ -o bin/$@ src/decipher.o src/rsa_encrypt.o $(CFLAGS)
	@echo "Done."

find: src/find.o src/rsa_encrypt.o
	@echo "Compiling find..."
	g++ -o bin/$@ src/find.o src/rsa_encrypt.o $(CFLAGS)
	@echo "Done."

rsa_encrypt.o: src/rsa_encrypt.h src/rsa_encrypt.cpp
	g++ -c src/rsa_encrypt.h src/rsa_encrypt.cpp $(CFLAGS)

clean:
	rm -f ./bin/* ./src/*.o ./src/*.h.gch ./output/*
	@echo "Done."