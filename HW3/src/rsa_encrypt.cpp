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
    string plain = "";

    pla.open(plainPath.c_str(), ifstream::in);
    pKey.open(pubKeyPath.c_str(), ifstream::in);

    if (!pla.is_open())
        return false;
    if (!pKey.is_open())
        return false;

    while (getline(pla, plain, '\n'))
        plainText.push_back(plain);

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
    uint32_t nPlainText;

    //cout << "n^e: " << pow(key_n, key_e) << endl;

    for (uint32_t i = 0; i < plainText.size(); ++i)
    {
        cout << "Plain text[" << plainText[i].size() << "]:" << endl;
        cout << plainText[i] << '\n' << endl;

        nPlainText = plainText[i].size();
        // odd
        if (nPlainText % 2)
            nPlainText--;

        for (uint32_t j = 0; j < nPlainText - 1; j += 2)
        {
            //cout << "(i,j)=" << "(" << i << "," << j << ")" << endl;
            cip = (uint64_t)plainText[i][j] * 256 + (uint64_t)plainText[i][j + 1];
            cip = myullPow(cip, key_e) % key_n;
            cipherText.push_back(cip);

            cout << setw(3) << (uint64_t)plainText[i][j] << " " << setw(3) << (uint64_t)plainText[i][j + 1] << " " << cip << endl;
        }

        // handle odd case
        if (plainText[i].size() % 2 != 0)
        {
            cip = (uint64_t)plainText.back().back() * 256;
            cip = myullPow(cip, key_e) % key_n;
            cipherText.push_back(cip);

            cout << setw(3)  << (uint64_t)plainText.back().back() << " "  << cip << endl;
        }
    }
    plainText.clear();
}

bool RSAEncrypt::writeEncrypt(const string &path)
{
    ofstream ofs;
    ofs.open(path.c_str(), ofstream::out);

    if(!ofs.is_open())
        return false;

    for (uint32_t i = 0; i < cipherText.size(); ++i)
        ofs << to_string(cipherText[i]) + '\n';

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

        decPlainText += (char)(plain >> 8);

        // check if there exists valid second character (handling odd string length)
        if ((plain & 0x00FF) == 0)
            continue;
        
        decPlainText += (char)(plain & 0x00FF);
    }

}

bool RSAEncrypt::writeDecrypt(const string &path)
{
    ofstream ofs;
    ofs.open(path.c_str(), ofstream::out);

    if(!ofs.is_open())
        return false;

    ofs << decPlainText;

    ofs.close();

    cout << decPlainText << endl;

    decPlainText = "";
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

void RSAEncrypt::solve_d()
{
    /*
    uint64_t e = key_e % phi;
    for (uint64_t i = 1; i < phi; ++i)
    {
        if ((e * i) % phi == 1)
        {
            key_d = i;
            return true;
        }
    }
    */
    key_d = modInverse(key_e, phi);

    cout << "d: " << key_d << endl;

}

bool RSAEncrypt::writeFind(const string &path)
{
    ofstream ofs;
    ofs.open(path.c_str(), ofstream::out | ofstream::trunc);

    if(!ofs.is_open())
        return false;

    ofs << to_string(key_d);

    ofs.close();
    return true;
}

// Private member functions
uint64_t RSAEncrypt::myullPow(uint64_t base, uint64_t exponent)
{
    uint64_t result = 1ULL;
    while (exponent)
    {
        if ( exponent & 1 )
        {
            result *= (uint64_t)base;
        }
        exponent >>= 1;
        base *= base;
    }
    return result;
}

// Modular Exponentiation (Power in Modular Arithmetic)
uint64_t RSAEncrypt::ExpBySq(uint64_t base, uint64_t exponent, const uint64_t &mod)
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

// modular inverse https://rosettacode.org/wiki/Modular_inverse
int64_t RSAEncrypt::modInverse(uint64_t a, uint64_t b)
{
	uint64_t b0 = b, t, q;
	int64_t x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}