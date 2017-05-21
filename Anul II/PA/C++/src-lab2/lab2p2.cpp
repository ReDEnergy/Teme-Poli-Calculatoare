#include <iostream>
#include <vector>

#define CEIL(a, b) ((a) / (b) + ((a) % (b) ? 1 : 0))

/* Modelam o fractie ca fiind o pereche (numarator, numitor). */
typedef std::pair<long long, long long> Fractie;

std::vector<Fractie> descompunere_fractii_egiptene(long long x, long long y)
{
  std::vector<Fractie> v;

  /* Cat timp nu am ajuns cu descompunerea la 0, continuam sa alegem cea mai
   * mare fractie de forma 1/n care este strict mai mica decat x/y, o adaugam la
   * suma si incecam sa descompunem mai departe diferenta (x/y - 1/n).
   *
   * Se observa ca acea fractie este mereu 1/(CEIL(y/x)).
   */
  do {
    v.push_back(Fractie(1, CEIL(y, x)));
    long long newx = (x - y % x) % x;
    long long newy = y * CEIL(y, x);
    x = newx;
    y = newy;
  } while (x != 0);

  return v;
}

int main()
{
  /* Citim x si y. */
  long long x, y;
  std::cin >> x >> y;

  /* Calculam descompunerea in fractii egiptene. */
  std::vector<Fractie> v;
  v = descompunere_fractii_egiptene(x, y);

  /* Si o afisam. */
  std::cout << x << "/" << y << " = ";
  for (unsigned int i = 0; i < v.size(); ++i) {
    std::cout << (i > 0 ? " + " : "") << v[i].first << "/" << v[i].second;
  }
  std::cout << std::endl;

  return 0;
}

