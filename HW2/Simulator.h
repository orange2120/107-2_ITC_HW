#ifndef __SIMULATOR__
#define __SIMULATOR__

#include <string>
#include <vector>
using namespace std;

class Simulator{
    public:
        Simulator(){
            programCounter.resize(8, 0);
            
            regis.resize(5);
            for(int i=0; i<regis.size(); i++){
                regis[i].resize(8, 0);
            }


        }

        bool loadMemory(string);
        bool storeMemory(string);
        bool simulate();

    private:
        vector<bool> programCounter;
        vector< vector<bool> > regis;
        vector< vector<bool> > memory;

};

#endif