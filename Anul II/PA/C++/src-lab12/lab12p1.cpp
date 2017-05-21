#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>

#include "VectorIO.h"
#include "Point.h"

#define SQR(a) ((a) * (a))

using namespace std;

static const int MaxIterations = 1000;

/* Selecteaza aleator k puncte din vectorul primit.
 * Se urmareste ca fiecare punct sa fie selectat cu probabilitatea k / n. */
void init_centroids(const std::vector<Point>& points, int k,
                    std::vector<Point>& centroids) {
  /* TODO */

	vector<Point> shuffle;

	for (unsigned int i=0; i<points.size(); i++)
		shuffle.push_back(points[i]);

	random_shuffle(shuffle.begin(), shuffle.end());

	for (unsigned int i=0; i<k; i++)
		centroids.push_back(shuffle[i]);


}

int distance_points(Point A, Point B) {
	return sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
}


/* Calculeaza cel mai apropiat centroid pentru punct. */
int compute_cluster(const std::vector<Point>& centroids,
                    const Point& point) {
  int min_index = 0;

  /* TODO: In min_index trebuie sa puneti indicele centroidului din vectorul
   * "centroids" care este cel mai apropiat de "point". Daca sunt mai multi
   * centroizi la aceeasi distanta de point, puteti intoarce indexul oricaruia
   * dintre ei. */
	int min_dist = 100000;
	int dist;

	for (unsigned int i=0; i<centroids.size(); i++)
	{
		dist = distance_points(point, centroids[i]);
		if (dist < min_dist) {
			min_dist = dist;
			min_index = i;
		}
	}

  return min_index;
}

void kmeansplusplus(std::vector<Point>& points, unsigned int k)
{
  /* Initializare centroizi. Pentru comoditate, vom folosi tot structura
   * Point, dar vom lasa campul cluster necompletat. */
  std::vector<Point> centroids;
  init_centroids(points, k, centroids);


  for (int step = 0; step < MaxIterations; ++step) {
    /* Vom folosi o variabila booleana pentru a detecta pasul de la care nu se
     * mai efectueaza schimbari in componenta clusterelor. */
    bool done = true;

    /* TODO: Asignati toate punctele la cate un centroid. */

    for (unsigned int i=0; i<points.size(); i++) {
    	int cluster = compute_cluster(centroids, points[i]);
    	if (cluster != points[i].cluster)
		{
			points[i].cluster = cluster;
			done = false;
		}
    }

	if (done == true) {
		cout<<" Steps : " << step << endl;
		break;
	}

	for (unsigned int i=0; i<centroids.size(); i++)
		centroids[i].x = centroids[i].y  = centroids[i].cluster = 0;

    /* TODO: Recalculati centroizii. */
	for (unsigned int i=0; i<points.size(); i++) {
		centroids[points[i].cluster].x += points[i].x;
		centroids[points[i].cluster].y += points[i].y;
		centroids[points[i].cluster].cluster++;
	}

	for (unsigned int i=0; i<centroids.size(); i++) {
		centroids[i].x /= centroids[i].cluster;
		centroids[i].y /= centroids[i].cluster;
	}

	cout << endl << centroids <<endl;
  }
}

int main()
{
  /* Deschidem un fisier si citim din el setul de puncte. */
  std::ifstream in("src-lab12/Kmeans.in");
  int k;
  std::vector<Point> points;
  in >> k >> points;

  std::cout << "Points are: " << std::endl << points << std::endl;

  /* Initializare generator numere aleatoare. */
  srand(time(NULL));

  /* Aplicam KMeans++ pentru a completa clusterele. */
  kmeansplusplus(points, k);

  /* Afisam punctele si clusterele asignate dupa fiecare pas. */
  std::cout << points << std::endl;

  return 0;
}
