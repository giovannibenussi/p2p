#ifndef PEER_PEER
#define PEER_PEER

#include "../node/Node.h"
#include "../lru/lru.hpp"
#include "../Constants.h"

class Transport;
class Dns;

class Peer : public Node
{
    private:
        Dns * dns;
        int number_of_messages_sended_to_dns;
        int number_of_querys_sended_this_cycle;
        unsigned int number_of_querys_sended;
        vector<MessageWSE *> message_wse_stack;
        lru_cache * cache;
        int cache_hits_cycle;
        int cache_miss_cycle;
    public:
        unsigned int querys_send;
        /**
          *
          * - Name: Nombre que utiliza libcppsim
          * - Id: identificador único del Peere (ojo: puede tener el mismo identificador
          *   que un edge server, pero el tipo es distinto)
          * - Type: Tipo de Node (e.g.: NODE_Peer, NODE_EDGE_SERVER, NODE_ORIGIN_SERVER, NODE_DNS, etc).
          *   Estos tipos están definidos en Constants.h
          *
          **/

        Peer(const string &name, int id, int type) : Node(name, id, type)
        {
            this->querys_send = 0;
            this->number_of_messages_sended_to_dns = 0;
            this->number_of_querys_sended = 0;
            this->number_of_querys_sended_this_cycle = 0;
            cache = new lru_cache( PEER_CACHE_SIZE );
            // lru_cache_cache * cache_cache = new lru_cache_cache(10);
            // (*cache)[ 0 ] = a;
            // (*cache)[ 1 ] = a;
            // (*cache_cache)[0] = 10;
            // cout << "PASO" << endl;
        }
        ~Peer()
        {

        }
        void AddCacheHit();
        void AddCacheMiss();
        int GetCacheHits();
        int GetCacheMiss();

        static inline int GetResponsiblePeer(string key);
        static inline int GetHash(string key);
        // bool IsInCache(string key);
        // MessageWSE GetFromCache(string key);
        void SetDns(Dns * dns)
        {
            this->dns = dns;
        }

        void AddMessageWse(MessageWSE * message_wse);

        void ResetCycle();
        int GetNumberOfQuerysSendedThisCycle();

        void inner_body(void);
        int GetEdgeServerId();
        int GetNumberOfMessagesSendedToDns();
        void HaveToSendAMessage();
        unsigned int GetNumberOfQueriesSended()
        {
            return this->number_of_querys_sended;
        }
};

#endif