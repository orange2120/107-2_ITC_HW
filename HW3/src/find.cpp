#include <iostream>
#include "rsa_encrypt.h"
using namespace std;

int main()
{
    RSAEncrypt rsa;

    if(!rsa.readFind("./cryptan.txt"))
    {
        cerr << "[ERROR] Open file failed!" << endl;
        return 0;
    }
    rsa.solve_d();
    if(!rsa.writeFind("./output/cryptan_result.txt"))
    {
        cerr << "[ERROR] Write file failed!" << endl;
    }
    
    return 0;
}