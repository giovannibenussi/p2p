#include "Dns.h"

inline int abs(int a) {
    return a < 0 ? -a : a;
}

/****************************************************************************************
 *  Nombre: GetEdgeServerId
 *  Funcion: Asigna un edge server al client i que pertenece al isp isp
 *
 *  Parametros:
 *      - int id: id del cliente
 *      - int isp: isp del cliente
 *
 *  Retorno: id del EdgeServer asignado al cliente
 ****************************************************************************************/
int Dns::GetEdgeServerId(int id, int isp) {
    return 0;
}

// El reseteo se realiza en stats.cpp cada TIME_WINDOW segundos
void Dns::ResetCycle() {
    for (int i = 0; i < NUM_EDGE_SERVERS; ++i) {
        // num_messages_received_by_edge_servers_cycle[ i ] = 0;
    }
}