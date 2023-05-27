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
				println("Chcę wejść do pokoju")
					debug("Zmieniam stan na wysyłanie");
				packet_t *pkt = malloc(sizeof(packet_t));
				pkt->otaku_cuch_count = queue[rank].cuch_count;
				ackCount = 0;
				for (int i = 0; i <= size - 1; i++)
					sendPacket(pkt, i, REQUEST);
				changeState(WaitingEntrance);
				free(pkt);
			}
			debug("Skończyłem myśleć");
			break;

		case WaitingEntrance:
			println("Czekam na wejście do pokoju");
				// TODO
				// tutaj zapewne jakiś muteks albo zmienna warunkowa
				// bo aktywne czekanie jest BUE
				if (ackCount == size && front_otaku_cuchs(lamport, queue) + queue[rank].cuch_count < M && front_otaku_count(lamport, queue) < S) {
					if (front_otaku_cuchs(lamport, queue) + queue[rank].cuch_count + K > X) {
						packet_t *pkt = malloc(sizeof(packet_t));
						debug("Zgłaszam chęć wymiany organizatora");
						for (int i = 0; i <= size - 1; i++)
							sendPacket(pkt, i, TOXIC);
						free(pkt);
					}
					changeState(Inside);
					queue[rank].is_inside = true;
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
					if (i != rank)
						sendPacket(pkt, (rank + 1) % size, RELEASE);
				queue[rank].cuch_count += random() % 4 + 1;
				debug("Moja nowa liczba cuchów: %d", queue[rank].cuch_count);
				changeState(Outside);
				queue[rank].is_inside = false;
				free(pkt);
			}
			break;

		default:
			break;
		}
		sleep(SEC_IN_STATE);
	}
}
