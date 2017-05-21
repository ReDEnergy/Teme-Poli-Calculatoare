/*
 * main.cpp
 *
 *  Created on: Apr 22, 2012
 *      Author: mircea
 */

#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sock_helpers.hh"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <algorithm>
#include <memory>

#include "structs.hh"

using namespace std;




/*
 * Imparte stringul str dupa delimiters si pune rezultatul in tokens
 * Daca este parametrizata cu split_all=true va imprati pana la capat,
 * altfel va imparti str in maxim n cuvinte
 */

template < bool split_all=true, class ContainerT >
void tokenize (const std::string& str, ContainerT& tokens,
				const std::string& delimiters = " ", int n = 1, const bool trimEmpty = true)
{
	typedef typename ContainerT::value_type val_t;
	std::string::size_type pos, lastPos = 0;

	--n;
	while (split_all || n--)
	{
		pos = str.find_first_of(delimiters, lastPos);
		if(pos == std::string::npos)
		{
			pos = str.length();
			if(pos != lastPos || !trimEmpty)
				tokens.push_back( val_t(str.data() + lastPos, pos - lastPos ) );
			return;
		}
		else
			if(pos != lastPos || !trimEmpty)
				tokens.push_back( val_t(str.data() + lastPos, pos - lastPos ));

		lastPos = pos + 1;
	}
	tokens.push_back( val_t(str.data() + lastPos, str.data() + str.length()));
}





/*
 * Realizeaza o conexiune catre clientul ale carui informatii
 * sunt furnizate in info
 */

inline int connect (const client_info_cache& info) {
	sockaddr_in cli_addr;
	memset (&cli_addr, 0, sizeof(cli_addr));
	cli_addr.sin_family = AF_INET;
	cli_addr.sin_addr.s_addr = info.ip;
	cli_addr.sin_port = htons (info.port);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror ("ERROR opening socket for client connection");
		return sockfd;
	}

	int r = connect(sockfd, (sockaddr*) &cli_addr,sizeof(cli_addr));
    if (r < 0) {
        perror ("ERROR connecting to client");
        return r;
    }

    return sockfd;
}



/*
 * Clasa de client
 */

class Client
{
	// Numarul maxim de conexiuni acceptate
	static const int MAX_CONNECTIONS = 100;

	string name;		// numele clientului
	uint port;			// portul pe care asculta
	uint srv_ip;		// ipul serverului de care apartine
	uint srv_port;		// portul serverului de care apartine

	int sockfd;			// socketul pe care asculta
	int srvfd;			// socketul conectat la server

	fd_set read_fds;	// descriptorii socketilor de la care se asteapta date
	int fd_max;			// desctiptorul cu indicele cel mai mare

	sockaddr_in my_addr;	// adresa pe care se asculta
	sockaddr_in srv_addr;	// adresa serverului

	// fisierele share-uite
	set<string> shared_files;

	// (fd, fname) -> se asteapta fisierul fname de la conexiunea fd
	set<pair<int, string>> requested_files;


	/*
	 * Clasa abstracta reprezentand un transfer
	 * Are ca defivate TransferIn si TransferOut
	 *
	 * Se vor face operatii cu socketul al carui fd e primit de constructor
	 * Numele fisierului este dat in head
	 */
	class Transfer {
	public:
		int fd;			// descriptorul socketului prin care se va efectua transferul
		string fname;	// numele fisierului transferat
		int64_t remaining;	// cati octeti mai sunt de transferat

		// Starea intoarsa in urma apelului run()
		enum Status {
			Ok = 0,
			Done,
			SockFail,
			FileFail
		};

		Transfer () = default;
		Transfer (int _fd, file_head &head) :
			fd(_fd), fname(head.name), remaining(head.size) {}

		Transfer (Transfer &t) = delete;	// nu vreau sa poata fi copiat

		virtual Status run (char *buf, long int bufsz) = 0;
	};



	/*
	 * Primeste un fisier
	 */
	class TransferIn : public Transfer, ofstream {
	public:
		TransferIn (int _fd, file_head &head) :
			Transfer(_fd, head),
			ofstream(head.name, binary) {
			if (!good())
				cout << "ofstream not good\n";

			cout << "Receiving " << fname << " (" << remaining << " bytes)\n";
		}

		Status run (char *buf, long int bufsz) {
			if (remaining < bufsz) {
				bufsz = remaining;
			}

			int n = recv(fd, buf, bufsz, 0);
			if (n != bufsz)
				return SockFail;

			write(buf, n);
			if (!good())
				return FileFail;

			remaining -= n;
			if (remaining <= 0)
				return Done;

			return Ok;
		}
	};


	/*
	 * Trimite un fisier
	 */
	class TransferOut : public Transfer, ifstream {
	public:
		TransferOut (int _fd, file_head &head) :
			Transfer(_fd, head),
			ifstream(head.name, binary) {
			if (!good())
				cout << "ifstream not good\n";

			// Determin dimensiunea fisierului
			seekg (0, ios::end);
			remaining = tellg();
			seekg (0, ios::beg);

			// Trimit antetul fisierului
			head.size = remaining;
			try { send(fd, head); }
			catch (SockEx &e){
				cout << "Client didn't receive file head\n";
			}

			cout << "Sending " << fname << " (" << remaining << " bytes)\n";
		}

		Status run (char *buf, long int bufsz) {
			read(buf, bufsz);

			if (remaining < bufsz)
				bufsz = remaining;

			if (gcount() != bufsz)
				return FileFail;

			int n = send(fd, buf, bufsz, 0);
			if ( n < bufsz)
				return SockFail;

			remaining -= bufsz;
			if (!remaining)
				return Done;

			return Ok;
		}

	};

	/*
	 * Conexiune in asteptare, de la care nu s-a primit nimic
	 */
	struct WaitingConnection {
		int ip;
		int port;
		int fd;
		WaitingConnection (sockaddr_in &addr, int _fd) : fd(_fd) {
			ip = addr.sin_addr.s_addr;
			port = addr.sin_port;
		}
	};


	/*
	 * Baza de date cu clientii conectati
	 * La cererea ei se verifica actualitatea intreband serverul
	 * Daca nu este actuala serverul o va retransmite
	 */
	struct _clients {
		time_t timestamp;
		vector<client_ini> lst;
	} clients;


	/*
	 * Informatiile de conectare la clienti
	 * Cand se primesc informatii legate de un client,
	 * acestea sunt stocate, pana cand serverul anunta
	 * ca acel user s-a deconectat
	 */
	map<string, client_info_cache> client_cache;

	/*
	 * Containerele stl pot stoca doar obiecte copiabile, insa
	 * clasele de io din stl nu se pot copia, prin urmare este
	 * necesar sa stochez pointeri la clasele de transfer, care
	 * mostenesc de la fstream
	 */
	typedef shared_ptr<TransferIn> TrInPtr;
	typedef shared_ptr<TransferOut> TrOutPtr;

	// Lista de transferuri prin care se primesc fisiere
	list<TrInPtr> tq_in;

	// Lista de transferuri prin care se trimit fisiere
	list<TrOutPtr> tq_out;

	// Lista de conexiuni in asteptare
	list<WaitingConnection> conn_q;

	// Coada de raspunsuri ce se asteapta de la server
	queue<SrvReply> srv_reply_q;

	// Coada de mesaje ce trebuie livrate clientilor
	queue<CliMsg> cli_msg_q;

	// Structura generica in care se vor primi structuri de la server/clienti
	// Trebuie sa aiba fix chunksz octeti
	struct _recv_buf {
		uint8_t type;
		char	data [chunksz - 1];
	} recv_buf;

	static_assert ( sizeof(_recv_buf) == chunksz, "Wrong size!" );

	char data_buf [1024];	// buferul de transfer date

	// Tipurile de comenzi acceptate de la tastatura
	enum Command {
		QUIT,
		LISTC,
		INFOC,
		MSG,
		SHAREF,
		UNSHAREF,
		GETSHARE,
		GETF
	};


	// Executa o comanda primita de la tastatura
	bool run_command (string &line) {
		// translateaza o comanda in format text intr-un indice
		static const map<string, Command> com {
				{ "quit",			QUIT },
				{ "listclients",	LISTC },
				{ "infoclient",		INFOC },
				{ "message",		MSG },
				{ "sharefile",		SHAREF },
				{ "unsharefile",	UNSHAREF },
				{ "getshare",		GETSHARE },
				{ "getfile",		GETF }
		};

		// Ignor comenzile goale
		if (line.empty() || line=="\n")
			return false;

		// Sparg linia in 3 bucati
		vector<string> words;
		tokenize <false> (line, words, " ", 3);

		// Identific comanda
		auto c = com.find(words[0]);
		if (c == com.end()) {
			cout << "Unknown command: " << words[0] << endl;
			return false;
		}

		request req;

		try {
			switch (c->second) {
			case QUIT:
				// vreau sa ies
				return true;

			// listclients
			case LISTC: {
				if (words.size() != 1)
					cout << words[0] << " takes 1 argument\n";
				else {
					db_request db_req;
					db_req.timestamp = clients.timestamp;
					send(srvfd, db_req);

					// Astept raspuns de la server cu lista de clienti
					srv_reply_q.push(USR_DB);
				}
				break;
			}


			// infoclient
			case INFOC: {
				if (words.size() != 2)
					cout << words[0] << " takes 2 arguments\n";
				else if (words[1] != name) {
					auto info = client_cache.find(words[1]);
					if (info != client_cache.end()) {
						// Daca e in cache iau direct de acolo
						cout << info->first << ": " << info->second << endl;
					}
					else {
						// Altfel cer de la server
						req.type = GET_INFO;
						words[1].copy(req.data, sizeof(req.data), 0);
						send(srvfd, req);
						srv_reply_q.push(USR_INFO);
					}
				}
				break;
			}


			// message
			case MSG: {
				if (words.size() < 3 )
					cout << words[0] << " takes more than 2 arguments\n";
				else if (words[1] != name) {
					send_txt_msg (words[1], words[2]);
				}
				break;
			}


			// sharefile
			case SHAREF: {
				if (words.size() != 2)
					cout << words[0] << " takes 1 argument\n";
				else if (words[1] != name) {
					// verific daca fisierul este accesibil si se poate citi din el
					if (access(words[1].c_str(), F_OK | R_OK) != 0) {
						cout << "Cannot access or not file\n";
						break;
					}
					// Instiintez serverul
					req.type = ADD_FILE;
					words[1].copy(req.data, sizeof(req.data), 0);
					shared_files.insert(words[1]);
					send(srvfd, req);
				}
				break;
			}


			// unsharefile
			case UNSHAREF: {
				// verific daca chiar am shareuit fisierul
				if (!shared_files.count(words[1]))
					break;

				// Instiintez serverul
				req.type = REM_FILE;
				words[1].copy(req.data, sizeof(req.data), 0);
				send(srvfd, req);
				shared_files.erase(words[1]);
				break;
			}


			// getshare
			case GETSHARE: {
				if (words.size() == 1)
					cout << "Currently sharing " << shared_files.size() << " files\n" << shared_files << endl;

				else if (words.size() != 2)
					cout << words[0] << " takes none or one argument\n";
				else if (words[1] != name) {
					req.type = LIST_FILES;
					words[1].copy(req.data, sizeof(req.data), 0);
					send(srvfd, req);
					srv_reply_q.push(FILE_LST);
				}
				break;
			}

			// getfile
			case GETF: {
				if (words.size() != 3)
					cout << words[0] << "takes 2 arguments\n";
				else if (words[1] != name) {
					// verific daca pot salva in directorul curent
					if (access(".", W_OK | R_OK) != 0) {
						cout << "Cannot save files in cwd\n";
						break;
					}
					request_file (words[1], words[2]);
				}
				break;
			}
			}
		}
		catch (SockEx &e) {
			cerr << e << " socket error\n";
		}

		return false;
	}






	/*
	 * Cere de la server informatiile de conectare catre un client
	 * Intoarce true daca totul a mers bine
	 */
	bool request_cli_info (string &name) {
		request req;
		req.type = GET_INFO;
		name.copy(req.data, sizeof(req.data), 0);
		try { send(srvfd, req); }
		catch (SockEx &e) {
			cerr << "Server did not respond\n";
			return false;
		}
		srv_reply_q.push(USR_INFO_INT);
		return true;
	}

	/*
	 * Trimite mesajul txt la userul dest_name
	 * Daca userul nu este in cacheul local si
	 * retry==true se va trimite o cerere catre server
	 */
	void send_txt_msg (string& dest_name, string &txt, bool retry = true) {
		if (txt.length() > 1024) {
			cout << "Max msg length is 1024 chars\n";
			return;
		}

		auto dest = client_cache.find(dest_name);

		if ( dest != client_cache.end()) {

			// Am informatiile despre user
			// pot sa trimit mesajul

			txt = name + ": " + txt;
			int fd = connect(dest->second);
			if (fd <= 0) {
				cout << "Could not connect to " << dest->first << endl;
				return;
			}

			txt_msg msg;
			msg.len = txt.length();
			msg.type = TXT_MSG;
			msg.status = 0;
			txt.copy(msg.data, sizeof(msg.data), 0);
			try {
				send(fd, msg);

				// Daca mesajul nu incaput integral in msg.data
				// trimit continuarea printr-un alt apel la send
				if (txt.length() > sizeof(msg.data)) {
					txt.copy(data_buf, sizeof(data_buf), sizeof(msg.data));
					send(fd, data_buf, txt.length() - sizeof(msg.data));

					data_buf[txt.length() - sizeof(msg.data)] = '\0';
				}
			}
			catch (SockEx &e) {
				cout << "Client did not respond\n\n";
				return;
			}
		}
		else {
			if (retry) {
				// cer informatiile de conectare
				if (request_cli_info(dest_name))
					cli_msg_q.push(CliMsg(TXT_MSG, dest_name, txt));
			}
			else
				cout << "User " << dest_name << " did not respond\n\n";
		}
	}






	/*
	 *  Cere fisierul fname de la client_name
	 *  Daca nu are informatii despre user, le cere de la server
	 */
	void request_file (string& client_name, string &fname, bool retry = true) {

		auto client = client_cache.find(client_name);

		if (client != client_cache.end()) {
			int fd = connect(client->second);
			if (fd <= 0) {
				cout << "Could not connect to " << client->first << endl;
				return;
			}

			file_head req(fname, 0);
			try { send(fd, req); }
			catch (SockEx &e) {
				cout << "Client did not respond\n\n";
				return;
			}

			// Voi astepta raspuns pe fd
			FD_SET(fd, &read_fds);
			if (fd > fd_max)
				fd_max = fd;

			requested_files.insert(make_pair(fd, fname));
		}
		else {
			if (retry) {
				if (request_cli_info(client_name))
					cli_msg_q.push(CliMsg(FILE_HEAD, client_name, fname));
			}
			else
				cout << "User " << client_name << " did not respond\n\n";
		}
	}




	/*
	 * Proceseaza un raspuns primit de la server
	 */
	void process_srv_reply()
	{
		uint8_t reply_t = recv_buf.type;
		char *lst = nullptr;
		try {
			// Daca nu ma asteptam sa primesc un raspuns de la server,
			// sau mesajul primit nu corespunde cu ce asteptam
			if (srv_reply_q.empty() || srv_reply_q.front() != reply_t) {

				if (reply_t == SRV_NOTIF) {
					// Am primit notificare ca a iesit un client
					srv_notification& notif = reinterpret_cast<srv_notification&>(recv_buf);
					cout << "\n" << notif.data << " has left\n\n"<< endl;
					client_cache.erase(notif.data);
				}

				else if (reply_t == USR_INFO ) {
					// Am primit informatii despre un client
					client_info& cli_info = reinterpret_cast<client_info&>(recv_buf);

					// Daca ip==0 clientul despre care s-a intrebat nu era inregistrat
					if (cli_info.ip)
						client_cache[cli_info.name] = cli_info;
					else {
						srv_reply_q.pop();
						cli_msg_q.pop();
						cout << "User is not online\n\n";
						return;
					}


					if (!srv_reply_q.empty() && srv_reply_q.front() == USR_INFO_INT) {

						// Aveam de trimis un mesaj sau de cerut un fisier
						srv_reply_q.pop();
						auto msg_waiting = cli_msg_q.front();

						switch (msg_waiting.type) {

						case TXT_MSG: {
							send_txt_msg (msg_waiting.dest, msg_waiting.data, false);
							break;
						}

						case FILE_HEAD: {
							request_file (msg_waiting.dest, msg_waiting.data, false);
							break;
						}
						}

						cli_msg_q.pop();
					}
					else
						cerr << "Didn't request user info\n\n";
				}
			}
			else {
				// Mesajul de la server corespunde cu cel asteptat

				srv_reply_q.pop();

				switch (reply_t)
				{

				// lista de clienti
				case USR_DB: {
					db_head& db = reinterpret_cast<db_head&>(recv_buf);
					if (db.timestamp) {
						clients.timestamp = db.timestamp;
						if (db.clients_n)
							recv(srvfd, clients.lst, db.clients_n);
						else
							clients.lst.clear();
					}

					cout << "Online users [" << clients.lst.size()+1 <<  "]:\n";
					cout << "\t*" << name << endl;
					for (auto i=clients.lst.begin(); i!=clients.lst.end(); ++i)
						cout << (*i);
					cout << endl;
					break;
				}

				// lista de fisiere shareuite de un client
				case FILE_LST: {
					client_files& files_h = reinterpret_cast<client_files&>(recv_buf);
					cout << "User " << files_h.name;
					if (files_h.len > 0) {
						lst = new char[files_h.len];
						recv(srvfd, lst, files_h.len);
						cout << " is sharing:\n" << lst << endl;
						delete [] lst;
						lst = nullptr;
					}
					else
						if (files_h.len == 0)
							cout << " isn't sharing anything\n\n";
						else
							cout << "is not online\n\n";
					break;
				}

				// informatii despre un client
				case USR_INFO: {
					client_info& cli_info = reinterpret_cast<client_info&>(recv_buf);
					if (cli_info.ip) {
						client_cache[cli_info.name] = cli_info;
						cout << cli_info.name << ": " << client_cache[cli_info.name] << endl;
					}
					else
						cout << "User is not online\n\n";
					break;
				}
				default: {
					cerr << "Unknown reply code from server !!!\n";
					break;
				}
				}
			}
		}
		catch (SockEx &e) {
			cerr << e << " socket error while processing req from srv\n";
			delete [] lst;
		}
	}




	/*
	 * Proceseaza un mesaj venit de la un client pe o conexiune noua
	 */
	void process_client_msg (WaitingConnection &c) {

		switch (recv_buf.type) {

		// Am primit un mesaj text
		case TXT_MSG: {
			txt_msg& msg = reinterpret_cast<txt_msg&>(recv_buf);
			const int bufsz = sizeof(txt_msg::data);

			// Daca textul a incaput in structura
			if (msg.len < bufsz) {
				msg.data[msg.len] = '\0';
				cout << msg.data << endl;
			}
			else if (msg.len == bufsz) {
				char e = msg.data[bufsz-1];
				msg.data[bufsz-1] = '\0';
				cout << msg.data << e << endl;
			}
			else {
				// Textul a fost mai lung, se asteapta primirea unei continuari
				data_buf[0] = msg.data[bufsz-1];
				msg.data[bufsz-1] = '\0';
				try { recv(c.fd, data_buf+1, msg.len - bufsz); }
				catch (SockEx &e) {
					close(c.fd, &read_fds);
					return;
				}
				data_buf[msg.len - bufsz + 1] = '\0';
				cout << msg.data << data_buf << endl;
			}
			close(c.fd, &read_fds);
			break;
		}

		// Am primit o cerere de fisier
		case FILE_HEAD: {
			file_head& fhead = reinterpret_cast<file_head&>(recv_buf);

			cout << "got request for " << fhead.name << endl;

			// Daca sunt dispus sa trimit fisierul
			if (shared_files.count(fhead.name)) {
				// Adaug fisierul la coada de fisiere de transmis
				tq_out.push_back(TrOutPtr(new TransferOut(c.fd, fhead)));
			}
			else {
				// Nu vreau sa il dau
				memset(&fhead, 0, sizeof(fhead));
				fhead.type = FILE_HEAD;
				send(c.fd, fhead);
				close(c.fd, &read_fds);
			}
			break;
		}
		}
	}



public:
	/*
	 * Metode publice
	 */


	Client (const char *_name) : name(_name) {
		if (name.length() > 40)
			name.substr(0, 40);
		cout << "\n\nHello, " << name << endl;
		clients.timestamp = 0;
	}




	// Clientul va asculta pe portul _port
	bool plug (uint _port){
		port = _port;
		memset (&my_addr, 0, sizeof(my_addr));
		my_addr.sin_family = AF_INET;
		my_addr.sin_addr.s_addr = INADDR_ANY;
		my_addr.sin_port = htons (port);

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) {
			perror ("ERROR opening listen socket");
			return false;
		}

		if ( ::bind( sockfd, (struct sockaddr *) &my_addr, sizeof (struct sockaddr) ) < 0 ) {
			 perror ("ERROR on binding");
			 return false;
		}

		// Daca nu s-a specificat un port anume trebuie sa vad ce port mi s-a assignat
		if (port==0) {
			sockaddr_in sin;
			socklen_t len = sizeof(sin);
			if (getsockname(sockfd, (struct sockaddr *)&sin, &len) == -1) {
				perror("ERROR on getsockname");
				return false;
			}
			port = ntohs(sin.sin_port);
		}

		if ( listen(sockfd, MAX_CONNECTIONS) < 0 ) {
			perror ("ERROR on listen");
			return false;
		}

		fd_max = sockfd;

		cout << "Listening on port " << port << endl;

		return true;
	}




	// Conecteaza clientul la server
	bool connect_to_srv (uint _srv_ip, uint _srv_port)
	{
		srv_ip = _srv_ip;
		srv_port = _srv_port;

		srvfd = socket(AF_INET, SOCK_STREAM, 0);
		if (srvfd < 0) {
			perror ("ERROR opening socket for server link");
			return false;
		}

		memset (&srv_addr, 0, sizeof(srv_addr));
		srv_addr.sin_family = AF_INET;
		srv_addr.sin_addr.s_addr = srv_ip;
		srv_addr.sin_port = htons (srv_port);

	    if (connect(srvfd, (sockaddr*) &srv_addr,sizeof(srv_addr)) < 0) {
	        perror ("ERROR connecting to server");
	        return false;
	    }

	    /*
	     * Handshake
	     */
	    MsgType res = OK;
	    try {
	    	send(srvfd, client_id(name, port));
	    	recv(srvfd, res);
	    }
	    catch (SockEx &e) {
	    	cout << "Unsuccessful handshake " << e << endl;
	    	return false;
	    }

    	if (res != OK) {
    		if (res == NAME_USED)
    			cout << "Name " << name << " already used\n";
    		else
    			cout << "Server refused connection\n";
    		return false;
    	}


	    if (srvfd > fd_max)
	    	fd_max = srvfd;

		FD_ZERO (&read_fds);
		FD_SET  (sockfd, &read_fds);	// Ascult pe portul deschis local,
		FD_SET  (srvfd, &read_fds);		// pe socketul legat la server
		FD_SET  (0, &read_fds);			// si pe stdin

		cout << "Connected to server\n";

		return true;
	}




	void kill () {
		cout << "\nGoodbye!\n\n";
		close(srvfd);
		close(sockfd);

		for (auto t = tq_in.begin(); t != tq_in.end(); ) {
			cout << "Killing inbound transfer of " << (*t)->fname << endl;
			close((*t)->fd);
			t = tq_in.erase(t);
		}

		for (auto t = tq_out.begin(); t != tq_out.end(); ) {
			cout << "Killing outbound transfer of " << (*t)->fname << endl;
			close((*t)->fd);
			t = tq_out.erase(t);
		}
	}





	/*
	 * Executa bucla principala
	 */
	bool work ()
	{
		sockaddr_in cli_addr;
		fd_set waiting_fds;

	    request req;

	    int waiting_count = 0;

	    string command;
	    timeval nowait = {0, 0};

	    // Trimit un singur pachet de 1024, apoi verific de evenimente
	    auto to_it = tq_out.begin();

	    cout << "Permitted commands: \n\tquit, message, listclients,\n\tsharefile, unsharefile,\n\tinfoclient, getfile, getshare\n\n";

	    for (;;)
		{
			waiting_fds = read_fds;

			// Daca am fisiere de transmis nu las select-ul sa blocheze
			waiting_count = select (fd_max+1, &waiting_fds, NULL, NULL, tq_out.empty() ? NULL : &nowait);

			if (waiting_count < 0) {
				perror ("ERROR on select");
				return false;
			}

			// Comanda de la stdin
			if ( FD_ISSET(0, &waiting_fds) ) {
				--waiting_count;
				getline(cin, command);
				if (run_command(command))
					return true;
			}

			// Mesaj de la server
			if ( FD_ISSET(srvfd, &waiting_fds) ) {
				--waiting_count;
				try { recv(srvfd, recv_buf); }
				catch (SockEx &e) {
					cerr << "\nServer has disconnected\n\n";
					return false;
				}
				process_srv_reply();
			}

			// Conexiune noua de la un alt client
			if ( FD_ISSET(sockfd, &waiting_fds) )
			{
				--waiting_count;

				uint addr_len = sizeof(cli_addr);
				int newsockfd = accept(sockfd, (sockaddr*) &cli_addr, &addr_len);
				if (newsockfd < 0) {
					perror("ERROR in accept");
				}
				else {
					//adaug noul socket intors de accept() la multimea descriptorilor de citire
					FD_SET(newsockfd, &read_fds);
					if (newsockfd > fd_max)
						fd_max = newsockfd;

					cout << "New connection (fd = " << newsockfd << ") from " << cli_addr;
					// pun conexiunea la coada
					conn_q.push_back( WaitingConnection(cli_addr, newsockfd) );
				}
			}

			// Verific conexiunile noi, pe care nu am primit nimic
			for (auto i = conn_q.begin(); waiting_count && i != conn_q.end();) {
				if ( FD_ISSET(i->fd, &waiting_fds) ) {
					--waiting_count;
					try {
						recv(i->fd, recv_buf);
						process_client_msg(*i);
					}
					catch (SockEx &e) {
						close(i->fd, &read_fds);
					}
					i = conn_q.erase(i);
				}
				else
					++i;
			}

			// Verific transferurile de intrare
			for (auto t = tq_in.begin(); waiting_count && t != tq_in.end(); ) {
				if ( FD_ISSET((*t)->fd, &waiting_fds) ) {
					--waiting_count;

					switch ((*t)->run(data_buf, sizeof(data_buf))) {
					case Transfer::Ok: {
						++t;
						break;
					}

					case Transfer::Done: {
						cout << "Successfully received file " << (*t)->fname << endl;
						close((*t)->fd, &read_fds);
						t = tq_in.erase(t);
						break;
					}

					default: {
						cout << "Transfer in of file " << (*t)->fname << " failed\n";
						close((*t)->fd, &read_fds);
						t = tq_in.erase(t);
						break;
					}
					}

				}
				else
					++t;
			}

			// Verific conexiunile pe care ar trebui sa primesc antet de fisier
			for (auto f = requested_files.begin(); waiting_count && f != requested_files.end();) {
				if ( FD_ISSET(f->first, &waiting_fds) ) {
					--waiting_count;
					try {
						recv(f->first, recv_buf);
						file_head &fhead = reinterpret_cast<file_head&>(recv_buf);

						if (fhead.name[0] == 0) {
							cout << "User did not share " << f->second << endl;
							close(f->first, &read_fds);
							continue;
						}

						if (fhead.name != f->second) {
							cout << "Expected file " << f->second << ", got " << fhead.name << endl;
							close(f->first, &read_fds);
							continue;
						}

						tq_in.push_back(TrInPtr(new TransferIn(f->first, fhead)));
					}
					catch (SockEx &e) {
						cout << "Error receiving file head for " << f->second << endl;
					}
					f = requested_files.erase(f);
				}
				else
					++f;
			}

			// Trimit un singur chunk din fisierul din coada
			if (!tq_out.empty()) {
				if (to_it == tq_out.end())
					// O iau de la capat
					to_it = tq_out.begin();

				// Efectuez transferul propriu-zis
				switch ((*to_it)->run(data_buf, sizeof(data_buf))) {
				case Transfer::Ok: {
					++to_it;
					break;
				}

				case Transfer::Done: {
					cout << "Successfully sent file " << (*to_it)->fname << endl;
					close((*to_it)->fd, &read_fds);
					to_it = tq_out.erase(to_it);
					break;
				}

				/*
				case Transfer::FileFail: {
					cout << "filefail\n";
				}

				case Transfer::SockFail: {
					cout << "sockfail\n";
				}
				*/

				default: {
					cout << "Transfer out of file " << (*to_it)->fname << " failed\n";
					close((*to_it)->fd, &read_fds);
					to_it = tq_out.erase(to_it);
					break;
				}
				}

			}

		}

	    return true;
	}

};



//#define DEBUG

int main (int argc, char *argv[])
{
	int port = 0;
	int srv_ip = 0;
	int srv_port = 7777;

#ifdef DEBUG
	if (inet_pton(AF_INET, "127.0.0.1", &srv_ip) != 1) {
		cerr << "Invalid ip\n";
		return 1;
	}


	string sname;
	if (argc==2)
		sname = argv[1];
	else
		sname = "007";
	const char *name = sname.c_str();

#else
    if (argc < 4) {
        cout << "Usage: " << argv[0] << " nume ip_srv port_srv <local_port>\n";
        return 1;
    }

    const char *name = argv[1];

	if (inet_pton(AF_INET, argv[2], &srv_ip) != 1) {
		cerr << "Invalid ip\n";
		return 1;
	}

    srv_port = atoi (argv[3]);
    if (srv_port == 0) {
    	cout << "Invalid server port\n";
    	return 1;
    }

    if (argc >= 5) {
    	port = atoi (argv[4]);
    }
#endif

    Client cli(name);
    if (cli.plug(port) && cli.connect_to_srv(srv_ip, srv_port)) {
    	cli.work();
    	cli.kill();
    }
    return 0;
}
