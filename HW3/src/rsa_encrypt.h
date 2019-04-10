#ifndef _RSA_ENCRYPT_H_
#define _RSA_ENCRYPT_H_

#include <string>
#include <vector>
using namespace std;

class RSAEncrypt
{
    public:
      RSAEncrypt(){};
      ~RSAEncrypt(){};
      bool readEncrypt(const string &, const string &);
      void encrypt();
      bool writeEncrypt(const string &);
      bool readDecrypt(const string &, const string &);
      void decrypt();
      bool writeDecrypt(const string &);
      bool readFind(const string &);
      void solve_d();
      bool writeFind(const string &);
      
    private:
      uint64_t myullPow(uint64_t, uint64_t);
      uint64_t ExpBySq(uint64_t, uint64_t, uint64_t);
      int64_t modInverse(uint64_t, uint64_t);

      string plainText = "";
      uint64_t key_n = 0;
      uint64_t key_e = 0;
      uint64_t key_d = 0;
      uint64_t phi = 0;
      vector<uint64_t> cipherText;
      vector<uint64_t> enCipherText;
};

#endif