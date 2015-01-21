#include "Peer.h"
#include "../Constants.h"
#include "../transport/Dns.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

void Peer::inner_body(void)
{
    int edge_server_to;
    double delay;
    cout << fixed << setprecision(4);
    while (1)
    {
        while (!message_stack.empty())
        {
            // cout << "------------ NUEVO MENSAJE EN PEER" << endl;
            Message * message = message_stack.back();
            // cout << time() << " - Peere " << setw(3) << this->GetId() << ": recibi respuesta a mi consulta " << message->GetMessage() << " enviada en tiempo " << message->GetCreationTime() << " hacia el servidor " << message->GetIdFrom() << endl;
            // cout << "Delay: " << time() - message->GetCreationTime() << endl;
            SumDelayAcum( time() - message->GetCreationTime() );
            message_stack.pop_back();
            // string * respuesta = new string("respuestaaaaa !!!!!!!");
            // this->SendMessage(new Message(this->GetId(), this->GetType(), message->GetIdFrom(), message->GetTypeFrom(), time(), respuesta));
            string key_string = message->GetMessagePointer()->getKey();

            int responsible_peeer = 0;
            for (int i = 0; i < key_string.length(); ++i)
            {
                responsible_peeer += key_string[ i ];
            }
            responsible_peeer = responsible_peeer % NUM_PEERS;
            int key = responsible_peeer;

            int hash = 0;
            for (int i = 0; i < key_string.length(); ++i)
            {
                hash += key_string[ i ] * 3 + 1;
            }
            hash = (hash) % NUM_PEERS;
            // cout << "Key: " << key << endl << "Hash: " << hash << endl << endl;

            // key = 1;
            // strcpy(key_string, message->GetMessage().getKey());
            // cout << "Key: " << key << endl;
            // int key = Peer::GetResponsiblePeer( key_string );
            if (message->GetTypeFrom() == NODE_PEER || message->GetTypeFrom() == NODE_ORIGIN_SERVER)
            {
                int id_message = -1;
                for (int id = 0; id < unprocessed_message_stack.size(); ++id)
                {
                    if (unprocessed_message_stack.at(id)->GetMessagePointer()->getQuery() == message->GetMessagePointer()->getQuery())
                    {
                        id_message = id;
                        break;
                    }
                }

                if (id_message == -1) // No lo envie antes
                {

                    if (message->GetTypeFrom() == NODE_PEER) // Un peer me contacto porque soy responsible
                    {

                        // Busco en mi cache
                        lru_cache::const_iterator it = cache->find( hash );
                        if (it != cache->end())
                        {
                            // cout << "CACHE HIT: " << key << endl;
                            this->AddCacheHit();
                        }else{
                            // cout << "CACHE MISS: " << key << endl;
                            this->AddCacheMiss();
                        }

                        // cout << "Recibi desde un peer" << endl;
                        Message * message_a_enviar = new Message(this->GetId(), NODE_PEER, 0, NODE_ORIGIN_SERVER, time(), message->GetMessagePointer());
                        message_a_enviar->SetOriginal(message->GetIdOriginal(), message->GetTypeOriginal());
                        // cout << "............. PEER -> WSE" << endl;
                        // cout << message_a_enviar->GetIdOriginal() << endl;
                        delay = this->SendMessage(message_a_enviar);
                        unprocessed_message_stack.push_back(message_a_enviar);
                    }

                }
                else   // Respuesta a un mensaje mio
                {
                    // Obtengo el mensaje original:
                    Message * original = unprocessed_message_stack.at(id_message);

                    if (message->GetTypeFrom() == NODE_PEER)
                    {
                        // cout << "Recibi desde un peer al que contacte antes" << endl;
                    }
                    if (message->GetTypeFrom() == NODE_ORIGIN_SERVER)
                    {
                        int mi_id = this->GetId();
                        int id_origen = original->GetIdOriginal();

                        // lru_cache::const_iterator it = cache->find( key );
                        // Lo Agrego al cache
                        // cout << "Key: " << key << endl;
                        // (*cache)[ key ] = message->GetMessage();
                        (*cache)[ hash ] = 10;
                        // if (it != cache->end())
                        // {
                        // cout << "AGREGUE A CACHE" << endl;
                        // }

                        // cout << "Recibi desde WSE" << endl;
                        // cout << "Mi id: " << mi_id << endl;
                        // cout << "Desde id: " << id_origen << endl;
                        if (mi_id != id_origen && id_origen != -1)
                        {
                            // cout << "Yo NOOO era el responsible" << endl;
                            Message * message_a_enviar = new Message(this->GetId(), NODE_PEER, original->GetIdOriginal(), NODE_PEER, time(), message->GetMessagePointer());
                            message_a_enviar->SetOriginal(message->GetIdOriginal(), message->GetTypeOriginal());
                            delay = this->SendMessage(message_a_enviar);
                        }
                        else   // Yo era el responsible
                        {
                            // cout << "Yo era el responsible" << endl;
                        }
                    }

                    unprocessed_message_stack.erase(unprocessed_message_stack.begin() + id_message);
                }


            }
        }

        // cout << this->GetId() << "Dormire por: " << sleep << endl  ;
        if ( ! message_wse_stack.empty() )
        {
            edge_server_to = this->GetEdgeServerId();
            // cout << "Peere " << this->GetId() << " hacia " << edge_server_to << endl;
            this->querys_send++;
            number_of_querys_sended_this_cycle++;
            MessageWSE * message_wse = message_wse_stack.back();
            message_wse_stack.pop_back();

            string key = message_wse->getKey();
            // cout << "Query: " << key << endl;
            // const char * key = message->GetMessage().getKey().c_str();
            int responsible_peeer = GetResponsiblePeer( key );
            // cout << "Ira a: " << responsible_peeer << endl;

            this->number_of_querys_sended++;
            // Parametros de esta llamada:
            // id del creador, tipo creador, id receptor, tipo receptor,
            // tiempo creacion mensaje, mensaje (de tipo MessageWSE *)

            // OPCION 1:
            // responsible_peeer = this->GetId();
            if (responsible_peeer == this->GetId())
            {
                Message * message = new Message(this->GetId(), NODE_PEER, 0, NODE_ORIGIN_SERVER, time(), message_wse);
                // cout << "YO ERA EL RESPONSIBLE !!!!" << endl;
                // cout << "1............. PEER -> WSE" << endl;
                delay = this->SendMessage(message);
                unprocessed_message_stack.push_back(message);
            }
            else
            {
                Message * message = new Message(this->GetId(), NODE_PEER, responsible_peeer, NODE_PEER, time(), message_wse);
                // cout << "2............. PEER -> PEER" << endl;
                message->SetOriginal(this->GetId(), this->GetType());
                // cout << message->GetIdOriginal() << endl;
                delay = this->SendMessage(message);
                unprocessed_message_stack.push_back(message);
            }
        }
        this->passivate();
    }
}

// bool Peer::IsInCache(string key)
// {
// lru_cache::const_iterator it = cache->find(key);
// return it != cache->end();
// }

// MessageWSE Peer::GetFromCache(string key)
// {
// lru_cache::const_iterator it = cache->find(key);

// if (it != cache->end())
// {
//     return it.value();
// }
//     return NULL;
// }

int Peer::GetResponsiblePeer(string key)
{
    int responsible_peeer = 0;
    for (int i = 0; i < key.length(); ++i)
    {
        responsible_peeer += key[ i ];
    }
    responsible_peeer = responsible_peeer % PEER_CACHE_SIZE;
    return responsible_peeer;
}

int Peer::GetHash(string key)
{
    int responsible_peeer = 0;
    for (int i = 0; i < key.length(); ++i)
    {
        responsible_peeer += key[ i ] * 3 + 1;
    }
    responsible_peeer = (responsible_peeer * 123) % PEER_CACHE_SIZE;
    return responsible_peeer;
}

void Peer::HaveToSendAMessage()
{
    if (this->idle())
        this->activate();
}

int Peer::GetEdgeServerId()
{
    this->number_of_messages_sended_to_dns++;
    return this->dns->GetEdgeServerId(this->GetId(), this->GetIsp());
}

int Peer::GetNumberOfMessagesSendedToDns()
{
    return this->number_of_messages_sended_to_dns;
}

void Peer::ResetCycle()
{
    // cout << "RESETEANDO CICLO " << endl;
    this->number_of_querys_sended_this_cycle = 0;
    this->cache_hits_cycle = 0;
    this->cache_miss_cycle = 0;
}

void Peer::AddCacheHit(){
    this->cache_hits_cycle++;
}

void Peer::AddCacheMiss(){
    this->cache_miss_cycle++;
}

int Peer::GetCacheHits(){
    return this->cache_hits_cycle;
}

int Peer::GetCacheMiss(){
    return this->cache_miss_cycle;
}

int Peer::GetNumberOfQuerysSendedThisCycle()
{
    return number_of_querys_sended_this_cycle;
}

void Peer::AddMessageWse(MessageWSE * message_wse)
{
    this->message_wse_stack.push_back(message_wse);
}