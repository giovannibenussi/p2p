#ifndef MESSAGE
#define MESSAGE value

#include <string>
#include <cstdlib>
#include "../wse/MessageWSE.h"

using namespace std;

class Message {
    private:
        // Atributos del nodo emisor
        int id_from; // Identificador dentro del conjunto de nodos de tipo type_from
        // Tipo de nodo emisor (e.g.: NODE_CLIENT, NODE_EDGE_SERVER, NODE_ORIGIN_SERVER, NODE_DNS,etc)
        // Nota: Los tipos están definidos en Constants.h
        int type_from;
        int id_unique_from;

        // Atributos del nodo receptor
        int id_to; // Identificador dentro del conjunto de nodos de tipo type_to
        // Tipo de nodo receptor
        int type_to;
        int id_unique_to;

        double creation_time; // tiempo de creación (obtenido de un llamado a time() dentro de un nodo)
        MessageWSE * message; // Mensaje que contiene la query, utilizado para mantener compatibilidad
        unsigned long int id; // identificador único del mensaje
        // Número de instancias de Message creadas (se utiliza)
        // para generar el id único
        static unsigned long int instances;

    public:
        /**
          *
          * - id_from: id del creador del mensaje.
          * - type_from: tipo del creador del mensaje.
          * - id_to: id del receptor del mensaje.
          * - type_to: tipo del receptor del mensaje.
          * - creation_time: cuando se crea el mensaje, esta variable se llama como time().
          * - message: Mensaje generado por gen_rnd (de tipo MessageWSE *) que contiene los datos
          *            necesarios para conocer la consulta y poder manejar un cache con ésta.
          * - 
          *
          **/
        Message(int id_from, int type_from, int id_to, int type_to, double creation_time, MessageWSE * message) {
            this->message = message;

            this->id_from = id_from;
            this->type_from = type_from;

            this->id_to = id_to;
            this->type_to = type_to;

            this->creation_time = creation_time;

            // Asigno un identificador único basado en el número de instancias
            // (Ej. de los valores que tomará: 0, 1, 2, 3, 4, 5, ...)
            this->id = Message::instances;
            Message::instances++;
        }
        
        Message(int id_unique_from, int id_unique_to, double creation_time, MessageWSE * message){
            this->id_unique_from = id_unique_from;
            
            this->id_unique_to   = id_unique_to;
            
            this->creation_time = creation_time;

            Message::instances++;
        }
        virtual ~Message() {
            free(message);
        }
        MessageWSE GetMessage(){ return * message; }
        inline MessageWSE * GetMessagePointer(){ return message; }
        int GetTypeFrom(){ return type_from; }
        int GetTypeTo(){ return type_to; }
        int GetIdFrom(){ return id_from; };
        int GetIdTo(){ return id_to; };
        double GetCreationTime(){ return creation_time ; }
        unsigned long int GetId(){ return this->id; }
};

#endif