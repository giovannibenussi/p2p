#include "cppsim.hh"
#include "Constants.h"
#include "transport/Dns.h"
#include "transport/Transport.h"
// #include "client/Client.h"
// #include "edgeserver/EdgeServer.h"
#include "peer/Peer.h"
#include "Stats.h"
#include "wse/WSE.h"
#include "generador/gen_rnd.h"

#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;

int NUM_PEERS;
int NUM_EDGE_SERVERS;
int DURACION_SIMULACION;
int ARRIVAL_TIME;
int WSECACHESIZE;
int FLASH_CROWD;
int TIME_WINDOW;
int CROWD_TIME;
int NORMAL_TIME;
int NORMAL_RATE;
int CROWDED_RATE;

double isps[3][3] =
{
    {0.1, 0.5, 0.5},
    {0.5, 0.1, 0.5},
    {0.5, 0.5, 0.1},
};

// vector< handle<Client> > clients;
// handle<Client> * clients;
// handle<EdgeServer> * edge_servers;
handle<Peer> * peers;

class Simulation : public process
{
    public:
        Simulation(const string &name) : process(name) { }
        ~Simulation() { }
        void inner_body()
        {
            rng<double> * arrival_time;
            rng<double> * random_client;
            arrival_time = new rngExp( "Arrive Time", ARRIVAL_TIME );
            random_client = new rngUniform("SelectSource", 0, 100);

            // Iniciar Edge Servers
            // edge_servers = new handle<EdgeServer>[NUM_EDGE_SERVERS];
            // for (int i = 0; i < NUM_EDGE_SERVERS; ++i)
            // {
                // edge_servers[ i ] = new EdgeServer("edge_server", i, NODE_EDGE_SERVER, WSECACHESIZE);
            // }

            // Iniciar Clientes
            peers = new handle<Peer>[ NUM_PEERS ];
            for (int i = 0; i < NUM_PEERS; ++i)
            {
                peers[ i ] = new Peer("cliente", i, NODE_PEER);
            }

            handle<WSE> wse = new WSE("wse", WSECACHESIZE);
            wse->SetIsp( 0 );

            // Iniciar Transporte
            handle<Transport> transport = new Transport("transporte", 0, 0);
            for (int i = 0, max = NUM_PEERS; i < max; i++)
            {
                transport->AddNode((handle< Node > *) &peers[ i ]);
            }
            for (int i = 0, max = NUM_EDGE_SERVERS; i < max; i++)
            {
                // transport->AddNode((handle< Node > *) &edge_servers[ i ]);
            }
            transport->AddNode((handle< Node > *) &wse);
            
            Transport::SetIsps(isps);
            transport->activate();



            Dns * dns = new Dns(peers, isps);

            // for (int i = 0; i < NUM_EDGE_SERVERS; ++i)
            // {
            //     edge_servers[ i ]->SetTransport(&transport);
            //     edge_servers[ i ]->SetIsp( ((double)i / (double)NUM_EDGE_SERVERS) * NUM_ISP );
            // }
            for (int i = 0; i < NUM_PEERS; ++i)
            {
                peers[ i ]->SetTransport(&transport);
                peers[ i ]->SetIsp( ((double)i / (double)NUM_PEERS) * NUM_ISP );
                // peers[ i ]->SetDns(dns);
            }

            wse->SetTransport(&transport);
            wse->activate();

            // handle<Stats> stats = new Stats("stats", DURACION_SIMULACION, edge_servers, dns, TIME_WINDOW);
            // for (int i = 0; i < NUM_EDGE_SERVERS; ++i)
            // {
            //     edge_servers[ i ]->activate();
            // }
            for (int i = 0; i < NUM_PEERS; ++i)
            {
                peers[ i ]->activate();
            }
            // stats->activate();


            char traces[2048];
            strcpy(traces, "../../RISE_16M.DAT");
            // strcpy(traces, "../partial3.DAT");
            // strcpy(traces, "../../../Descargas/partial3.DAT");
            unsigned long int totalQueries = 1000000;
            int Peer_Selection = 0;
            int Nuser = 0;
            int nodes = 100;
            int * ends = new int[nodes + 1];
            for ( int i = 0; i < nodes + 1; i++)
                ends[i] = 0;

            ofstream * chart_file = new ofstream();
            chart_file->open ("charts/query_charts/query_chart");

            ofstream * querys_sended_stream = new ofstream();
            querys_sended_stream->open ("charts/querys_sended_streams/querys_sended_stream");

            handle<Gen_rnd> generator = new Gen_rnd(
                "GENERATOR",
                traces,
                &totalQueries,
                nodes,
                ends,
                Nuser,
                &wse,
                Peer_Selection,
                chart_file,
                querys_sended_stream,
                FLASH_CROWD,
                NORMAL_RATE,
                NORMAL_TIME,
                CROWDED_RATE,
                CROWD_TIME
            );

            generator->SetPeers(peers);
            generator->activate();
            hold(DURACION_SIMULACION);

            chart_file->close();
            querys_sended_stream->close();
            // stats->CloseAll();

            // Generar procecomandos
            ofstream * comandos_charts_stream = new ofstream();
            comandos_charts_stream->open ("comandos_charts/comandos_charts");

            (*comandos_charts_stream) << "plot ";
            for (int i = 0; i < NUM_EDGE_SERVERS; ++i)
            {
                (*comandos_charts_stream) << "'querys_sended_stream' using 1:" << (i + 2) << " with lines, ";
            }
            (*comandos_charts_stream) << endl;

            ofstream cache_hits_stream;
            cache_hits_stream.open("charts/cache_hits");
            vector<double> * cache_hits_vector = new vector<double>[ NUM_EDGE_SERVERS ];
            int highest_cache_hit_vector_length = 0;
            // for (int i = 0; i < NUM_EDGE_SERVERS; ++i)
            // {
            //     cache_hits_vector[ i ] = edge_servers[ i ]->GetCacheHitsVector();
            //     if (cache_hits_vector[ i ].size() > highest_cache_hit_vector_length)
            //     {
            //         highest_cache_hit_vector_length = cache_hits_vector[ i ].size();
            //     }
            // }

            for (int i = 0; i < highest_cache_hit_vector_length; ++i)
            {
                cache_hits_stream << (i * INTERVALO_MEDIR_CACHE_HITS);
                for (int k = 0; k < NUM_EDGE_SERVERS; ++k)
                {
                    if (i < cache_hits_vector[ k ].size())
                    {
                        cache_hits_stream << ", " << cache_hits_vector[ k ].at( i );
                    }
                    else
                    {
                        cache_hits_stream << ", " << 0;
                    }
                }
                cache_hits_stream << endl;
            }
            cache_hits_stream.close();




            // ! Generar comandos

            end_simulation( );
        }
};

void GenerateGraph();

string GenerarResumen(double elapsed_secs)
{
    stringstream ss;
    ss << "Tamano Cache Wse: " << WSECACHESIZE << endl;
    ss << "Clientes:         " << NUM_PEERS << endl;
    ss << "Servidores:    " << NUM_EDGE_SERVERS << endl;
    ss << "Tiempo simulado: " << DURACION_SIMULACION << endl;
    ss << "Tiempo de ejecucion: " << elapsed_secs << " ( 1 -> " << DURACION_SIMULACION / elapsed_secs << ")" << endl;
    ss << endl;
    ss << "===== Statistics =====" << endl;
    ss << "Edge Servers:" << endl;
    double total_idle_time_percentage  = 0;
    double total_busy_time_percentage  = 0;

    // for (int i = 0; i < NUM_EDGE_SERVERS; ++i)
    // {
    //     ss << fixed << setprecision(2);
    //     double idle_time = edge_servers[ i ]->GetIdleTime();
    //     double busy_time = edge_servers[ i ]->GetBusyTime();
    //     double total_time = idle_time + busy_time;

    //     double idle_time_percentage = total_time == 0 ? -1 : ( (idle_time / total_time) * 100 );
    //     double busy_time_percentage = total_time == 0 ? -1 : ( (busy_time / total_time) * 100 );

    //     total_idle_time_percentage += idle_time_percentage;
    //     total_busy_time_percentage += busy_time_percentage;
    //     ss << "\tEdge Server " << i << endl;
    //     // ss << "\t\tIdle Time:  " << idle_time << " s" << endl;
    //     // ss << "\t\tBusy Time:  " << busy_time << " s" << endl << endl;
    //     ss << "\t\tIdle Percentage:  " << idle_time_percentage << " %" << endl;
    //     ss << "\t\tBusy Percentage:  " << busy_time_percentage << " %" << endl;
    //     ss << fixed << setprecision(0);
    //     ss << "\t\tProcessed Querys: " << edge_servers[ i ]->GetProcessedQueries() << endl;
    // }
    // ss << endl;
    // ss << "\tAverage Idle Percentage: " << ( (total_idle_time_percentage / NUM_EDGE_SERVERS) ) << "%" << endl;
    // ss << "\tAverage Processing Time: " << ( (total_busy_time_percentage / NUM_EDGE_SERVERS) ) << "%" << endl;
    // ss << endl;

    ss << "Peers:" << endl;

    // Total consultas enviadas
    unsigned int total_num_messages_sended = 0;
    for (int i = 0; i < NUM_PEERS; ++i)
    {
        total_num_messages_sended += peers[ i ]->GetNumberOfQueriesSended();
        // cout << "Peer " << i << ": " << peers[ i ]->GetNumberOfQueriesSended() << endl;
    }
    ss << "\tTotal querys sended:   " << total_num_messages_sended << endl;
    ss << fixed << setprecision(2);
    ss << "\tAverage querys sended: " << (double)total_num_messages_sended / (double)NUM_PEERS << endl;
    return ss.str();
}

string itos(int i)   // convert int to string
{
    stringstream s;
    s << i;
    return s.str();
}

int main(int argc, char const * argv[])
{
    clock_t begin = clock();

    NUM_PEERS         = argc >  1 ? atoi(argv[ 1])      : 100;
    NUM_EDGE_SERVERS    = argc >  2 ? atoi(argv[ 2])      : 5;
    DURACION_SIMULACION = argc >  3 ? atoi(argv[ 3])      : 100;
    ARRIVAL_TIME        = argc >  4 ? atoi(argv[ 4])      : 1;
    WSECACHESIZE        = argc >  5 ? atoi(argv[ 5])      : 8000;
    FLASH_CROWD         = argc >  6 ? atoi(argv[ 6]) != 0 : true;
    TIME_WINDOW         = argc >  7 ? atoi(argv[ 7])      : 10;
    CROWD_TIME          = argc >  8 ? atoi(argv[ 8])      : 5;
    NORMAL_TIME         = argc >  9 ? atoi(argv[ 9])      : 50;
    NORMAL_RATE         = argc > 10 ? atoi(argv[10])      : 100;
    CROWDED_RATE        = argc > 11 ? atoi(argv[11])      : 500;

    cout << "NUM_PEERS: " << NUM_PEERS << endl;
    cout << "NUM_EDGE_SERVERS: " << NUM_EDGE_SERVERS << endl;
    cout << "DURACION_SIMULACION: " << DURACION_SIMULACION << endl;
    cout << "ARRIVAL_TIME: " << ARRIVAL_TIME << endl;
    cout << "WSECACHESIZE: " << WSECACHESIZE << endl;
    cout << "FLASH_CROWD: " << FLASH_CROWD << endl;
    cout << "TIME_WINDOW: " << TIME_WINDOW << endl;
    cout << "CROWD_TIME: " << CROWD_TIME << endl;
    cout << "NORMAL_TIME: " << NORMAL_TIME << endl;
    cout << "NORMAL_RATE: " << NORMAL_RATE << endl;
    cout << "CROWDED_RATE: " << CROWDED_RATE << endl;



    simulation * sim = simulation::instance();
    sim->begin_simulation( new sqsDll() );

    handle<Simulation> simulation_handle = new Simulation( "source" );
    simulation_handle->activate();

    sim->run();
    sim->end_simulation();

    /* Get Seconds */
    time_t timer;
    struct tm y2k;
    double seconds;

    y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
    y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;
    time(&timer);  /* get current time; same as: timer = time(NULL)  */
    seconds = difftime(timer, mktime(&y2k));

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    cout << GenerarResumen(elapsed_secs) << endl;

    ofstream salida;
    string archivo_salida = "salidas/";
    archivo_salida += itos( seconds ) + "_" + itos(NUM_PEERS) + "_" + itos(NUM_EDGE_SERVERS) + "_" + itos(DURACION_SIMULACION);
    cout << "Archivo salida: " << archivo_salida << endl;
    salida.open(archivo_salida.c_str());
    // salida << GenerarResumen(elapsed_secs);
    salida.close();

    archivo_salida = "cp charts/received_querys_by_edge_servers charts/" + itos( seconds ) + "_received_querys_by_edge_servers";
    system(archivo_salida.c_str());
    archivo_salida = "cp charts/cache_hits_by_edge_servers charts/" + itos( seconds ) + "_cache_hits_by_edge_servers";
    system(archivo_salida.c_str());

    // GenerateGraph();

    return 0;
}

string GetColor(int, int);
string GetColorNode(int, int);

void GenerateGraph()
{
    // int total_edge_querys = 0;
    // for (int i = 0; i < NUM_EDGE_SERVERS; ++i)
    // {
    //     total_edge_querys += edge_servers[ i ]->GetProcessedQueries();
    // }

    // ofstream grafo;
    // grafo.open ("grafo.dot");
    // grafo << "digraph G {" << endl;
    // grafo << "\tsplines=false;" << endl;

    // string string_tamano = ",height=0.30, width=0.30, fixedsize=true,";

    // for (int i = 0; i < NUM_PEERS; ++i)
    // {
    //     grafo << "\tc" << i << "[shape=circle, color=lightblue,style=filled " << string_tamano << "];" << endl;
    // }
    // for (int i = 0; i < NUM_EDGE_SERVERS; ++i)
    // {
    //     grafo << "\te" << i << "[shape=box, color=\"" << GetColorNode(edge_servers[ i ]->GetProcessedQueries(), total_edge_querys) << "\",style=filled " << string_tamano << "];" << endl;
    // }
    // // grafo << "\tDNS[shape=box, color=orange,style=filled " << string_tamano << "];" << endl;


    // int max_querys = 0;
    // int total;
    // int total_total = 0;
    // int total_querys[ NUM_PEERS ];

    // for (int i = 0; i < NUM_PEERS; ++i)
    // {
    //     total = 0;
    //     for (int j = 0; j < NUM_EDGE_SERVERS; ++j)
    //     {
    //         total += edge_servers[ j ]->GetQuerysByClient(i);
    //     }
    //     if (total > max_querys)
    //         max_querys = total;
    //     total_querys[ i ] = total;
    //     total_total += total;
    // }

    // // Clients -> EdgeServers
    // for (int i = 0; i < NUM_PEERS; ++i)
    // {
    //     for (int j = 0; j < NUM_EDGE_SERVERS; ++j)
    //     {
    //         if (edge_servers[ j ]->GetQuerysByClient(i) > 0)
    //             grafo << "\t" << "\"c" << i << "\" -> \"e" << j << "\"[color=\"" << GetColor(edge_servers[ j ]->GetQuerysByClient(i), max_querys) << "\"];" << endl;
    //     }
    // }

    // // Clients -> DNS
    // for (int i = 0; i < NUM_PEERS; ++i)
    // {
    //     for (int j = 0; j < NUM_EDGE_SERVERS; ++j)
    //     {
    //         if (edge_servers[ j ]->GetQuerysByClient(i) > 0)
    //         {
    //             // grafo << "\t" << "\"c" << i << "\" -> \"DNS\"" << endl;
    //         }
    //     }
    // }

    // grafo << "}";
    // grafo.close();
}

string GetColor(int querys, int total_querys)
{
    // rdylgn11 color scheme
    // http://www.graphviz.org/doc/info/colors.html#brewer
    vector<string> colors;
    colors.push_back("#006837");
    colors.push_back("#1a9850");
    colors.push_back("#66bd63");
    colors.push_back("#a6d96a");
    colors.push_back("#d9ef8b");
    colors.push_back("#ffffbf");
    colors.push_back("#fee08b");
    colors.push_back("#fdae61");
    colors.push_back("#f46d43");
    colors.push_back("#d73027");
    colors.push_back("#a50026");
    // ss << querys << " / " << total_querys << endl;
    // ss << ((double)querys / (double)total_querys * 10) << endl;
    return colors.at( ((double)querys / (double)total_querys * (colors.size() - 1 ) ) );
}

string GetColorNode(int querys, int total_querys)
{
    // rdylgn11 color scheme
    // http://www.graphviz.org/doc/info/colors.html#brewer
    vector<string> colors;
    colors.push_back("#edf8fb");
    colors.push_back("#b3cde3");
    colors.push_back("#8c96c6");
    colors.push_back("#8856a7");
    colors.push_back("#810f7c");
    if (total_querys == 0)
        return colors.at(0);
    // ss << querys << " / " << total_querys << endl;
    // ss << ((double)querys / (double)total_querys * 10) << endl;
    return colors.at( ((double)querys / (double)total_querys * (colors.size() - 1 ) ) );
}
