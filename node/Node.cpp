#include "Node.h"
#include "../transport/Transport.h"

unsigned int Node::id_uniques = 0;

int Node::GetId()
{
    return this->id;
}

unsigned int Node::GetIdUnique()
{
    return this->id_unique;
}

string Node::GetIdString()
{
    return "Hola";
    // return IntToString(this->id);
}
int Node::GetType()
{
    return type;
}

void Node::SetTransport(handle<Transport> * transport)
{
    this->transport = transport;
}

double Node::GetIspDelay(int isp_from, int id_to, int type_to)
{
    return (*transport)->GetIspDelay(isp_from, id_to, type_to);
}

double Node::SendMessage(Message * message)
{
    // Suma uno al contador de mensajes enviados
    this->num_messages_sended++;
    // Calcula el delay basado en el isp tanto del emisor como del receptor
    // Necesita el tipo del receptor para saber en que arreglo de servidores buscar (clients, edge_servers, wse)
    double delay = this->GetIspDelay(this->GetIsp(), message->GetIdTo(), message->GetTypeTo());
    // Añade el mensaje a la cola de la capa de transporte y lo despierta si está idle
    (*transport)->AddMessage(message);
    if ((*transport)->idle())
    {
        (*transport)->activate();
    }
    return delay;
}

int Node::GetIsp()
{
    return this->isp;
}

void Node::SetIsp(int isp)
{
    // cout << "Mi id es: " << this->id << " y tengo el isp: " << isp << endl;
    this->isp = isp;
}

void Node::AddMessage(Message * message)
{
    this->num_messages_received++;
    message_stack.push_back(message);
}

void Node::SumDelayAcum(double delay)
{
    this->delay_acum += delay;
    this->num_messages_sended++;
}
