#ifndef TRANSPORT
#define TRANSPORT

#include "cppsim.hh"
#include "../node/Node.h"
#include "Message.h"
#include <vector>

using namespace std;

class Transport : public Node {
    private:
        vector< handle<Node> * > nodes;
        static double isps[3][3];
    public:
        /**
          * Solo recibe los parámetros necesarios para llamar al constructor de la clase padre Node
          **/
        Transport(const string &name, int id, int type) : Node(name, id, type) {
            //cout << "{*} Transporte iniciado" << endl;
        }
        static void SetIsps(double isps[][3]);
        ~Transport() {}
        void AddNode(handle< Node > * node);
        void AddMessage(Message *message);
        void SetIsp(int);
        void inner_body(void);
        Node *GetServer(int id_, int type);
        Node *GetServer(int id_unique);
        double GetIspDelay(int isp_from, int id_to, int type_to) {
            Node *server = GetServer(id_to, type_to);
            // cout << "Isp from: " << isp_from << " - " << server->GetIsp() << ": " << isps[ server->GetIsp() ][ isp_from ] << endl;
            /*cout << isps[ 0 ][ 0 ] << endl;
            cout << server->GetIsp() << " x " << isp_from << endl;*/
            return isps[ server->GetIsp() ][ isp_from ];
        }
};

#endif