#ifndef NODE
#define NODE

// #include "Message.h"
#include "cppsim.hh"
#include "../transport/Message.h"
#include <vector>

using namespace std;

class Transport;

class Node : public process {
    private:
        int id;
        int id_unique;
        int type;
        handle<Transport> *transport;
        int isp;
        double delay_acum;
        unsigned int num_messages_sended;
        unsigned int num_messages_received;
        static unsigned int id_uniques;
    protected:
        vector<Message *> message_stack;
        vector<Message *> unprocessed_message_stack;
    public:

        Node(const string &name, int id, int type) : process(name) {
            this->id                    = id;
            this->type                  = type;
            this->delay_acum            = 0;
            this->num_messages_received = 0;
            this->num_messages_sended   = 0;
            this->id_unique = Node::id_uniques;
            Node::id_uniques++;
        }
        ~Node() { }
        int GetId();
        unsigned int GetIdUnique();
        string GetIdString();
        int GetType();
        int GetIsp();
        void SetIsp(int);
        double GetIspDelay(int isp_from, int id_to, int type_to);
        void SetTransport(handle<Transport> *transport);
        int GetHoldTimeBeforeSendMessage();
        unsigned int GetNumMessagesSended(){
            return this->num_messages_sended;
        }
        unsigned int GetNumMessagesReceived(){
            return this->num_messages_received;
        }
        void SumDelayAcum(double delay);
        virtual void AddMessage(Message *message);
        virtual double SendMessage(Message *message);
        virtual void inner_body(void) = 0;
};

#endif
