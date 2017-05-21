#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <vector>
#include "helpers.h"
#include "rut.h"

#define DRUMAX 10000


/****************************
 **    Message Handlers    **
 ****************************/

/** Aceasta functie adauga un nou mesaj in coada pentru a  fi procesat
 **/

void addMessage(msg &mesaj) {
	unsigned int i;
	for (i=0; i<ReceiveMsg.size(); i++)
		if (ReceiveMsg[i].type == mesaj.type &&
			ReceiveMsg[i].creator == mesaj.creator &&
			ReceiveMsg[i].nr_secv < mesaj.nr_secv)
		{
			ReceiveMsg[i] = mesaj;
			return;
		}

	ReceiveMsg.push_back(mesaj);
}

/*********************
 **      Events     **
 *********************/

/** Functie necesare pentru a extrage urmatoarea valoare Int din payload
 ** Necesara la parsarea Evenimentelor
 **/

int getNextInt (char *payload) {
	int nr = 0;
	nr = atoi(&payload[Ev.pos++]);
	while (payload[Ev.pos]<58 && payload[Ev.pos]>47)
		Ev.pos++;
	return nr;
}

/** Functia creaza si trimite un mesaj Database Request catre nodul - hop
 **/

void Send_Database_Request( int hop, int cost) {
	//fprintf(out, "----------------------Database Request Start\n");
	msg mesaj;
	mesaj.timp = timp;
	mesaj.type = 2;
	mesaj.sender = nod_id;
	mesaj.creator = nod_id;
	mesaj.next_hop = hop;
	mesaj.nr_secv = NR_SECV++;
	mesaj.len = cost;
	write (pipeout, &mesaj, sizeof(msg));
	//fprintf(out, "\tSent to %d\n", hop);
	//fprintf(out, "----------------------Database Request End\n");
}

/*****************************
 **      Message TYPE 1     **
 *****************************/

/**	Functia va face update la LSADatabase pentru un router X
 **/

int Update_Nod_X_LSA (int msg_pos) {
	int nr, updated = 0;
	msg receive = ReceiveMsg[msg_pos];
	LSA routerLSA;
	memcpy(&routerLSA, &receive.payload, sizeof(LSA));
	nr = receive.creator;

	//Print_Info_LSA (routerLSA, nr);
	//fprintf(out, "----------------------Updating NOD X LSA Start\n");

	if (LSADatabase[nr].time < routerLSA.time) {
		LSADatabase[nr] = routerLSA;
		updated = 1;
	}

	//PrintLSA(nr);
	//fprintf(out, "----------------------Updating NOD X LSA End\n");
	return updated;
}

/**	Update Oglinda - se face update si la toti vecinii routerului X in
 ** LSA-ul vecinilor din LSADatabase - pentru legatura cu nod_id
 **/

void Update_Nod_X_Link_LSA (int msg_pos) {
	LSA routerLSA;
	msg receive = ReceiveMsg[msg_pos];
	int nr = receive.creator;
	memcpy(&routerLSA, &receive.payload, sizeof(LSA));
	for (int i=0; i<KIDS; i++) {
		if (routerLSA.cost[i] > 0 && routerLSA.time > LSADatabase[i].time)
			LSADatabase[i].cost[nr] = routerLSA.cost[i];
	}
}

/**	Functia va forwarda un LSA catre toti vecinii sai cu exceptia celui de unde
 ** a primit mesajul
 **/

void Forward_LSA (int msg_pos) {
	//fprintf(out, "----------------------Forward LSA Start\n");
	msg mesaj = ReceiveMsg[msg_pos];
	int x = mesaj.sender;
	mesaj.sender = nod_id;
	mesaj.timp = timp;

	for (int i=0; i<KIDS; i++) {
		if (LSADatabase[nod_id].cost[i] > 0 && i!=x) {
				mesaj.next_hop = i;
				write (pipeout, &mesaj, sizeof(msg));
				//fprintf(out, " Forward LSA to %d\n", i);
		}
	}
	//fprintf(out, "----------------------Forward LSA End\n");
}


/*****************************
 **      Message TYPE 2     **
 *****************************/

/** Functia genereaza mesaje LSA pt Database Reply, si le trimite creatorului
 **/

void Send_Database_Reply(int msg_pos) {
	//fprintf(out, "----------------------Database Reply Start\n");
	msg mesaj = ReceiveMsg[msg_pos];
	mesaj.type = 3;
	mesaj.sender = nod_id;
	mesaj.next_hop = mesaj.creator;

	for (int i=0; i<KIDS; i++)
		if (LSADatabase[i].time > 0) {
			mesaj.len = i;
			memcpy(mesaj.payload, &LSADatabase[i], sizeof(LSA));
			write (pipeout, &mesaj, sizeof(msg));
			//fprintf(out, "\tSent LSAdb - router %d\n", i);
		}
	//fprintf(out, "----------------------Database Reply End\n");
}

/**	Functia necesara pentru a updata LSADatabase pentru nodul curent - nod id
 **/

void Update_Nod_ID_LSA (int msg_pos) {
	//fprintf(out, "----------------------Updating NOD ID LSA Start\n");
	msg receive = ReceiveMsg[msg_pos];
	LSADatabase[nod_id].time = timp;
	LSADatabase[nod_id].cost[receive.creator] = receive.len;
	//PrintLSA(nod_id);
	//fprintf(out, "----------------------Updating NOD ID LSA End\n");
}

/**	Update in LSADatabase la toti vecinii routerului nod_id cu informatii din
 **	LSA => nod_id
 **/
void Update_Nod_ID_LINK_LSA () {
	for (int i=0; i<KIDS; i++)
		if (LSADatabase[i].time < LSADatabase[nod_id].time && LSADatabase[nod_id].cost[i] > 0)
			LSADatabase[i].cost[nod_id] = LSADatabase[nod_id].cost[i];

}

/**	Functia propaga un mesaj LSA nod_id catre toti vecinii acestuia
 **/

void Send_Nod_ID_LSA () {
	//fprintf(out, "----------------------Propagate NOD ID LSA Start\n");
	msg mesaj;
	mesaj.type = 1;
	mesaj.sender = nod_id;
	mesaj.nr_secv = NR_SECV++;
	mesaj.creator = nod_id;
	mesaj.timp = timp;
	memcpy(mesaj.payload, &LSADatabase[nod_id], sizeof(LSA));
	//fprintf(out,"INFO nonpacked:\n");
	//PrintLSA(nod_id);

	for (int i=0; i<KIDS; i++) {
		if (LSADatabase[nod_id].cost[i] > 0) {
			mesaj.next_hop = i;
			write (pipeout, &mesaj, sizeof(msg));
			//fprintf(out, " Sent LSA to %d\n", i);
		}
	}
	//fprintf(out, "----------------------Propagate NOD ID LSA End\n");
}

/**	Functie necesara pentru a sterge un Link dintre nod_id si vecin X
 **/

void Delete_Link_Nod_ID (int vecin) {
	//fprintf(out, "----------------------Delete Link NOD ID Start\n");
	LSADatabase[nod_id].time = timp;
	LSADatabase[nod_id].cost[vecin] = 0;
	//PrintLSA(nod_id);
	//fprintf(out, "----------------------Delete Link NOD ID End\n");
}


/*****************************
 **      Message TYPE 3     **
 *****************************/

/**	Functia necesara pentru a procesa Database Reply prin compararea LSA primite
 ** cu valorile din LSADatabase si inlocuirea lor daca este nevoie
 **/
void Save_Database_Reply(int msg_pos) {
	msg receive = ReceiveMsg[msg_pos];
	LSA routerLSA;
	memcpy(&routerLSA, &receive.payload, sizeof(LSA));
	int nr = receive.len;

	//fprintf(out, "----------------------Save_Database_Reply Start\n");

	if (LSADatabase[nr].time < routerLSA.time) {
		LSADatabase[nr] = routerLSA;

		for (int i=0; i<KIDS; i++) {
			if (routerLSA.cost[i] > 0 && routerLSA.time > LSADatabase[i].time)
				LSADatabase[i].cost[nr] = routerLSA.cost[i];
		}
	}

	//PrintLSA(nr);
	//fprintf(out, "----------------------Save_Database_Reply End\n");
}


/*****************************
 **    New Routing Table    **
 *****************************/

/**	Bellman Ford pentru a calcula distata minima de la nod_id catre nodul X
 ** Se calculeaza si next_hop
 **	d[i] - costul drumului nod_id = > destinatie
 ** p[i] - next_hop pentru packet
 **/

void Compute_Routing_Table () {
	int i, j, d[KIDS], p[KIDS];

	for (i=0; i<KIDS; i++) {
		if (LSADatabase[nod_id].cost[i] > 0) {
			d[i] = LSADatabase[nod_id].cost[i];
			p[i] = i;
		}
		else {
			d[i] = 10000;
			p[i] = -1;
		}
	}

	// relaxari succesive
	for (i=0; i<KIDS; i++) {
		for (j=0; j<KIDS; j++) {
			if (j == nod_id)
				continue;
			if (LSADatabase[i].cost[j] > 0) {
				if (d[j] > d[i] + LSADatabase[i].cost[j]) {
					d[j] = d[i] + LSADatabase[i].cost[j];
					p[j] = i;
				}
			}
		}
	}

	// Salvare informatii in tabela de rutare
	for (i=0; i<KIDS; i++) {
		tab_rutare[i][0] = d[i];
		tab_rutare[i][1] = p[i];
	}

	//fprintf(out, "\n_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_  TABELA DE RUTARE\n");

	for (i=0; i<KIDS; i++) {
		//fprintf(out, "%d %d %d \n", i, d[i] , p[i]);
	}

	//fprintf(out, "\n_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_  TABELA DE RUTARE\n");

}



/*****************************
 **      Message TYPE 4     **
 *****************************/

/**	Mesaj necesar pentru a trimte mai departe un mesaj Type 4
 ** Doar in cazul in care se poate fowarda
 **/

void Forward_Message (int msg_pos) {
	msg *mesaj = &ReceiveMsg[msg_pos];

	if (mesaj->len == nod_id) {
		//fprintf(out, "Mesajul a ajuns la Destinatie\n");
		return;
	}

	if (tab_rutare[mesaj->len][1] > 0) {
		mesaj->next_hop = tab_rutare[mesaj->len][1];
		write (pipeout, mesaj, sizeof(msg));
	}
}

/**	Crearea unui pachet type 4 pentru a fi trimis catre destinatie daca se poate
 **/

void Create_Routing_Message (int destination) {
	if (tab_rutare[destination][1] < 0) {
		//fprintf(out, "Imposibil de trimis mesajul la Destinatie\n");
		return;
	}

	msg mesaj;
	mesaj.type = 4;
	mesaj.sender = nod_id;
	mesaj.creator = nod_id;
	mesaj.nr_secv = NR_SECV++;
	mesaj.timp = timp;
	mesaj.len = destination;
	mesaj.next_hop = tab_rutare[destination][1];

	write (pipeout, &mesaj, sizeof(msg));
	//fprintf(out, " Sent Pack to %d\n", tab_rutare[destination][1]);
}


/*****************************
 **      Main Function      **
 *****************************/

int main (int argc, char ** argv)
{
	pipeout = atoi(argv[1]);
	pipein = atoi(argv[2]);
	nod_id = atoi(argv[3]); //procesul curent participa la simulare numai dupa ce nodul cu id-ul lui este adaugat in topologie
	timp =-1 ;
	int gata = FALSE;
	msg mesaj;
	int cit, k;

	//OpenDebuggingFile();

	//nu modificati numele, modalitatea de alocare si initializare a tabelei de rutare - se foloseste la mesajele de tip 8/10, deja implementate si la logare
	//tab_rutare[k][0] reprezinta costul drumului minim de la ruterul curent (nod_id) la ruterul k
								//tab_rutare[k][1] reprezinta next_hop pe drumul minim de la ruterul curent (nod_id) la ruterul k

	for (k = 0; k < KIDS; k++) {
		tab_rutare[k][0] = 10000;  // drum =DRUMAX daca ruterul k nu e in retea sau informatiile despre el nu au ajuns la ruterul curent
		tab_rutare[k][1] = -1; //in cadrul protocolului(pe care il veti implementa), next_hop =-1 inseamna ca ruterul k nu e (inca) cunoscut de ruterul nod_id (vezi mai sus)
	}

	printf ("Nod %d, pid %u alive & kicking\n", nod_id, getpid());

	if (nod_id == 0) { //sunt deja in topologie
		timp = -1; //la momentul 0 are loc primul eveniment
		mesaj.type = 5; //finish procesare mesaje timp -1
		mesaj.sender = nod_id;
		write (pipeout, &mesaj, sizeof(msg));
		printf ("TRIMIS Timp %d, Nod %d, msg tip 5 - terminare procesare mesaje vechi din coada\n", timp, nod_id);

	}

	while (!gata) {
		cit = read(pipein, &mesaj, sizeof(msg));

		if (cit <= 0) {
			printf ("Adio, lume cruda. Timp %d, Nod %d, msg tip %d cit %d\n", timp, nod_id, mesaj.type, cit);
			exit (-1);
		}

		switch (mesaj.type) {

			//1,2,3,4 sunt mesaje din protocolul link state;
			//actiunea imediata corecta la primirea unui pachet de tip 1,2,3,4 este buffer-area (punerea in coada /coada new daca sunt 2 cozi - vezi enunt)
			//mesajele din coada new se vor procesa atunci cand ea devine coada old (cand am intrat in urmatorul pas de timp)
			case 1:
			//	printf ("Timp %d, Nod %d, msg tip 1 - LSA\n", timp, nod_id);
			//	Se adauga mesajul in coada pentru a fi procesat la t+1
				addMessage(mesaj);
				break;

			case 2:
			//	printf ("Timp %d, Nod %d, msg tip 2 - Database Request\n", timp, nod_id);
			//	Se adauga mesajul in coada pentru a fi procesat la t+1
				addMessage(mesaj);
				break;

			case 3:
			//	printf ("Timp %d, Nod %d, msg tip 3 - Database Reply\n", timp, nod_id);
			//	Se adauga mesajul in coada pentru a fi procesat la t+1
				addMessage(mesaj);
				break;

			case 4:
			//	printf ("Timp %d, Nod %d, msg tip 4 - pachet de date (de rutat)\n", timp, nod_id);
			//	Se adauga mesajul in coada pentru a fi procesat la t+1
				addMessage(mesaj);
				break;

			case 6://complet in ceea ce priveste partea cu mesajele de control
					//puteti inlocui conditia de coada goala, ca sa corespunda cu implementarea personala
				  //aveti de implementat procesarea mesajelor ce tin de protocolul de rutare
				{
				timp++;
				printf ("Timp %d, Nod %d, msg tip 6 - incepe procesarea mesajelor puse din coada la timpul anterior (%d)\n", timp, nod_id, timp-1);

				//veti modifica ce e mai jos -> in scheletul de cod nu exista nicio coada
				//daca NU mai am de procesat mesaje venite la timpul anterior
				//(dar mai pot fi mesaje venite in acest moment de timp, pe care le procesez la t+1)
				//trimit mesaj terminare procesare pentru acest pas (tip 5)
				//altfel, procesez mesajele venite la timpul anterior si apoi trimit mesaj de tip 5
				//	procesez tote mesajele din coada old
				//	(sau toate mesajele primite inainte de inceperea timpului curent - marcata de mesaj de tip 6)
				//	la acest pas/timp NU se vor procesa mesaje venite DUPA inceperea timpului curent
				//cand trimiteti mesaje de tip 4 nu uitati sa setati (inclusiv) campurile, necesare pt logare:  mesaj.timp, mesaj.creator, mesaj.nr_secv, mesaj.sender, mesaj.next_hop
				//la tip 4 - creator este sursa initiala a pachetului rutat

				//fprintf(out, "\n");
				//fprintf(out, "Time : %d\n", timp);
				//fprintf(out, "Coada : %d\n", ReceiveMsg.size());

				unsigned int i;
				for (i=0; i<ReceiveMsg.size(); i++) {
					//InfoMesaj(i);

					// Prelucrare Mesaj Type 1
					if (ReceiveMsg[i].type == 1) {
						if (Update_Nod_X_LSA(i)) {
							Update_Nod_X_Link_LSA(i);
							Forward_LSA(i);
						}
						continue;
					}

					// Prelucrare Mesaj Type 2
					if (ReceiveMsg[i].type == 2) {
						Send_Database_Reply(i);
						Update_Nod_ID_LSA(i);
						Update_Nod_ID_LINK_LSA();
						Send_Nod_ID_LSA();
						continue;
					}

					// Prelucrare Mesaj Type 3
					if (ReceiveMsg[i].type == 3) {
						Save_Database_Reply(i);
						continue;
					}


					// Prelucrare Mesaj Type 4
					if (ReceiveMsg[i].type == 4) {
						Forward_Message(i);
						continue;
					}
				}


				// Golire Coada de Mesaje
				ReceiveMsg.clear();

				// Afisare LSADatabase =>debugging
				// PrintLSADatabase();

				// Calcularea tabelei de rutare dupa finalizarea prelucrarii mesajelor
				Compute_Routing_Table();

				//acum coada_old e goala, trimit mesaj de tip 5
					mesaj.type = 5;
					mesaj.sender = nod_id;
					write (pipeout, &mesaj, sizeof(msg));
				}
				break;

			case 7: {
				//complet in ceea ce priveste partea cu mesajele de control
				//aveti de implementat tratarea evenimentelor si trimiterea mesajelor ce tin de protocolul de rutare
				//in campul payload al mesajului de tip 7 e linia de fisier (%s) corespunzatoare respectivului eveniment
				//vezi multiproc.c, liniile 88-115 (trimitere mes tip 7) si liniile 184-1KIDS4 (parsare fisiere evenimente)

				//rutere direct implicate in evenimente, care vor primi mesaje de tip 7 de la simulatorul central:
				//eveniment tip 1: ruterul nou adaugat la retea  (ev.d1  - vezi liniile indicate)
				//eveniment tip 2: capetele noului link (ev.d1 si ev.d2)
				//eveniment tip 3: capetele linkului suprimat (ev.d1 si ev.d2)
				//evenimet tip 4:  ruterul sursa al pachetului (ev.d1)

				// Pozitie initiala pentru a parsa payload
				Ev.pos = 1;
				Ev.type = atoi(&mesaj.payload[0]);

				//fprintf(out, "\n********************* Event :%s",mesaj.payload);

				if (mesaj.join == TRUE) {
					timp = mesaj.timp;
					timp++;
					//	printf ("Nod %d, msg tip eveniment - voi adera la topologie la pasul %d\n", nod_id, timp+1);

					Ev.A = getNextInt(mesaj.payload);
					Ev.nr = getNextInt(mesaj.payload);

					// Actualizare timp pentru LSA curent
					LSADatabase[nod_id].time = timp;

					// Adauga un nou router in schema
					for (int j=0; j<Ev.nr; j++) {
						Ev.B = getNextInt(mesaj.payload);
						Ev.cost = getNextInt(mesaj.payload);

						//fprintf(out, "B: %d,  Cost: %d \n",Ev.B, Ev.cost);

						// Send DatabaseRequest
						Send_Database_Request(Ev.B, Ev.cost);

						// Update DatabaseLSA
						LSADatabase[nod_id].cost[Ev.B] = Ev.cost;
					}
					timp--;
				}
				else {
					// Incrementare timp pentru a prelucra mai usor evenimentele
					// Se revine la timp-- inainte de iesirea din else

					timp++;
					printf ("Timp %d, Nod %d, msg tip 7 - eveniment\n", timp, nod_id);

					// Prelucrare Evenimente
					switch (Ev.type) {

						// Eveniment 2
						case 2:
							Ev.A = getNextInt(mesaj.payload);
							Ev.B = getNextInt(mesaj.payload);
							Ev.cost = getNextInt(mesaj.payload);

							//fprintf(out, "Type: %d,  A: %d,  B: %d,  cost: %d \n",Ev.type, Ev.A, Ev.B, Ev.cost);

							if (Ev.A == nod_id)
								Send_Database_Request(Ev.B, Ev.cost);
							if (Ev.B == nod_id)
								Send_Database_Request(Ev.A, Ev.cost);
							timp--;
							break;

						// Eveniment 3
						case 3:
							Ev.A = getNextInt(mesaj.payload);
							Ev.B = getNextInt(mesaj.payload);

							//fprintf(out, "Type: %d,  A: %d,  B: %d \n",Ev.type, Ev.A, Ev.B);

							if (Ev.A == nod_id)
								Delete_Link_Nod_ID(Ev.B);
							if (Ev.B == nod_id)
								Delete_Link_Nod_ID(Ev.A);
							Send_Nod_ID_LSA();
							Compute_Routing_Table();
							timp--;
							break;

						// Eveniment 4
						case 4:
							Ev.A = getNextInt(mesaj.payload);
							Ev.B = getNextInt(mesaj.payload);

							//fprintf(out, "Type: %d,  A: %d,  B: %d \n",Ev.type, Ev.A, Ev.B);

							Create_Routing_Message(Ev.B);

							timp--;
							break;

						default: break;
					}
				}
			}
				break;

			case 8: //complet implementat - nu modificati! (exceptie afisari on/off)
				{
			//	printf ("Timp %d, Nod %d, msg tip 8 - cerere tabela de rutare\n", timp+1, nod_id);
				mesaj.type = 10;  //trimitere tabela de rutare
				mesaj.sender = nod_id;
				memcpy (mesaj.payload, &tab_rutare, sizeof (tab_rutare));
				//Observati ca acest tip de mesaj (8) se proceseaza imediat - nu se pune in nicio coada (vezi enunt)
				write (pipeout, &mesaj, sizeof(msg));
				}
				break;

			case KIDS: //complet implementat - nu modificati! (exceptie afisari on/off)
				{
				//Aici poate sa apara timp -1 la unele "noduri"
				//E ok, e vorba de procesele care nu reprezentau rutere in retea, deci nu au de unde sa ia valoarea corecta de timp
				//Alternativa ar fi fost ca procesele neparticipante la simularea propriu-zisa sa ramana blocate intr-un apel de read()
			//	printf ("Timp %d, Nod %d, msg tip KIDS - terminare simulare\n", timp, nod_id);
				gata = TRUE;
				}
				break;


			default:
			//	printf ("\nEROARE: Timp %d, Nod %d, msg tip %d - NU PROCESEZ ACEST TIP DE MESAJ\n", timp, nod_id, mesaj.type);
				exit (-1);
		}
	}

return 0;

}


/****************************
 **   Debugging Function   **
 ****************************/

void PrintLSADatabase () {
	fprintf(out, "\n");
	fprintf(out, "----------------------LSA_DATABSE Start\n");
	for (int i=0;i<KIDS;i++)
		PrintLSA(i);
	fprintf(out, "----------------------LSA_DATABSE End\n");
	fprintf(out, "\n");
}


void PrintLSA (int pos) {
	int i;
	fprintf(out, "LSA %d: - time %d => \t", pos, LSADatabase[pos].time);
	for (i=0; i<KIDS; i++)
		fprintf(out, "%d ", LSADatabase[pos].cost[i]);
	fprintf(out, "\n");
}

void Print_Info_LSA (LSA x, int router) {
	int i;
	fprintf(out,"INFO FROM PACK\n");
	fprintf(out, "LSA %d: - time %d => \t", router, x.time);
	for (i=0; i<KIDS; i++)
		fprintf(out, "%d ", x.cost[i]);
	fprintf(out, "\n");
}

void InfoMesaj(int pos) {
	fprintf(out, "\n");
	fprintf(out, "----------------------Info Mesaj Start\n");
	fprintf(out, "\t Type %d\n", ReceiveMsg[pos].type);
	fprintf(out, "\t Creator %d\n", ReceiveMsg[pos].creator);
	fprintf(out, "\t Sender %d\n", ReceiveMsg[pos].sender);
	fprintf(out, "----------------------Info Mesaj End\n");
	fprintf(out, "\n");
}

void OpenDebuggingFile () {
	char filename[20];
	sprintf(filename,"out%d.txt", nod_id);
	printf("START PROGRAM ********************************** %s\n", filename);
	out = fopen(filename,"w");
}
