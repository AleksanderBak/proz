#ifndef UTILH
#define UTILH
//#include "main.h"

/* typ pakietu */
typedef struct {
    int ts;       /* timestamp (zegar lamporta */
    int src;  
    int otaku_cuch_count;     /* przykładowe pole z danymi; można zmienić nazwę na bardziej pasującą */
} packet_t;
/* packet_t ma trzy pola, więc NITEMS=3. Wykorzystane w inicjuj_typ_pakietu */

typedef struct {
    int lamport;
    int cuch_count;
    bool is_inside;
} otaku_request;

#define NITEMS 3

/* Typy wiadomości */
/* TYPY PAKIETÓW */
#define ACK     1
#define REQUEST 2
#define RELEASE 3
#define TOXIC 4

extern MPI_Datatype MPI_PAKIET_T;
void inicjuj_typ_pakietu();

/* wysyłanie pakietu, skrót: wskaźnik do pakietu (0 oznacza stwórz pusty pakiet), do kogo, z jakim typem */
void sendPacket(packet_t *pkt, int destination, int tag);

typedef enum {Outside, WaitingEntrance, Inside} state_t;
extern state_t stan;
extern pthread_mutex_t stateMut;
extern pthread_mutex_t lamportMut;

/* zmiana stanu, obwarowana muteksem */
void changeState( state_t );

int front_otaku_cuchs(int lamport, otaku_request queue[]);

int front_otaku_count(int lamport, otaku_request queue[]);


#endif
