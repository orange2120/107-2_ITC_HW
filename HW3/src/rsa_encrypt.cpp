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

    pla.open(plainPath.c_str(), ifstream::in);
    pKey.open(pubKeyPath.c_str(), ifstream::in);

    if (!pla.is_open())
        return false;
    if (!pKey.is_open())
        return false;

    // get plain text content
    getline(pla, plainText, '\n');

    // IMPORTANT remove Line Feed(New Line) and Carriage Return symbol
    plainText.erase(std::remove(plainText.begin(), plainText.end(), '\n'), plainText.end());
    plainText.erase(std::remove(plainText.begin(), plainText.end(), '\r'), plainText.end());

    // get N,e
    pKey >> key_n;
    pKey >> key_e;

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
        cip = (uint64_t)plainText[i] * 256 + (uint64_t)plainText[i + 1];
        cip = myullPow(cip, key_e) % key_n;
        cipherText.push_back(cip);

        cout << setw(3) << (uint64_t)plainText[i] << " " << setw(3) << (uint64_t)plainText[i + 1] << " " << cip << endl;
    }

    // handle odd case
    if (plainText.size() % 2 != 0)
    {
        cip = (uint64_t)plainText.back() * 256;
        cip = myullPow(cip, key_e) % key_n;
        cipherText.push_back(cip);

        cout << setw(3)  << (uint64_t)plainText.back() << " "  << cip << endl;
    }

    plainText = "";
}

bool RSAEncrypt::writeEncrypt(const string &path)
{
    ofstream ofs;
    ofs.open(path.c_str(), ofstream::out);

    if(!ofs.is_open())
        return false;

    for (uint32_t i = 0; i < cipherText.size(); ++i)
        ofs << to_string(cipherText[i])  + '\n';

    ofs.close();
    return true;
}

bool RSAEncrypt::readDecrypt(const string &cipPath, const string &pKeyPath)
{
    ifstream cip;
    ifstream pKey;
    uint64_t cipherTmp = 0;
    string str;

    cip.open(cipPath.c_str(), ifstream::in);
    pKey.open(pKeyPath.c_str(), ifstream::in);

    // get N,d
    pKey >> key_n;
    pKey >> key_d;

    while (cip >> str)
    {
        cipherTmp = strtoull(str.c_str(), 0, 10);
        enCipherText.push_back(cipherTmp);
    }

    cip.close();
    pKey.close();

    cout << "N: " << key_n << " ,d: " << key_d << endl;

    return true;
}

void RSAEncrypt::decrypt()
{
    uint64_t plain = 0;
    uint32_t nCipherText = enCipherText.size();
    char text;

    cout << "cip.s:" << enCipherText.size() << endl;

    for (uint32_t i = 0; i < nCipherText; ++i)
    {
        plain = ExpBySq(enCipherText[i], key_d, key_n);

        cout << plain << " " << (plain >> 8) << " " << (plain & 0xFF) << endl;

        plainText += (char)(plain >> 8);

        // check if there exists valid second character (handling odd string length)
        if ((plain & 0x00FF) == 0)
            continue;
        
        plainText += (char)(plain & 0x00FF);
    }

}

bool RSAEncrypt::writeDecrypt(const string &path)
{
    ofstream ofs;
    ofs.open(path.c_str(), ofstream::out);

    if(!ofs.is_open())
        return false;

    ofs << plainText;

    ofs.close();

    cout << plainText << endl;

    plainText = "";
    return true;
}

bool RSAEncrypt::readFind(const string &path)
{
    ifstream ifs;
    ifs.open(path.c_str(), ifstream::in);

    if(!ifs.is_open())
        return false;

    ifs >> key_e;
    ifs >> phi;
    ifs.close();

    cout << "e: " << key_e << ", phi: " << phi << endl;
    return true;
}

bool RSAEncrypt::solve_d()
{
    uint64_t e = key_e % phi;
    for (uint64_t i = 1; i < phi; ++i)
    {
        if ((e * i) % phi == 1)
        {
            key_d = i;
            return true;
        }
    }
    
    /*uint64_t d;
    uint64_t i, j;
    extendedGCD(key_e, phi, i, j, d);
    if (d == 1)
        key_d = i;
    */
    cout << "d: " << key_d << endl;
    return false;
}

bool RSAEncrypt::writeFind(const string &path)
{
    ofstream ofs;
    ofs.open(path.c_str(), ofstream::out | ofstream::trunc);

    if(!ofs.is_open())
        return false;

    ofs << to_string(key_d) + '\n';

    ofs.close();
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

// Modular Exponentiation (Power in Modular Arithmetic)
uint64_t RSAEncrypt::ExpBySq(uint64_t base, uint64_t exponent, uint64_t mod)
{
    uint64_t r = 1;
    while(exponent > 0)
    {
        if (exponent & 1)
            r = (r * base) % mod;
        exponent >>= 1;
        base = (base * base) % mod;
    }
    return r;
}

void RSAEncrypt::extendedGCD(uint64_t a, uint64_t b, uint64_t& i, uint64_t& j, uint64_t& d)
{
    int64_t i_ = 1, j_ = 0;
    int64_t c = a;
    
    i = 0, j = 1;
    d = b;
    
    while (true)
    {
        uint64_t q = c / d, r = c % d, t;
        if (r == 0) break;
        c = d; d = r;
        t = i_; i_ = i; i = t - q * i;
        t = j_; j_ = j; j = t - q * j;
    }
}