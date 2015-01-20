#ifndef GEN_H
#define GEN_H

#include "../glob.h"
#include "../auxiliar/Util.h"
#include "../auxiliar/Hash.h"
#include "../wse/WSE.h"
#include "../wse/MessageWSE.h"

class Peer;

// class TlcProtocol;

class Gen_rnd: public process
{
        handle<Peer> * peers;
        rng<double> * random_Peer;

        ofstream * chart_file;
        ofstream * querys_sended_stream;
        //  list<handle<TlcProtocol> *> queue_thread;

        unsigned long int * totalQueries;
        int sentQueries, lastStepQueries;

        // vector<handle<TlcProtocol> *> Peers;
        Hash * h;
        int NP;
        UTIL * util;
        // Hash* h;
        char traces_file[2048];
        rng<double> * arrival_time;
        bool phase; //rafaga when true is a flash crowd, when false its normal time

        rng<double> * SelectSource;

        double prev, actual;
        ifstream endStream;
        vector<string> tokens;

        double CTE;

        handle<WSE> * wse;

        //ends simulation
        int * ends;
        int Nuser;
        int Totaluser; //Usuarios + peers
        int porcentaje_peers;

        int Peer_Selection;
        bool FLASH_CROWD;
        int NORMAL_RATE;
        int NORMAL_TIME;
        int CROWDED_RATE;
        int CROWD_TIME;

    protected:
        void inner_body( void );

    public:

        void freeGen_rnd()
        {
            // Peers.clear();
            delete util;
            // delete h;
            delete arrival_time;
            delete SelectSource;

            //free(wse);
        }

        Gen_rnd ( const string &name, char * _traces_file, unsigned long int * _totalQueries,
                  int _NP, int * _ends, int _Nuser,
                  handle<WSE> * _wse, int _PS, ofstream * chart_file, ofstream * querys_sended_stream, bool FLASH_CROWD, int NORMAL_RATE, int NORMAL_TIME, int CROWDED_RATE, int CROWD_TIME): process( name )
        {
            this->FLASH_CROWD  = FLASH_CROWD;
            this->NORMAL_RATE  = NORMAL_RATE;
            this->NORMAL_TIME  = NORMAL_TIME;
            this->CROWDED_RATE = CROWDED_RATE;
            this->CROWD_TIME = CROWD_TIME;

            this-> chart_file = chart_file;
            this->querys_sended_stream = querys_sended_stream;

            totalQueries    = _totalQueries;
            sentQueries     = 0;
            wse             = _wse;
            lastStepQueries = 0;
            Nuser           = _Nuser;
            //Peers=_Peers;
            NP              = _NP;
            Totaluser       = NP + Nuser;
            porcentaje_peers = (NP * 100) / Totaluser;

            util            = new UTIL();
            h               = new Hash();
            strcpy( traces_file, _traces_file );
            ends            = _ends;
            Peer_Selection = _PS;
            /*queries per unit of time*/
            // 1.0/3000.0
            phase = false;

            double media1 = (1.0 / NORMAL_RATE);
            arrival_time = new rngExp( "Arrive Time", media1 );

            arrival_time->reset();

            SelectSource = new rngUniform("SelectSource", 0, 100);
            SelectSource->reset();

            random_Peer = new rngUniform("RandomPeer", 0, NUM_PEERS);
            random_Peer->reset();

            prev = 0.0;
            actual   = 0.0;
        }
        void SetPeers(handle<Peer> * Peers)
        {
            this->peers = Peers;
        }

        // void add_peer(handle<TlcProtocol> * p)
        // {
        //    Peers.push_back(p);
        // }

        int getZipf(int, int);
        void setQueryRate(int);
        double rand_val(int);
};
#endif
