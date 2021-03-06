#ifndef CLIENT
#define CLIENT

#include "../node/Node.h"

class EdgeServer;
class Transport;
class Dns;

class Client : public Node {
    private:
        Dns *dns;
        int number_of_messages_sended_to_dns;
        int number_of_querys_sended_this_cycle;
        unsigned int number_of_querys_sended;
        vector<MessageWSE*> message_wse_stack;
    public:
        unsigned int querys_send;
        /**
          *
          * - Name: Nombre que utiliza libcppsim
          * - Id: identificador único del cliente (ojo: puede tener el mismo identificador
          *   que un edge server, pero el tipo es distinto)
          * - Type: Tipo de Node (e.g.: NODE_CLIENT, NODE_EDGE_SERVER, NODE_ORIGIN_SERVER, NODE_DNS, etc).
          *   Estos tipos están definidos en Constants.h
          *
          **/
        Client(const string &name, int id, int type) : Node(name, id, type) {
            this->querys_send = 0;
            this->number_of_messages_sended_to_dns = 0;
            this->number_of_querys_sended = 0;
            this->number_of_querys_sended_this_cycle = 0;
        }
        ~Client() {

        }
        void SetDns(Dns *dns) {
            this->dns = dns;
        }

        void AddMessageWse(MessageWSE *message_wse);

        void ResetCycle();
        int GetNumberOfQuerysSendedThisCycle();

        void inner_body(void);
        int GetEdgeServerId();
        int GetNumberOfMessagesSendedToDns();
        void HaveToSendAMessage();
        unsigned int GetNumberOfQueriesSended(){
            return this->number_of_querys_sended;
        }
};

#endif