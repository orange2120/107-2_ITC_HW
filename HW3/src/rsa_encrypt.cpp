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
        _plainText.push_back(plain);

    // get N,e
    pKey >> _key_n;
    pKey >> _key_e;

    cout << "N: " << _key_n << " e: " << _key_e << endl;

    pla.close();
    pKey.close();

    return true;
}

void RSAEncrypt::encrypt()
{
    uint64_t cip = 0;
    uint32_t nPlainText;

    //cout << "n^e: " << pow(_key_n, _key_e) << endl;

    for (uint32_t i = 0; i < _plainText.size(); ++i)
    {
        cout << "Plain text[" << _plainText[i].size() << "]:" << endl;
        cout << _plainText[i] << '\n' << endl;

        nPlainText = _plainText[i].size();
        // odd
        if (nPlainText % 2)
            nPlainText--;

        for (uint32_t j = 0; j < nPlainText - 1; j += 2)
        {
            //cout << "(i,j)=" << "(" << i << "," << j << ")" << endl;
            cip = (uint64_t)_plainText[i][j] * 256 + (uint64_t)_plainText[i][j + 1];
            cip = myullPow(cip, _key_e) % _key_n;
            _cipherText.push_back(cip);

            cout << setw(3) << (uint64_t)_plainText[i][j] << " " << setw(3) << (uint64_t)_plainText[i][j + 1] << " " << cip << endl;
        }

        // handle odd case
        if (_plainText[i].size() % 2 != 0)
        {
            cip = (uint64_t)_plainText.back().back() * 256;
            cip = myullPow(cip, _key_e) % _key_n;
            _cipherText.push_back(cip);

            cout << setw(3)  << (uint64_t)_plainText.back().back() << " "  << cip << endl;
        }
    }
    _plainText.clear();
}

bool RSAEncrypt::writeEncrypt(const string &path)
{
    ofstream ofs;
    ofs.open(path.c_str(), ofstream::out);

    if(!ofs.is_open())
        return false;

    for (uint32_t i = 0; i < _cipherText.size(); ++i)
        ofs << to_string(_cipherText[i]) + '\n';

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
    pKey >> _key_n;
    pKey >> _key_d;

    while (cip >> str)
    {
        cipherTmp = strtoull(str.c_str(), 0, 10);
        _enCipherText.push_back(cipherTmp);
    }

    cip.close();
    pKey.close();

    cout << "N: " << _key_n << " ,d: " << _key_d << endl;

    return true;
}

void RSAEncrypt::decrypt()
{
    uint64_t plain = 0;
    uint32_t nCipherText = _enCipherText.size();
    char text;

    cout << "cip.s:" << _enCipherText.size() << endl;

    for (uint32_t i = 0; i < nCipherText; ++i)
    {
        plain = ExpBySq(_enCipherText[i], _key_d, _key_n);

        cout << plain << " " << (plain >> 8) << " " << (plain & 0xFF) << endl;

        _decPlainText += (char)(plain >> 8);

        // check if there exists valid second character (handling odd string length)
        if ((plain & 0x00FF) == 0)
            continue;
        
        _decPlainText += (char)(plain & 0x00FF);
    }

}

bool RSAEncrypt::writeDecrypt(const string &path)
{
    ofstream ofs;
    ofs.open(path.c_str(), ofstream::out);

    if(!ofs.is_open())
        return false;

    ofs << _decPlainText;

    ofs.close();

    cout << _decPlainText << endl;

    _decPlainText = "";
    return true;
}

bool RSAEncrypt::readFind(const string &path)
{
    ifstream ifs;
    ifs.open(path.c_str(), ifstream::in);

    if(!ifs.is_open())
        return false;

    ifs >> _key_e;
    ifs >> _phi;
    ifs.close();

    cout << "e: " << _key_e << ", phi: " << _phi << endl;
    return true;
}

void RSAEncrypt::solve_d()
{
    /*
    uint64_t e = _key_e % _phi;
    for (uint64_t i = 1; i < _phi; ++i)
    {
        if ((e * i) % _phi == 1)
        {
            _key_d = i;
            return true;
        }
    }
    */
    _key_d = modInverse(_key_e, _phi);

    cout << "d: " << _key_d << endl;

}

bool RSAEncrypt::writeFind(const string &path)
{
    ofstream ofs;
    ofs.open(path.c_str(), ofstream::out | ofstream::trunc);

    if(!ofs.is_open())
        return false;

    ofs << to_string(_key_d);

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