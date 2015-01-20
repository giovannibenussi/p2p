#ifndef WSE_H
#define WSE_H

#include "../glob.h"
#include "../node/Node.h"
#include "MessageWSE.h"
#include "LruA.h"
#include "../Constants.h"

//#define WSECACHESIZE 8000

class WSE: public Node
{

    private:
        // ID_PAGE , (TTL, VERSION)
        //   map<int*, pair<long int,int> > answers;

        list<MessageWSE *> queue_in;

        bool busy;
        LRUA * ANSWERS;
        unsigned int cache_hits;
        // ofstream cache_hits_stream;
        // cache_hits_stream.open("charts/wse_cache_hits");
    public:
        WSE ( const string &name, int WSECACHESIZE ): Node ( name, 0, NODE_ORIGIN_SERVER )
        {
            busy = false;
            int cacheSize = WSECACHESIZE;
            ANSWERS = new LRUA(&cacheSize);
        }

        ~WSE()
        {
            //  answers.clear();
            delete ANSWERS;
        }


        void inner_body ( void );

        void add_request ( MessageWSE * bn )
        {
            queue_in.push_back(bn);
        }

        bool get_busy()
        {
            return busy;
        }

        void print()
        {
            cout << "WSE" << endl;
        }

        long int getTTL(int *);
        //int getVersion(int*);
        int getVersion(string, BIGNUM *);
        long int randomTTL();
};


#endif