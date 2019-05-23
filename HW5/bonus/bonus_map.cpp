#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#define FILE_PATH "./bonus/bonus.txt"

typedef map<string ,uint32_t> WordsMap;

class FreqCounter
{
  public:
    FreqCounter(){}
    ~FreqCounter(){}
    bool readFile()
    {
        ifstream ifs;
        string str = "";


        ifs.open(FILE_PATH , ifstream::in);
        if (!ifs.is_open())
            return false;

        ifs >> k;
        cout << "K = " << k << endl;

        while (ifs >> str)
            ++_mp[str];

        cout << "MP.S = " << _mp.size() << endl;
        ifs.close();
        return true;
    }

    void countFrequency()
    {
        vector<pair<string, uint32_t>> v(_mp.begin(), _mp.end());
        sort(v.begin(), v.end(), compByVal);

        for (uint32_t i = 0; i < k; ++i)
            cout << v[i].first << " " << v[i].second << endl;
        cout << endl;
    }

    void printMap() const
    {
        for (WordsMap::const_iterator it = _mp.begin(); it != _mp.end(); ++it)
            cout << "[" << it->first << "] = " << it->second << endl;
    }

  private:

    static bool compByVal(pair<string, uint32_t> &lhs, pair<string, uint32_t> &rhs)
    {
        return lhs.second > rhs.second;
    }

    uint64_t k = 0;
    map<string, uint32_t> _mp;
};

int main()
{
    FreqCounter fc;
    if (!fc.readFile())
    {
        cerr << "[ERROR] Open input file failed!" << endl;
        return 0;
    }
    fc.countFrequency();
    //fc.printMap();

    return 0;
}