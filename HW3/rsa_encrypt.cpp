#include "rsa_encrypt.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
using namespace std;

bool RSAEncrypt::readEncrypt(const string &plainPath, const string &pubKeyPath)
{
    ifstream pla;
    ifstream pKey;
    string n_str;
    string e_str;

    pla.open(plainPath.c_str(), ios::in);
    pKey.open(pubKeyPath.c_str(), ios::in);

    if (!pla.is_open())
        return false;
    if (!pKey.is_open())
        return false;

    // get plain text content
    getline(pla, plainText, '\n');

    // IMPORTANT remove Line Feed(New Line) and Carriage Return symbol
    plainText.erase(std::remove(plainText.begin(), plainText.end(), '\n'), plainText.end());
    plainText.erase(std::remove(plainText.begin(), plainText.end(), '\r'), plainText.end());

    // get public key N and e
    getline(pKey, n_str, '\n');
    getline(pKey, e_str, '\n');

    // convert string to lld
    key_n = strtoull(n_str.c_str(), 0, 10);
    key_e = strtoull(e_str.c_str(), 0, 10);

    cout << "N: " << key_n << " e: " << key_e << endl;

    pla.close();
    pKey.close();

    return true;
}

void RSAEncrypt::encrypt()
{
    uint64_t cip = 0;
    uint32_t nPlainText = plainText.size();

    cout << "Plain text:" << endl;
    cout << plainText << '\n' << endl;
    //cout << "n^e: " << pow(key_n, key_e) << endl;

    // odd
    if (nPlainText % 2)
        nPlainText--;

    for (uint32_t i = 0; i < nPlainText - 1; i += 2)
    {
        cip = (uint64_t)plainText[i] * myullPow(key_n, key_e) + (uint64_t)plainText[i + 1];
        cipherText.push_back(cip);
        
        cout << setw(3) << (uint64_t)plainText[i] << " " << setw(3) << (uint64_t)plainText[i + 1] << " " << cip << endl;
    }

    // handle odd case
    if (plainText.size() % 2 != 0)
    {
        cip = (uint64_t)plainText.back() * myullPow(key_n, key_e);
        cipherText.push_back(cip);

        cout << setw(3)  << (uint64_t)plainText.back() << " "  << cip << endl;
    }

    plainText = "";
}

bool RSAEncrypt::writeEncrypt(const string &path)
{
    ofstream ofs;
    ofs.open(path.c_str(), ios::out | ios::trunc);

    if(!ofs.is_open())
        return false;

    for (uint32_t i = 0; i < cipherText.size(); ++i)
        ofs << to_string(cipherText[i])  + '\n';

    ofs.close();
    return true;
}

bool RSAEncrypt::readDecrypt(const string &cipPath, const string &priKeyPath)
{
    ifstream cip;
    ifstream pKey;
    uint64_t cipherTmp = 0;
    string n_str = "";
    string d_str = "";
    string str;

    cip.open(cipPath.c_str(), ios::in);
    pKey.open(priKeyPath.c_str(), ios::in);

    // get N,
    pKey >> key_n;
    pKey >> key_d;

    //key_n = strtoull(n_str.c_str(), 0, 10);

    while (cip >> str)
    {
        cipherTmp = strtoull(str.c_str(), 0, 10);
        enCipherText.push_back(cipherTmp);
    }

    cip.close();
    pKey.close();

    return true;
}

void RSAEncrypt::decrypt()
{
    uint64_t plain = 0;
    uint32_t nCipherText = enCipherText.size();

    if(nCipherText % 2)
        nCipherText--;

    for (uint32_t i = 0; i < nCipherText; ++i)
    {
        plain = myullPow(enCipherText[i], key_d);

    }
}

bool RSAEncrypt::writeDecrypt(const string &path)
{
    ofstream ofs;
    ofs.open(path.c_str(), ios::out | ios::trunc);

    if(!ofs.is_open())
        return false;

    ofs << plainText + '\n';
    
    return true;
}


// Private member functions
uint64_t RSAEncrypt::myullPow(uint64_t base, uint64_t exp)
{
    uint64_t result = 1ULL;
    while( exp )
    {
        if ( exp & 1 )
        {
            result *= (uint64_t)base;
        }
        exp >>= 1;
        base *= base;
    }
    return result;
}