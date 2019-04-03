#include <iostream>
#include "rsa_encrypt.h"
using namespace std;

int main()
{
    RSAEncrypt rsa;

    if(!rsa.readDecrypt("./secret.txt", "./private_key.txt"))
    {
        cerr << "[ERROR] Open file failed!" << endl;
        return 0;
    }
    rsa.decrypt();
    if(!rsa.writeDecrypt("./output/message.txt"))
    {
        cerr << "[ERROR] Write file failed!" << endl;
    }
    
    return 0;