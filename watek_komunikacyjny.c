#include "main.h"
#include "watek_komunikacyjny.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */
void *startKomWatek(void *ptr)
{
    MPI_Status status;
    int is_message = FALSE;
    packet_t pakiet;
    /* Obrazuje pętlę odbierającą pakiety o różnych typach */
    while (TRUE) {
	debug("czekam na recv");
        MPI_Recv( &pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        pthread_mutex_lock( &lamportMut );
        lamport = (lamport > pakiet.ts) ? lamport + 1: pakiet.ts + 1;
        pthread_mutex_unlock( &lamportMut );

        switch ( status.MPI_TAG ) {
	    case REQUEST: 
            debug("Ktoś coś prosi. A niech ma!");
            packet_t *pkt = malloc(sizeof(packet_t));
            pkt->ts = lamport;
            queue[pakiet.src].is_inside = true;
            queue[pakiet.src].cuch_count = pakiet.otaku_cuch_count;
            queue[pakiet.src].lamport = pakiet.ts;
            sendPacket(pkt, status.MPI_SOURCE, ACK );
            free(pkt);
            break;

	    case ACK: 
            debug("Dostałem ACK od %d, mam już %d", status.MPI_SOURCE, ackCount);
	        ackCount++; /* czy potrzeba tutaj muteksa? Będzie wyścig, czy nie będzie? Zastanówcie się. */
	        break;

        case TOXIC:
            K = 0;
            if (queue[rank].is_inside) {
                packet_t *pkt = malloc(sizeof(packet_t));
                pkt->ts = lamport;
                for (int i = 0; i <= size - 1; i++)
					sendPacket(pkt, (rank + 1) % size, RELEASE);
                free(pkt);            
            }
            sleep(5);
            break;

        case RELEASE:
            queue[pakiet.src].is_inside = false;
            K += queue[pakiet.src].cuch_count;
            break;

	    default:
	    break;
        }
    }
}
