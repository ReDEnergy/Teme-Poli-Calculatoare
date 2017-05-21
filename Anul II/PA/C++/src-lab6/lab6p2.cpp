#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <queue>
#include <string>
#include <cstring>
#include <vector>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

enum Color {
  WHITE,
  GRAY,
  BLACK
};

struct Node {
  Color color;
  unsigned int finishTime;
  std::string subjectName;
  unsigned int inDegree;

  std::vector<Node*> neigh;

  Node(std::string subjectName) : subjectName(subjectName), inDegree(0) { }
};

/* Functie care compara doua noduri in functie de timpul de finish. */
bool compareNodePointersByFinishTime(const Node* left, const Node* right)
{
  return left->finishTime < right->finishTime;
}

typedef std::vector<Node*> Graph;

std::map<std::string, unsigned int> subjectToInt;

void dfs(Node* node, unsigned int& time)
{
  /* TODO: Puteti folosi aceasta functie pentru a implementa o parcurgere in
   * adancime (in cadrul careia sa completati si timpii de finish). */
	int i;
	node->Node = Color.GRAY;
	for (i=0; i<node->neigh.size(); i++) {
		if (node->color == Color.WHITE) {
			dfs(node->neigh[i], time);
		}
	}

	node->finishTime = time++;
	node->color = Color.BLACK;
}

/**
DFS(G) {
    V = noduri(G)
    foreach (u ∈ V) {
        // initializare structura date
        c(u) = alb;
        p(u)=null;
    }
    timp = 0; // retine distanta de la radacina pana la nodul curent
    foreach (u ∈ V)
        if (c(u) = alb) explorare(u); // explorez nodul
}

explorare(u) {
    d(u) = timp++; // timpul de descoperire al nodului u
    c(u) = gri; // nod in curs de explorare
    foreach (v ∈ succes(u)) // incerc sa prelucrez vecinii
        if (c(v) = alb) { // daca nu au fost prelucrati deja
            p(v) = u;
            explorare(v);
        }
    c(u) = negru; // am terminat de prelucrat nodul curent
    f(u) = timp++; // timpul de finalizare al nodului u
}
**/


void topologicalSorting(Graph& graph)
{
  /* TODO: Sortati vectorul primit ca parametru astfel incat sa contina
   * pointerii la noduri in ordine topologica. */
	int i =0;
	for (i; i<graph.size(); i++) {
		graph[i].color = Color.WHITE;
	}

	for (i; i<graph.size(); i++)
		if (graph[i].color == Color.WHITE)
			dfs(&graph[i]);

	sort(graph.begin(), graph.end(),  compareNodePointersByFinishTime);
}

void planYears(Graph& graph,
               std::vector<std::vector<std::string> >& yearlyPlanning)
{
  /* TODO: Folosind algoritmul lui Kahn, impartiti materiile la care fac
   * referire nodurile din vectorul graph pe ani, astfel incat:
   *
   * (a) Daca o materie A depinde de o materie B, atunci anul in care se face
   * materia A trebuie sa fie _strict_ mai mare decat anul in care se face
   * materia B.
   *
   * (b) Intr-un an nu se pot face mai mult de 5 materii.
   *
   * (c) Fiecare matrie trebuie planificata.
   *
   * In varaibila yearly planning veti pune in ordine, vectori de stringuri care
   * contin materiile ce vor fi parcurse in anii consecutivi de studiu: 1, 2, 3,
   * etc.
   */




}

int main()
{
  /* Declaram un graf. */
  Graph graph;

  /* Citim un graf din fisierul de intrare. */
  std::ifstream in("src-lab6/Materii.txt");
  unsigned int edgeCount;
  in >> edgeCount;

  std::string subjectA, separator, subjectB;
  for (unsigned int i = 0; i < edgeCount; ++i) {
    in >> subjectA >> separator >> subjectB;

    /* Daca n-am mai vazut niciodata subjectA, atunci creaza-i un cod unic. */
    if (subjectToInt.count(subjectA) == 0) {
      unsigned int code = subjectToInt.size();
      subjectToInt[subjectA] = code;
      graph.push_back(new Node(subjectA));
    }

    /* La fel facem si cu subjectB. */
    if (subjectToInt.count(subjectB) == 0) {
      unsigned int code = subjectToInt.size();
      subjectToInt[subjectB] = code;
      graph.push_back(new Node(subjectB));
    }

    /* Adaugam subjectB ca vecin al lui subjectA. */
    graph[subjectToInt[subjectA]]->neigh.push_back(
        graph[subjectToInt[subjectB]]);
    graph[subjectToInt[subjectB]]->inDegree++;
  }

  topologicalSorting(graph);

  std::cout << "O posibila sortare topologica este: " << std::endl;
  for (unsigned int i = 0; i < graph.size(); ++i) {
    std::cout << "\t" << graph[i]->subjectName << std::endl;
  }

  /* Facem impartirea pe ani. */
  std::vector<std::vector<std::string> > yearlyPlanning;
  planYears(graph, yearlyPlanning);

  std::cout << std::endl << "Iar o impartire pe ani ar putea fi urmatoarea: "
      << std::endl;
  for (unsigned int i = 0; i < yearlyPlanning.size(); ++i) {
    std::cout << "Anul " << (i + 1) << std::endl;
    std::vector<std::string>& subjects = yearlyPlanning[i];
    for (unsigned int j = 0; j < subjects.size(); ++j) {
      std::cout << "\t" << subjects[j] << std::endl;
    }
  }

  return 0;
}

