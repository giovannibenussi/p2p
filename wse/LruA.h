/* Implementa cache con politica LRU. */
#ifndef LRUA_h
#define LRUA_h

#include "../glob.h"
#include "Answer.h"
// #include "../auxiliar/Hash.h"
#include <queue>
#include <list>
#include <iostream>

using namespace std;


struct CompareAnswers: public std::binary_function<Answer *, Answer *, bool>
{
    bool operator() (Answer *e1, Answer *e2 ){
        return e1->priority > e2->priority;
    }
};

/* Class that implements a cache LRU */

class LRUA
{
    private:
        // Cache is form with entries
        int *cacheSize;
        int freeCache;

        int timestamps;

        // Hash *h;

        //--temporales
        Answer *ptr;
        unsigned int prio;
        //vector<string> v;
        //vector<string>::iterator itv;

        priority_queue <Answer *, vector<Answer *>, CompareAnswers> pq;
        vector<Answer *> cache;
        vector<Answer *>::iterator cache_entry_pointer;
    public:

        LRUA( int *_cacheSize ){
            cacheSize   = _cacheSize;
            timestamps   = 0;
            freeCache = *_cacheSize;
            // h = new Hash();
        }

        ~LRUA( ){

            //pq.clear();
            vector<Answer *>::iterator it;

            it = cache.begin();

            while (cache.size() > 0)
            {
                it = cache.begin();
                if (it != cache.end())
                {
                    Answer *temp = (*it);
                    cache.erase(it);
                    delete temp;
                }
                it++;
                //    cout << "DEBUG CS : " << cache.size() << endl;
                // // cache.clear( );
            }

        }
        void insertAnswer(Answer *);
        bool isFreeSpace( int/*size*/ );
        void createSpace( int/*size*/ );
        void remove(Answer *);
        Answer *check(string);
        void cacheErase(Answer *);
        void getSize(){
            cout << "LruA:cache.size()=" << cache.size( ) << endl;
        }
};
#endif
