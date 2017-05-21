#include <iostream>
#include <vector>
#include <algorithm>

#include "VectorIO.h"
#include "PairIO.h"

/* Vom defini un task sub forma unei perechi (start, finish). */
typedef std::pair<int, int> Task;

bool compare_finish_time(const Task& left, const Task& right)
{
  return left.second < right.second;
}

int get_max(std::vector<Task>& v)
{
  /* Vom sorta vectorul crescator dupa timpul de finish. */
  std::sort(v.begin(), v.end(), compare_finish_time);

  /* Scanam de la stanga la dreapta (in ordinea crescatoare a timpului de
   * finish si alegem in mod greedy acele task-uri care nu se suprapun cu
   * ultimul task ales.
   *
   * Aceasta solutie ne conduce la optim pentru ca nu ne intereseaza in nici un
   * mod durata totala de timp cat robotul este ocupat, ci doar numarul de
   * task-uri indeplinite. Cu alte cuvinte, daca avem de ales intre doua
   * task-uri care se suprapun (partial sau total), il vom alege intotdeauna pe
   * cel care se termina mai devreme deoarece ne lasa mai mult loc de
   * planificare pentru viitor (si deoarece oricare dintre cele doua task-uri
   * este la fel de bun).
   */
  int return_value = 1;
  int last_completed = v[0].second;
  for (unsigned int i = 1; i < v.size(); ++i) {
    if (v[i].first >= last_completed) {
      return_value++;
      last_completed = v[i].second;
    }
  }

  return return_value;
}

int main()
{
  /* Declaram si citim vectorul de task-uri. */
  std::vector<Task> v;
  std::cin >> v;

  /* Afisam numarul maxim de task-uri pe care le poate indeplini robotul. */
  std::cout << "Nr maxim de task-uri care nu se suprapun: " << get_max(v)
      << std::endl;

  return 0;
}

