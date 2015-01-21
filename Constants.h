#ifndef CONSTANTS
#define CONSTANTS

//#define NUM_CLIENTS 10000
//#define NUM_EDGE_SERVERS 10
//#define DURACION_SIMULACION 10 // seconds

//int NUM_CLIENTS;
//int NUM_EDGE_SERVERS;
//int DURACION_SIMULACION;

extern int NUM_PEERS;
extern int NUM_EDGE_SERVERS;
extern int DURACION_SIMULACION;

#define NODE_CLIENT 	   0
#define NODE_EDGE_SERVER   1
#define NODE_ORIGIN_SERVER 2
#define NODE_DNS		   3
#define NODE_PEER		   3

// #define WSECACHESIZE 8000
#define SIZE_ANSWER 1
#define STATIC_TIMEOUT 100 // cambiar por dinamico

// FLASH CROWDS
// #define FLASH_CROWD true  // se utiliza o no el flash crowd
// #define CROWD_TIME 5.0 // segundos con flash crowd
// #define NORMAL_TIME 50.0 //segundos con tasa normal
// #define NORMAL_RATE 100.0 // tasa de arribo sin flash crowd
// #define CROWDED_RATE 500.0 // tasa de arribo con flash crowd

#define QUERY_RATE_STRATEGY 0
#define QUERY_DELTA_T 10000 //CADA 10000 Unidades de Tiempo
#define QUERY_DELTA_Q 10000 //CADA 10000 QUERIES
#define MIN_TTL 100 //1 Hour
#define MAX_TTL 86400 //24 Hours
#define TTL_WSE 180 // 3min
#define TIMEOUT_Q 100 // Time out de las consultas

#define MAX_BUFFER 2048

#define D 10

#define DELTA_O 100 // Observer check

#define PEER 411
#define USER 412

#define NUM_ISP 3

// #define TIME_WINDOW 10 // cada cuantos segundos registrará datos

#define INTERVALO_MEDIR_CACHE_HITS 50

#define PEER_CACHE_SIZE 100

typedef plb::LRUCacheH4<string, int> lru_cache;

#endif