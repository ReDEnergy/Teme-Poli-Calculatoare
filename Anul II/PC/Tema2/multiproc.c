#include "helpers.h"

typedef struct {
	int d1;
	int d2;
	int type;
	char even[LINE_SIZE];
} ev;

//NU AVETI VOIE SA MODIFICATI acest fisier!

int simulare(int pipec[KIDS][4], ev *events, int nr_ev)
{
//se vor folosi  pipec[k][0] pentru citire si pipec[k][3] pentru scriere			    
	
	
	int i, k, nod, nod1, nod2;
	int outpipe;
	msg mesaj, mesaj2;

	fd_set read_fds;	//fd_set folosit in select()
	fd_set tmp_fds;	//fd_set folosit temporar
	int fdmax=0;		//nr maxim de file descriptori
	int timp = -1;
	int last_topologie = 0; //ultimul nod adaugat in topologie - are cel mai mare id
	int mes_fin = 0;
	int nr_msg = 0;
	int tr[KIDS][2];
	int cit, nodmax;
	//golim read_fds
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);
	
	//initial in topologie aven nodul cu id-ul 0
	//adaugam in multimea read_fds, pipe-descriptorul din care citim de la nodul 0 - el exista de la inceput in topologie 
	FD_SET(pipec[0][0], &read_fds);
	fdmax = pipec[0][0];

	FILE *fislog = fopen("log_rutare","w");
	DIE (fislog == NULL, "fopen fisier iesire" );
	// main loop
	while((mes_fin != last_topologie+1) || (timp < nr_ev)) { 
	
		tmp_fds = read_fds; 
		
		DIE (select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) < 0, "select"); 
		
		for(i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {
				cit = read (i, &mesaj, sizeof(msg));
			
				DIE (cit < 0, "citire mesaj simulator" );  //server citeste, client scrie	
			
				if (cit == 0)  //procesul child a iesit si a inchis pipe-ul
					continue;
				
				switch (mesaj.type) {
			
				//1,2,3,4 sunt mesaje din protocolul link state; se trimit la next hop
				case 1:
				case 2:
				case 3:
				case 4: {
					printf ("Timp %d, Simulator , msg tip%d de la %d\n", timp, mesaj.type, mesaj.sender);
					nr_msg++; //numar de mesaje de ale protocolului(tip 1,2,3,4) in aceasta runda 
					
					if (mesaj.type ==4) {
						fprintf (fislog, "\nRutare timp %d, creator %d, nr_secv %d, sender %d, next_hop %d\n", mesaj.timp, mesaj.creator, mesaj.nr_secv, mesaj.sender, mesaj.next_hop); 
					}
			
					if ( (mesaj.next_hop < 0 ) || (mesaj.next_hop > last_topologie)) {
						printf("Nodul %d nu este (inca) in topologie!", mesaj.next_hop);
						exit(-1);
					}
					
					outpipe = pipec[mesaj.next_hop][3];
					write(outpipe, &mesaj, sizeof(msg));
					
				}
					break; 
				
				//5 finalizare procesare la pasul t	
				case 5: {
					printf ("Timp %d, Simulator , msg tip %d de la %d\n", timp, mesaj.type, mesaj.sender);
					mes_fin++;  //numar cate rutere au terminat procesarea la pasul trecut
					mesaj.sender = -1;
					mesaj2.sender = -1;
					
					if (mes_fin == last_topologie+1) { //daca toate nodurile din topologie au finalizat procesarea mesajelor de la timpul curent
						if (timp+1 < nr_ev) {  //daca mai am evenimente, le anunt nodurilor direct implicate
							mesaj.type = 7; //anunt eveniment
							sprintf (mesaj.payload, "%s\n", events[timp+1].even);	
							
							nod1 = events[timp+1].d1;
							mesaj.join = FALSE; //adevarat doar pentru eveniment tip 1
							
							if (events[timp+1].type == 1) { //adaugare nod la topologie
								last_topologie ++; //creste topologia cu 1
								//adaug descriptor pipe citire in multimea la care ascult
								FD_SET(pipec[nod1][0], &read_fds);
								if (fdmax < pipec[nod1][0])
									fdmax = pipec[nod1][0];
								mesaj.join = TRUE;	
								mesaj.timp = timp;  //instiintez noul nod de pasul de timp al simularii
							}
							
							outpipe = pipec[nod1][3];
							write(outpipe, &mesaj, sizeof(msg));
						
							nod2 = events[timp+1].d2;
							if (nod2 > -1){
								outpipe = pipec[nod2][3];
								write(outpipe, &mesaj, sizeof(msg));  //mesaj 7 anunt eveniment
							
							}
						}

						if ((nr_msg == 0) && (timp >= nr_ev)) {//daca nu s-au primit mesaje (din protocolul linkstate) in aceasta runda si nu mai sunt evenimente
							mesaj.type = 9; //s-a terminat simularea
							nodmax = KIDS - 1;
						}	
						else {
							mesaj.type = 6; //se da liber la procesare mesaje vechi (primite la t-1) din coada
							mes_fin = 0;
							nr_msg = 0;
							timp ++;
							nodmax = last_topologie;
						}
					
						for (nod = 0; nod <= nodmax; nod++) {
							outpipe = pipec[nod][3];
						
							mesaj2.type = 8; 
							write(outpipe, &mesaj2, sizeof(msg));  //mesaj 8 cer tabela de rutare
							
							write(outpipe, &mesaj, sizeof(msg));  //mesaj 6 sau 9
						}	
					}
				}
					break; 
				
				case 10:
						memcpy (&tr, mesaj.payload, sizeof(tr));
						fprintf (fislog, "\n Timp %d, tabela rutare ruter %d\n", timp, mesaj.sender);
						for (k = 0; k < KIDS; k++) {
							fprintf (fislog, "%d %04d %d\n", k, tr[k][0], tr[k][1]);
						}
						break;
				
				default: 
					printf ("\n EROARE: Timp %d, Simulator, msg tip %d - NU PROCESEZ ACEST TIP DE MESAJ\n", timp, mesaj.type);
					exit (-1);	
			
				}
			}
		}
	}
fclose(fislog);
return 0;
}


int main (int argc, char ** argv) {
 
	
	int   fd  [KIDS][4];
	pid_t pid [KIDS];
	int k;
	// flag to avoid  'if pid == 0' repetition
	int child = FALSE;
	// each child needs to know what pipe to use
	int  cidx = -1;
	
	char buf[BUFSIZE];
	char buf2[BUFSIZE];
	char buf3[BUFSIZE];
	
	
	int i, tip_ev, nr_ev;
	
	FILE *f = fopen(argv[1],"rt");
	DIE (f == NULL, "fopen fisier intrare" );
	fscanf(f, "%d ", &nr_ev);
	ev * v_ev = (ev *) malloc(nr_ev * sizeof (ev)) ;
	
	//salvez evenimentele din fisierul de intrare intr-un vector; la momentul potrivit, il voi trimite la nodul/nodurile direct implicate
	for (i = 0; i < nr_ev; i++) {
		fgets(v_ev[i].even, LINE_SIZE-1, f);
		sscanf (v_ev[i].even, "%d %d %d", &tip_ev, &v_ev[i].d1, &v_ev[i].d2);
		if ((tip_ev == 1) || (tip_ev == 4))
			v_ev[i].d2 = -1;	
		v_ev[i].type = tip_ev;
	}
	
	fclose(f);
	
	for(k=0; k<KIDS; k++){

		if (!child) {
		
			DIE (pipe(fd[k]) < 0, "creare pereche 1 pipe-uri" );  //server citeste, client scrie
			DIE (pipe(&(fd[k][2])) < 0, "creare pereche 2 pipe-uri" );  //server scrie, client citeste
		
			pid[k] = fork();	
			DIE (pid[k] < 0, "fork"); 
			
			if (pid[k]==0){   //CHILD
			
				child = TRUE;
				cidx = k;
					
				DIE (close(fd[cidx][0]) < 0, "close pipe1 child" ); //inchid capetele nefolosite ale pipe-urilor
				DIE (close(fd[cidx][3]) < 0, "close pipe2 child" ); 
				
				memset(buf,0,BUFSIZE);
				sprintf (buf, "%d", fd[cidx][1]);
				memset(buf2,0,BUFSIZE);
				sprintf (buf2, "%d", fd[cidx][2]);
				memset(buf3,0,BUFSIZE);
				sprintf (buf3, "%d", cidx);
				
	 			execl("./ruter","ruter", buf, buf2, buf3, NULL);  //trimit proceselor fiu ca argumente pipe de scriere, pipe de citire si indicele nodului
				
				fprintf(stderr, "Execution failed for client %s %s %s\n",  buf, buf2, buf3);
				fflush(stdout);
				exit(EXIT_FAILURE);
			} 

			else {  //PARENT -> pid [k] > 0
				
				DIE (close(fd[k][1]) < 0, "close pipe1 parent" ); //inchid capetele nefolosite ale pipe-urilor
				DIE (close(fd[k][2]) < 0, "close pipe2 parent" ); 
		   	}
		} //if
	}//for
	// this is outside the for loop

	if (!child) {
	
		//argumente simulare: vectorul de pipe-uri, vectorul de evenimente, numarul de evenimente
		DIE(simulare (fd, v_ev, nr_ev) < 0, "eroare in simulare");		
		for(k=0; k<KIDS;k++){
			// wait for this child to finish
			DIE(waitpid (pid[k], NULL, 0) < 0, "waitpid");
			close (fd[k][0]);
			close (fd[k][3]);
		}
	}  //if

return 0;
} //main

