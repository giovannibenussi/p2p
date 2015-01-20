#include "Client.h"
#include "../Constants.h"
#include "../transport/Dns.h"

#include <iostream>
#include <iomanip>
using namespace std;

void Client::inner_body(void) {
    int edge_server_to;
    double delay;
    cout << fixed << setprecision(4);
    while (1) {

        while (!message_stack.empty()) {
            Message * message = message_stack.back();
            // cout << time() << " - Cliente " << setw(3) << this->GetId() << ": recibi respuesta a mi consulta " << message->GetMessage() << " enviada en tiempo " << message->GetCreationTime() << " hacia el servidor " << message->GetIdFrom() << endl;
            // cout << "Delay: " << time() - message->GetCreationTime() << endl;
            SumDelayAcum( time() - message->GetCreationTime() );
            message_stack.pop_back();
            // string * respuesta = new string("respuestaaaaa !!!!!!!");
            // this->SendMessage(new Message(this->GetId(), this->GetType(), message->GetIdFrom(), message->GetTypeFrom(), time(), respuesta));
            if (message->GetTypeFrom() == NODE_EDGE_SERVER) {
                int id_message = -1;
                for (int id = 0; id < unprocessed_message_stack.size(); ++id) {
                    if (unprocessed_message_stack.at(id)->GetMessagePointer()->getQuery() == message->GetMessagePointer()->getQuery()) {
                        id_message = id;
                        break;
                    }
                }
                // Obtengo el mensaje original:
                // Message * original = unprocessed_message_stack.at(id_message);
                unprocessed_message_stack.erase(unprocessed_message_stack.begin() + id_message);
            }
        }

        // cout << this->GetId() << "Dormire por: " << sleep << endl  ;
        if ( ! message_wse_stack.empty() ) {
            string * message_str = new string();
            *message_str = "Hola desde ";
            *message_str +=  this->GetIdString();

            edge_server_to = this->GetEdgeServerId();
            // cout << "CLiente " << this->GetId() << " hacia " << edge_server_to << endl;
            this->querys_send++;
            number_of_querys_sended_this_cycle++;
            MessageWSE * message_wse = message_wse_stack.back();
            message_wse_stack.pop_back();

            this->number_of_querys_sended++;
            // Parametros de esta llamada: 
            // id del creador, tipo creador, id receptor, tipo receptor, 
            // tiempo creacion mensaje, mensaje (de tipo MessageWSE *) 

            // OPCION 1:
            Message * message = new Message(this->GetId(), NODE_CLIENT, edge_server_to, NODE_EDGE_SERVER, time(), message_wse);
            // OPCION 2:

            // FIN OPCIONES
            
            // delay corresponde al delay de enviar mensaje entre isp de emisor y receptor
            delay = this->SendMessage(message);
            unprocessed_message_stack.push_back(message);
        }
        this->passivate();
    }
}

void Client::HaveToSendAMessage() {
    if (this->idle())
        this->activate();
}

int Client::GetEdgeServerId() {
    this->number_of_messages_sended_to_dns++;
    return this->dns->GetEdgeServerId(this->GetId(), this->GetIsp());
}

int Client::GetNumberOfMessagesSendedToDns() {
    return this->number_of_messages_sended_to_dns;
}

void Client::ResetCycle() {
    this->number_of_querys_sended_this_cycle = 0;
}

int Client::GetNumberOfQuerysSendedThisCycle() {
    return number_of_querys_sended_this_cycle;
}

void Client::AddMessageWse(MessageWSE * message_wse) {
    this->message_wse_stack.push_back(message_wse);
}