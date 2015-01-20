#include "../Constants.h"
#include "Transport.h"

double Transport::isps[3][3];

void Transport::AddMessage(Message * message)
{
    this->message_stack.push_back(message);
}

void Transport::SetIsps(double ptr_isps[][3])
{
    // isps = ptr_isps;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            isps[ i ][ j ] = ptr_isps[ i ][ j ];
        }
    }
}

void Transport::AddNode(handle< Node > * node)
{
    this->nodes.push_back(node);
}

Node * Transport::GetServer(int id_unique)
{
    Node * node;
    for (int i = 0, max = nodes.size(); i < max; i++)
    {
        node = (Node *)(*nodes.at(i));
        if (node->GetIdUnique() == id_unique)
        {
            return node;
        }
    }
    return NULL;
}

Node * Transport::GetServer(int id, int type)
{
    Node * node;
    for (int i = 0, max = nodes.size(); i < max; i++)
    {
        node = (Node *)(*nodes.at(i));
        if (node->GetType() == type && node->GetId() == id)
        {
            return node;
        }
    }
    return NULL;
}

void Transport::inner_body(void)
{
    while (1)
    {
        // ===== BUSY TIME START =====
        // time_aux = time();
        // Por cada mensaje en el stack
        while (!message_stack.empty())
        {
            Message * message = message_stack.back();
            message_stack.pop_back();
            // Variable que guardará el servidor receptor (cliente, edge server o wse)
            Node * servidor = this->GetServer(message->GetIdTo(), message->GetTypeTo());

            servidor->AddMessage(message);
            // hold(0.001);
            if (servidor->idle())
            {
                servidor->activate();
            }
        }
        // busy_time += time() - time_aux;
        // ===== BUSY TIME END =====

        // time_aux = time();
        this->passivate();
        // idle_time += time() - time_aux;
    }
}