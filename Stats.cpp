#include "Stats.h"
// #include "edgeserver/EdgeServer.h"
#include "Constants.h"

unsigned int Stats::cycles = 0;

static inline void loadBar(int x, int n, int w, double time) {
    // ANSI Control codes to go back to the
    // previous line and clear it.
    printf("\n\033[F\033[J");

    // Calculuate the ratio of complete-to-incomplete.
    float ratio = x / (float)n;
    int   c     = ratio * w;

    // Show the percentage complete.
    printf("Elapsed: %4.2f Left: %4.2f %3d%% [", time, ( time * 100 / (int)(ratio * 100) ) - time, (int)(ratio * 100) );

    // Show the load bar.
    for (int x = 0; x < c; x++)
        printf("=");

    for (int x = c; x < w; x++)
        printf(" ");

    printf("]");
}

void Stats::inner_body() {
    clock_t begin = clock();
    // unsigned int previous_total_received_queries_by_edge_servers[ NUM_EDGE_SERVERS ];
    // for (int i = 0; i < NUM_EDGE_SERVERS; ++i) {
    //     previous_total_received_queries_by_edge_servers[ i ] = 0;
    // }

    cout << ".";

    while (1) {
        loadBar(time(), DURACION_SIMULACION, 100, double(clock() - begin) / CLOCKS_PER_SEC);

        // // Imprimo el tiempo en cada archivo
        // (*received_querys_by_edge_servers) << time() << ", ";
        // cache_hits_by_edge_servers << time() << ", ";
        // cache_usage_stream << time() << ", ";

        // // Escribo en los archivos
        // for (int i = 0; i < NUM_EDGE_SERVERS; ++i) {
        //     (*received_querys_by_edge_servers) << ( edge_servers[ i ]->GetReceivedQueriesByClients() - previous_total_received_queries_by_edge_servers[ i ] ) << ", ";
        //     cache_hits_by_edge_servers << ( edge_servers[ i ]->GetReceivedQueriesByClients() > 0 ? ( (double)edge_servers[i]->GetCacheHitsReceivedQueriesByClients() / (double)edge_servers[ i ]->GetReceivedQueriesByClients() ) : 0 ) << ", ";
        //     // cache_hits_by_edge_servers << ( edge_servers[ i ]->GetReceivedQueriesByClients() > 0 ? ( (double)(edge_servers[i]->GetReceivedQueriesByClients() - edge_servers[ i ]->GetCacheMissReceivedQueriesByClients() ) / (double)edge_servers[ i ]->GetReceivedQueriesByClients() ) : 0 ) << ", ";
        //     cache_usage_stream << edge_servers[ i ]->GetUsedCache() << ", ";
        //     previous_total_received_queries_by_edge_servers[ i ] = edge_servers[ i ]->GetReceivedQueriesByClients();
        // }

        dns->ResetCycle();

        // (*received_querys_by_edge_servers) << endl;
        // cache_hits_by_edge_servers << endl;
        // cache_usage_stream << endl;

        cycles++;
        hold(TIME_WINDOW);
    }
}
