#include <iostream>
#include "rsa_encrypt.h"
using namespace std;

int main()
{
    RSAEncrypt rsa;

    if(!rsa.readEncrypt("./plain.txt", "./public_key.txt"))
    {
        cerr << "[ERROR] Open file failed!" << endl;
        return 0;
    }
    rsa.encrypt();
    if(!rsa.writeEncrypt("./output/secret.txt"))
    {
        cerr << "[ERROR] Write file failed!" << endl;
    }
    
    return 0;
}