/*
 ============================================================================
 Name        : Tema4
 Author      : Gabriel Ivanica
 ============================================================================
 */

#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include <mpi.h>

using namespace std;

// Variabila pentru DEBUG
int DBG = 0;

// Conditie de afisare debug in fisier de output
#define DEBUG	if(DBG) out

// Vecinii unui nod
int *V;

// Nexthop pentru un nod - rutare
int *next_hop;

// Vector de topologie temporara primita ca ecou de la un nod
int *topology;

// Mesaj temporar
int temp;

// Parintele unui nod
int parent = -1;

// Numar de ecos ce trebuie primite
int eco_left = 0;

// Numarul de mesaje trimise dar neconfirmate
int unconfirmed_msg = 0;

// Stream pentru afisarea in fisier
fstream out;

/*
 *	Taguri MPI pentru receptionare si trimitere mesaje intre procese
 */
enum MsgType {
	sonda,
	ecou,
	ecou_vid,
	syncronize,
	message,
	confirm,
	broadcast,
	done_sending,
	vote
};

/*
 *	Taguri MPI pentru receptionare si trimitere mesaje intre procese
 *	Valori rezultante in urma alegerilor
 */
enum VoteMsgType {
	ELECTION,
	RESULT,
	LEADER,
	ASSISTANT
};

/*
 *	Structura pentru a trimite un mesaj
 *		- nodul sursa
 *		- nodul destinatie
 *		- mesaj propriu zis
 */
struct Message {
	int sender;
	int destination;		
	char msg[256];
};

/*
 *	Structura pentru a trimite un mesaj de vot cu informatiile necesare
 *		- leaderul ales
 *		- asistentul ales
 *		- resultatul votului
 */
struct VoteMessage {
	int sender;
	int destination;		
	int leader;
	int assistant;
	int result;
	char msg[256];
};

/*
 * Deschidere fisier de output / debugging
 */
void openDebugFile(int rank) {

	char file[20]; 
	sprintf(file, "out%d.txt", rank);

	out.open(file, fstream::out);
}

/*
 *	Debug Functions for printing arrays
 */
void printArray(int *a, int size, string message, bool force = false) {

	if (DBG == 0 && force == false)
		return;

	out << message << ": \t";
	out << "{";
	for(int i = 0; i < size; i++) {
		out << a[i] << " ";
	}
	out << "}" << endl;

}

void printArrayInfo(int *a, int size, string message) {
	out << message << "\n";
	for(int i = 0; i < size; i++) {
		out << i << " => " << a[i] << "\n";
	}
	out << endl;
}

void printTask(string mesaj, int rank) {
	out << endl;
	out << "==================================================" << endl;
	out << mesaj << "\t" << "NOD " << rank << endl;
	out << endl;
}

/*
 *	Printare informatii mesaj simplu
 */
template <typename T>
void printMsg(T M, int source) {
	out << "sender: " << M.sender << "\t"
		<< "from: " << source << "\t\t"
		<< "dest: ";

	if (M.destination >= 0)
		out << M.destination;
	else
		out << 'B';

	out << "\t\t" << "msg: '" << M.msg << "'" << endl;
}

/*
 *	Printare informatii despre vot
 */
void printVoteInfo(VoteMessage VM) {

	out << "Leader: " << VM.leader << "\t"
		<< "Assistant: " << VM.assistant << "\t"
		<< "Result: " << VM.result << endl;
}

/*
 * Citire noduri adiacente pentru fiecare proces
 */
inline void readInput(char *file, int rank, int size) {

	fstream fin;
	fin.open(file, fstream::in);
	stringstream adjlist (stringstream::in | stringstream::out);

	char s[1000];
	int node, vecin;

	fin >> node;
	while (node != rank) {
		fin.getline(s, 1000);
		fin >> node;
	}

	fin >> s[0];
	fin.getline(s, 1000);

	adjlist << s;

	V = (int*) calloc(size, sizeof(int));
	next_hop = (int*) calloc(size, sizeof(int));
	topology = (int*) calloc(size, sizeof(int));


	while(adjlist >> vecin) {
		V[vecin] = 1;
		next_hop[vecin] = vecin;
	}

	for (int i = 0; i<size; i++)
		next_hop[i] = -1;

	fin.close();
}

/*
 * Citirea mesajelor si trimiterea acestora in retea
 * Pe baza topologiei mesajele sunt trimise
 *		- la destinatie
 *		- la nexthop
 *		- broadcast
 */
void sendMessages(char *file, int rank, int size) {

	fstream msgf;
	msgf.open(file, fstream::in);

	int nr;
	char c;
	string dest;

	Message M;

	msgf >> nr;

	for (int i = 0; i < nr; i++) {
		msgf >> M.sender;

		if (M.sender == rank) {

			msgf >> dest;

			if (dest[0] == 'B')
				M.destination = -1;
			else
				M.destination = atoi(dest.c_str());

			msgf.get(c);
			msgf.getline(M.msg, 256);

			M.msg[strlen(M.msg) - 1] = '\0';

			printMsg(M, rank);

			if (M.destination >= 0) {

				out << " => send to " << next_hop[M.destination] << "\n";

				MPI_Send(&M, sizeof(M), MPI_BYTE, next_hop[M.destination], message, MPI_COMM_WORLD);

				unconfirmed_msg++;
			}
			else {
				for (int i=0; i<size; i++) {
					if (V[i] == 1) {
						out << " => send to " << i << "\n";
						MPI_Send(&M, sizeof(M), MPI_BYTE, i, broadcast, MPI_COMM_WORLD);
					}
				}
			}
			out << "\n";
		}
		else
			msgf.getline(M.msg, 256);
	}

	msgf.close();
}

/*
 * Trimite mesaj SONDA la toate nodurile
 */
void inline sendProbeToAll(int size) {

	DEBUG << "Trimitere sonda la nodurile : ";
	for (int i = 1; i<size; i++)
		if (V[i] && parent != i) {
			DEBUG << i << " "; 
			MPI_Send(&temp, 1, MPI_INT, i, sonda, MPI_COMM_WORLD);
			eco_left++;
		}
	DEBUG << endl;
}

/*
 * Trimite Broadcast :
 *		tag : tag
 *		exceptie : source (nu trimite catre acest nod)
 */
template <typename T>
void inline broadcastMsg(T M, int size, int source, int tag) {

	for (int i=0; i<size; i++) {
		if (V[i] == 1 && i != source) {

			if(tag != done_sending || DBG)
				out << " => forwarded to " << i << "\n";

			MPI_Send(&M, sizeof(M), MPI_BYTE, i, tag, MPI_COMM_WORLD);
		}
	}

	if(tag != done_sending || DBG)
		out << endl;
}

/*
 * Trimite mesaj de terminare transmitere mesaje
 */
void inline sendDoneSendingMessages(int source, int size) {

	Message M;

	M.sender = source;
	M.destination = -1;
	strcpy(M.msg, "done sending messages");

	printMsg(M, source);

	broadcastMsg(M, size, source, done_sending);

	if (DBG == 0)
		out << endl;

}

/*
 * Calculare nod castigator pe baza numarului de voturi
 */
int getWinner(int *votes, int size) {
	int max = -1;
	int winner = -1;

	for (int i=0; i<size; i++) {
		if (votes[i] > max) {
			max = votes[i];
			winner = i;
		}
	}

	for (int i=0; i<size; i++) {
		if (votes[i] == max && i != winner)
			return -1;
	}

	return winner;
}

/*
 * Generarea unui VOT random
 */
int getVote(int size, int rank) {

	int vote;

	do {
		vote = rand() % size;
	} while (vote == rank);

	return vote;
}

int main(int argc, char *argv[]) {

	int rank, nr_processes, i;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nr_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	srand(time(NULL) * rank);

	if (argc == 4)
		DBG = 1;

	///////////////////////////////////////////////////////
	// Deschidere fisierelor si citirea listelor de vecini

	// Fisierele de output pentru fiecare proces in parte
	openDebugFile(rank);

	out << "INFO NOD : " << rank << "\n";

	readInput(argv[1], rank, nr_processes);

	printArray(V, nr_processes, "Vecini");


	///////////////////////////////////////////////////////
	// Stabilire topologie
	// Se trimit mesaje de tip sonda si se asteapta un ecou
	///////////////////////////////////////////////////////

	if (rank == 0) {

		parent = rank;

		out << "Descoperire topologie" << "\n";
	}

	else {

		MPI_Recv(&temp, 1, MPI_INT, MPI_ANY_SOURCE, sonda, MPI_COMM_WORLD, &status);

		parent = status.MPI_SOURCE;
	}

	out << "NOD parinte : " << parent << "\n";

	printTask("TOPOLOGIE", rank);

	sendProbeToAll(nr_processes);

	// Se captureaza mesajele ECO si se construieste topologia pe baza arborelui minim

	while (eco_left) {

		MPI_Recv(topology, nr_processes, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		int tag = status.MPI_TAG;
		int source = status.MPI_SOURCE;

		if (tag == sonda) {
			DEBUG << "Primit sonda de la " << source << "\n";
			MPI_Send(&temp, 1, MPI_INT, source, ecou_vid, MPI_COMM_WORLD);
			continue;
		}

		if (tag == ecou_vid)
			DEBUG << "Primit ecou vid de la " << source << "\n";
				
		if (tag == ecou) {

			DEBUG << "Primit ecou de la " << source << "\n";

			for (i=0; i<nr_processes; i++)
				if (topology[i] != -1)
					next_hop[i] = source;
		}

		eco_left--;
	}

	DEBUG << endl;

	
	// Determinarea Topologiei nodului curent si trimiterea acesteia catre parinte
	next_hop[rank] = rank;

	if (rank) {

		MPI_Send(next_hop, nr_processes, MPI_INT, parent, ecou, MPI_COMM_WORLD);

		for (int i = 0; i < nr_processes; i++) {
			if (next_hop[i] == -1) {

				if(V[i] && i != parent)
					V[i] = 0;

				next_hop[i] = parent;
			}
		}
		V[parent] = 1;
	}

	printArray(V, nr_processes, "Vecini");
	DEBUG << endl;
	printArrayInfo(next_hop, nr_processes, "Hext hop");


	///////////////////////////////////////////////////////
	// Topologia a fost determinata
	///////////////////////////////////////////////////////

	/*
	 *	Se asteapta sincronizarea tuturor proceselor pana in acest punct
	 *	Echivalent cu MPI_BARRIER()
	 */

	out << "Syncronize all process" << endl;

	if (rank == 0) 
		broadcastMsg(temp, nr_processes, rank, syncronize);
	else {
		MPI_Recv(&temp, 1, MPI_INT, MPI_ANY_SOURCE, syncronize, MPI_COMM_WORLD, &status);
		broadcastMsg(temp, nr_processes, parent, syncronize);
	}

	///////////////////////////////////////////////////////
	// Trimitere mesaje
	///////////////////////////////////////////////////////

	printTask("TRANSMITERE MESAJE", rank);

	// Citire mesaje si transmitere catre destinatie 
	sendMessages(argv[2], rank, nr_processes);

	// Notare conexiuni in asteptare
	int nr_connections = nr_processes;
	int *connection = new int[nr_processes];

	// Notare conexiuni in asteptare
	for (int i=0; i< nr_processes; i++)
		connection[i] = 1;

	Message M;

	int finish = 0;

	while(nr_connections) {

		DEBUG << "\n********** Receivend New Message ********** " << "\n";

		MPI_Recv(&M, sizeof(M), MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		int tag = status.MPI_TAG;
		int source = status.MPI_SOURCE;

		// Afisare mesaje
		if(tag != done_sending || DBG)
			printMsg(M, source);

		// Terminare primire mesaje de la un nod x
		if (tag == done_sending && connection[M.sender] == 1) {

			connection[M.sender] = 0;

			nr_connections--;

			broadcastMsg(M, nr_processes, source, done_sending);
		}

		// Transmitere Mesaj / Forwarding
		if (tag == message) {

			if (rank == M.destination) {

				M.destination = M.sender;
				M.sender = rank;
				string cnf = string("confirm => ").append(M.msg);
				strcpy(M.msg, cnf.c_str());

				out << " => received " << "\n";

				printMsg(M, rank);
				out << " => forwarded to " << source << "\n";

				MPI_Send(&M, sizeof(M), MPI_BYTE, source, confirm, MPI_COMM_WORLD);
			}
			else {
				out << " => forwarded to " << next_hop[M.destination] << "\n";
				MPI_Send(&M, sizeof(M), MPI_BYTE, next_hop[M.destination], message, MPI_COMM_WORLD);
			}

			out << endl;
		}

		// Confirmare mesaje
		if (tag == confirm) {

			if (rank == M.destination) {
				out << " => received " << "\n";
				unconfirmed_msg --;
			}
			else {
				out << " => forwarded to: " << next_hop[M.destination] << "\n";
				MPI_Send(&M, sizeof(M), MPI_BYTE, next_hop[M.destination], confirm, MPI_COMM_WORLD);
			}

			out << endl;

		}

		// Primire mesaje Broadcast si Forwardare mai departe
		if (tag == broadcast) {
			out << " => received " << "\n";
			broadcastMsg(M, nr_processes, source, broadcast);
		}

		// Trimiterea confirmare inchidere conexiune
		if(unconfirmed_msg == 0 && finish == 0) {
			finish = 1;
			// Broadcast terminare trimitere mesaje proprii
			sendDoneSendingMessages(rank, nr_processes);
		}

		// Terminarea executiei nodului curent
		if (nr_connections == 1 && unconfirmed_msg == 0) {
			nr_connections--;
			connection[rank] = 0;
		}


		DEBUG << "Connections opened : " << nr_connections << "\n";
		printArray(connection, nr_processes, "Connections");
	}


	///////////////////////////////////////////////////////
	// Trimitere mesaje
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// Alegerea liderului
	///////////////////////////////////////////////////////

	printTask("ALEGEREA LIDERULUI", rank);

	// Numarul de voturi primit
	int votes_received = 0;

	// Vectorii de voturi pentru Leader si Assistant
	int *Lvotes = (int*) calloc(nr_processes, sizeof(int));
	int *Avotes = (int*) calloc(nr_processes, sizeof(int));

	// Leaderul si Asistentul ales
	int leader = -1;
	int assistant = -1;

	// Mesaj de comunicatie
	VoteMessage VM;

	// Trimiterea cerintei de vot
	if (rank == 0 ) {

		VM.destination = -1;
		VM.sender = rank;
		VM.leader = 0;
		VM.assistant = 0;
		VM.result = ELECTION;
		strcpy(VM.msg, "Your country needs you!");

		printMsg(VM, rank);
		printVoteInfo(VM);

		broadcastMsg(VM, nr_processes, rank, ELECTION);
	}

	out << endl;

	// Primire voturi si inregistrare a acestora
	while(1) {

		out << "\n********** Receivend New Message ********** " << "\n";

		MPI_Recv(&VM, sizeof(VoteMessage), MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		int tag = status.MPI_TAG;
		int source = status.MPI_SOURCE;

		printMsg(VM, source);
		printVoteInfo(VM);

		// Se primeste cerere de votare si este trimis votul catre nodul ce colecteaza
		// Se trimite votul pentru ambii sau doar pentru tip de vot
		if (tag == ELECTION) {

			broadcastMsg(VM, nr_processes, source, ELECTION);

			if (VM.result == ELECTION || VM.result == LEADER)
				VM.leader = getVote(nr_processes, rank);

			if (VM.result == ELECTION || VM.result == ASSISTANT)
				VM.assistant = getVote(nr_processes, rank);

			VM.destination = VM.sender;
			VM.sender = rank;
			strcpy(VM.msg, "Vote");

			printMsg(VM, rank);
			printVoteInfo(VM);

			out << " => sent to: " << next_hop[VM.destination] << "\n";

			MPI_Send(&VM, sizeof(VM), MPI_BYTE, source, vote, MPI_COMM_WORLD);
		}

		// Rutare mesaje intre noduri
		if (tag == vote && rank != VM.destination) {
			out << " => forwarded to: " << next_hop[VM.destination] << "\n";
			MPI_Send(&VM, sizeof(VM), MPI_BYTE, next_hop[VM.destination], vote, MPI_COMM_WORLD);
		}

		// Calculare voturi si determinare finalizare program
		if (tag == vote && rank == VM.destination) {

				votes_received++;

				out << " => received " << "\n\n";

				if (VM.result == ELECTION || VM.result == LEADER) {
					Lvotes[VM.leader]++;
					printArray(Lvotes, nr_processes, "Leader", true);
				}

				if (VM.result == ELECTION || VM.result == ASSISTANT) {
					Avotes[VM.assistant]++;
					printArray(Avotes, nr_processes, "Assistant", true);
				}

				if (votes_received == nr_processes - 1) {

					votes_received = 0;

					if (VM.result == ELECTION || VM.result == LEADER)
						leader = getWinner(Lvotes, nr_processes);
					
					if (VM.result == ELECTION || VM.result == ASSISTANT)
						assistant = getWinner(Avotes, nr_processes);

					VM.leader = leader;
					VM.assistant = assistant;
					VM.sender = rank;
					VM.destination = -1;

					if (leader != -1 && assistant != -1 && leader != assistant) {

						VM.result = RESULT;
						strcpy(VM.msg, "Success");

						out << "\n";
						printMsg(VM, rank);
						printVoteInfo(VM);

						broadcastMsg(VM, nr_processes, rank, RESULT);
						break;
					}
					
					if (leader == assistant)
						assistant = -1;

					if (leader == -1 && assistant == -1) {

						memset(Lvotes, 0, sizeof(int) * nr_processes);
						memset(Avotes, 0, sizeof(int) * nr_processes);

						VM.result = ELECTION;
						VM.leader = -1;
						VM.assistant = -1;
						strcpy(VM.msg, "Election Failed");
					}

					else if (leader == -1) {
						memset(Lvotes, 0, sizeof(int) * nr_processes);

						VM.result = LEADER;
						VM.leader = -1;
						strcpy(VM.msg, "Failed Election Leader");
					}

					else if (assistant == -1) {
						memset(Avotes, 0, sizeof(int) * nr_processes);

						VM.result = ASSISTANT;
						VM.assistant = -1;
						strcpy(VM.msg, "Failed Election Assistant");
					}

					out << "\n";
					out << "########################################################################\n";
					out << "###########################   DECISION   ###############################\n\n";
					printMsg(VM, rank);
					printVoteInfo(VM);
					broadcastMsg(VM, nr_processes, rank, ELECTION);
				}
			}


		if (tag == RESULT) {

			broadcastMsg(VM, nr_processes, source, RESULT);
			break;
		}
	}

	out <<"\n";
	out << "Alegeri Terminate" << "\n";
	out << "BUNCAR LEADER : " << VM.leader << "\n";
	out << "BUNCAR ASISTENT : " << VM.assistant << "\n";

	if (rank == 0) {
		cout <<"\n";
		cout << "Alegeri Terminate" << "\n";
		cout << "BUNCAR LEADER : " << VM.leader << "\n";
		cout << "BUNCAR ASISTENT : " << VM.assistant << "\n";
	}

	///////////////////////////////////////////////////////
	// Alegerea liderului
	///////////////////////////////////////////////////////

	// Finalizare
	MPI_Finalize();

	return 0;
}

