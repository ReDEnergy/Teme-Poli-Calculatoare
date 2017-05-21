#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <math.h>


using namespace std;

FILE *F, *out;
int N, dim;

struct Point {
	float x;
	float y;
	float z;
	int C;
	float dist;

	Point() {
		C = 0;
		dist = 9999;
		x = y = z = 0;
	}

	float distTo(Point p) {
		return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
	}
};

int main() {

	srand (time(NULL));

	fopen_s(&F, "TwoDiamonds.lrn", "r");
	fopen_s(&out, "out.txt", "w");

	fscanf(F, "%d", &N);
	fscanf(F, "%d", &dim);

	int skip, cls = 2;
	Point *K = new Point[N];
	Point C[2];

	for (int i=0; i<N; i++) {
		if (dim == 3) {
			fscanf(F, "%d %f %f", &skip, &K[i].x, &K[i].y);
		}
	}

	bool change = true; 

	int r = rand() % N;
	C[0] = K[r];
	C[0].C = 1;
	r = rand() % N;
	C[1] = K[r];
	C[1].C = 2;

	Point med;

	while (change) {
		change = false;
		for (int i = 0; i < cls; i++) {
			for (int j = 0; j < N; j++) {
				float dist = C[i].distTo(K[j]);
				if (K[j].dist > dist) {
					K[j].dist = dist;
					K[j].C = C[i].C;
					change = true;
				}
			}
		}

		int nr;
		for (int i = 0; i < cls; i++) {
			med.x = 0;
			med.y = 0;
			nr = 0;
			for (int j = 0; j < N; j++) {
				if (K[j].C == C[i].C) {
					med.x += K[j].x;
					med.y += K[j].y;
					nr++;
				}
			}
			C[i].x = (float)med.x / nr;
			C[i].y = (float)med.y / nr;
		}

	}



	for (int i=0; i<N; i++) {
		fprintf(out, "%d %d\n", i+1, K[i].C);
	}


	//for (int i=0; i<N; i++) {
	//	if (dim == 3) {
	//		fprintf(out, "%f %f\n", K[i].x, K[i].y);
	//	}
	//}

	return 0;
}
