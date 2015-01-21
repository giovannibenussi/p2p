#ifndef STATS
#define STATS

#include "cppsim.hh"
#include "Constants.h"
#include "transport/Dns.h"
#include "peer/Peer.h"
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>

class EdgeServer;

using namespace std;

class Stats : public process
{
    private:
        handle<EdgeServer> * edge_servers;
        static unsigned int cycles;
        ofstream * received_querys_by_edge_servers;
        ofstream cache_hits_by_edge_servers;
        ofstream cache_usage_stream;
        ofstream cache_hits_stream;
        ofstream cache_miss_stream;
        int DURACION_SIMULACION;
        Dns * dns;
        int TIME_WINDOW;
        handle<Peer> * peers;
    public:
        /**
          *
          * - name: utilizado por libcppsim.
          * - DURACION_SIMULACION: duración de la simulación (se utiliza para calcular el tiempo
                                   restante aproximado).
          * - edge_servers: puntero al arreglo de edge servers creado en main.cpp
          * - dns: puntero a la instancia de dns creada en main.cpp
          * - TIME_WINDOW: indica cada cuanto tiempo se recogerán estadísticas
          *
          **/
        Stats(const string &name, int DURACION_SIMULACION, Dns * dns, int TIME_WINDOW, handle<Peer> * peers) : process(name)
        {
            this->peers = peers;
            this->edge_servers        = edge_servers;
            this->dns                 = dns;
            this->DURACION_SIMULACION = DURACION_SIMULACION;
            this->TIME_WINDOW         = TIME_WINDOW;

            // Archivo para guardar las consultas recibidas por los edge servers en un formato
            // que puede leer gnuplot
            received_querys_by_edge_servers = new ofstream();
            received_querys_by_edge_servers->open("charts/received_querys_by_edge_servers");


            /*    Cache Hits File */
            // Archivo para guardar los cache hits de los edge servers en un formato
            // que puede leer gnuplot
            cache_hits_by_edge_servers.open("charts/cache_hits_by_edge_servers");
            cache_usage_stream.open("charts/cache_usage");

            cache_hits_stream.open("charts/cache_hits_y_miss");

            /*// Cache Hits File */
            this->GenerateCommands();
        }
        ~Stats()
        {

        }
        void inner_body();
        unsigned int GetCycles()
        {
            return this->cycles;
        }
        void GenerateCommands()
        {
            ofstream comandos;
            comandos.open("charts/comandos");

            /*   Received Queries by Edge Servers Commands (Doesn't Include Total)*/
            comandos << "Received Queries By Edge Servers:" << endl;;
            comandos << "set title 'Received Queries By Edge Servers';";
            comandos << "set xlabel 'Time';";
            comandos << "set ylabel '#Received Queries / second';";
            comandos << "plot ";
            for (int i = 0; i < NUM_EDGE_SERVERS; ++i)
            {
                comandos << "'charts/received_querys_by_edge_servers' using 1:" << (i + 2) << " with lines title \"EdgeServer " << i << "\", ";
            }
            /*-- Received Queries by Edge Servers Commands */
            comandos << endl;
            comandos << endl;

            /*    Cache Hits Commands */
            comandos << "Cache Hits By Edge Servers:" << endl;
            comandos << "set title 'Cache Hits By Edge Servers';";
            comandos << "set xlabel 'Time';";
            comandos << "set ylabel 'Cache Hits / second';";
            comandos << "plot ";
            for (int i = 0; i < NUM_EDGE_SERVERS; ++i)
            {
                comandos << "'charts/cache_hits_by_edge_servers' using 1:" << (i + 2) << " with lines title \"EdgeServer " << i << "\", ";
            }
            /* // Cache Hits Commands */

            comandos << endl;
            comandos << endl;

            /*    Cache Hits Commands */
            comandos << "Cache Hits By Edge Servers:" << endl;
            comandos << "set title 'Cache Hits By Edge Servers';";
            comandos << "set xlabel 'Time';";
            comandos << "set ylabel 'Cache Hits / second';";
            comandos << "plot ";
            for (int i = 0; i < NUM_EDGE_SERVERS; ++i)
            {
                comandos << "'charts/cache_hits' using 1:" << (i + 2) << " with lines title \"EdgeServer " << i << "\", ";
            }
            /* // Cache Hits Commands */
        }
        void CloseAll()
        {
            received_querys_by_edge_servers->close();
            cache_hits_by_edge_servers.close();
            cache_usage_stream.close();
        }

};

#endif
