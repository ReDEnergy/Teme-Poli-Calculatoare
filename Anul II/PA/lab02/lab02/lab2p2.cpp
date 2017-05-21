#include <iostream>
#include <vector>

#define CEIL(a, b) ((a) / (b) + ((a) % (b) ? 1 : 0))

/* Modelam o fractie ca fiind o pereche (numarator, numitor). */
typedef std::pair<long long, long long> Fractie;


long long gcd(long long a, long long b) {
	if(b)
		return gcd(b, a%b);
	else 
		return a;
}

std::vector<Fractie> descompunere_fractii_egiptene(long long x, long long y)
{
  std::vector<Fractie> v;

	while (x!=1) {
		v.push_back( Fractie (1, CEIL(y,x)) );

		long long newx = x - (y % x);
		long long newy = y * CEIL (y, x);
		x = newx;
		y = newy;

		long long b = gcd(x,y);
		x /=b;
		y /=b;

	//	std::cout << x << " " << y << std::endl;

	}
		v.push_back( Fractie (1, CEIL(y,x)) );
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

  system("PAUSE");
  return 0;
}

