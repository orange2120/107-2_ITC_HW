CFLAGS = -O3 -Wall -std=c++11
CFLAGS = -g -Wall -std=c++11

cipher: cipher.o rsa_encrypt.o
	@echo "Compiling cipher..."
	g++ -o $@ cipher.o rsa_encrypt.o $(CFLAGS)
	@echo "Done."

decipher: decipher.o rsa_encrypt.o
	@echo "Compiling decipher..."
	g++ -o $@ decipher.o rsa_crypt.o $(CFLAGS)
	@echo "Done."

find: find.o rsa_encrypt.o
	@echo "Compiling find..."
	g++ -o $@ find.o rsa_crypt.o $(CFLAGS)
	@echo "Done."

rsa_encrypt.o: rsa_encrypt.h rsa_encrypt.cpp
	g++ -c rsa_encrypt.h rsa_encrypt.cpp $(CFLAGS)

clean:
	rm -f cipher decipher find *.o *.h.gch ./output/*
	@echo "Done."