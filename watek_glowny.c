#include "main.h"
#include "watek_glowny.h"

void mainLoop()
{
	srandom(rank);
	int tag;
	int perc;
	queue[rank].cuch_count = 1;

	while (true)
	{
		perc = random() % 100;

		switch (stan)
		{
		case Outside:
			if (perc < 25) {
				println("Chcę wejść do sali")
					debug("Zmieniam stan na wysyłanie");
				packet_t *pkt = malloc(sizeof(packet_t));
				pkt->otaku_cuch_count = queue[rank].cuch_count;
				ackCount = 0;
				for (int i = 0; i < size; i++)
					sendPacket(pkt, i, REQUEST);
				changeState(WaitingEntrance);
				free(pkt);
			}
			debug("Skończyłem myśleć");
			break;

		case WaitingEntrance:
			println("Czekam na wejście do sali");
			queue[rank].is_inside = true;
				// TODO
				// tutaj zapewne jakiś muteks albo zmienna warunkowa
				// bo aktywne czekanie jest BUE
				for(int i = 0;i<size;i++){
				debug("queue[%d]:{lamport=%d, cuch_count=%d, is_inside=%d}",i,queue[i].lamport,queue[i].cuch_count,queue[i].is_inside);
				}
				debug("Front otaku cuchs: %d", front_otaku_cuchs(queue[rank].lamport, queue));
				debug("Cuch count: %d",queue[rank].cuch_count);
				debug("M: %d", M);
				debug("Front otaku count: %d",front_otaku_count(queue[rank].lamport, queue));
				if (ackCount == size && front_otaku_cuchs(queue[rank].lamport, queue) + queue[rank].cuch_count < M && front_otaku_count(queue[rank].lamport, queue) < S) {
					changeState(Inside);
					if (front_otaku_cuchs(queue[rank].lamport, queue) + queue[rank].cuch_count + K > X) {
						packet_t *pkt = malloc(sizeof(packet_t));
						debug("Zgłaszam chęć wymiany organizatora");
						for (int i = 0; i < size; i++)
							sendPacket(pkt, i, TOXIC);
						free(pkt);
					}
					//queue[rank].is_inside = true;
				}
			break;

		case Inside:
			// tutaj zapewne jakiś muteks albo zmienna warunkowa
			println("Jestem w sali");
			if ( perc < 10 ) {
				println("Wychodzę z sali")
					debug("Zmieniam stan na wysyłanie");
				packet_t *pkt = malloc(sizeof(packet_t));
				for (int i = 0; i <= size - 1; i++)
					sendPacket(pkt, (rank + 1) % size, RELEASE);
				queue[rank].cuch_count += random() % 4 + 1;
				debug("Moja nowa liczba cuchów: %d", queue[rank].cuch_count);
				changeState(Outside);
				free(pkt);
			}
			break;

		default:
			break;
		}
		sleep(SEC_IN_STATE);
	}
}
