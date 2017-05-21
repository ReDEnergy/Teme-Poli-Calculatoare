#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#include <string.h>
#include <queue>

/**	Define pentru pixel accesat prin coordonata
 **/
#define IMG(C) image[C.x][C.y]

using namespace std;

/**	Structura pentru Constante MIU, SIGMA,
 ** cap => valorea capacitatilor posibile (0,255) pentru un pixel
 **/
struct Constants {
	double uB;
	double uF;
	double sigmaB;
	double sigmaF;
	double capF[256];
	double capB[256];
};

/**	Structura pentru informatiile despre un pixel
 **/
struct Pixel {
	int value;				// Valoare pixel
	double capacity[4];		// Capacitatile catre vecinii pixelului
	double source;			// Capacitatea catre Foreground
	double dest;			// Capacitatea catre Background
	int parent;				// Parintele pentru construirea Path-ului
	int visited;			// Daca pixelul a vost vizitat sau nu
};

/**	Coordonate x,y pentru un pixel
 **/
struct Coord {
	int x;
	int y;
};

int lambda;			// Constanta lambda
int prag;			// Prag, Threshold
int test_number;	// numarul folderului de testing
int cols, rows;		// numarul de coloane, randuri imagine
double ENERGY = 0;	// Energia configuratiei

/**	Transformare rapida a directiilor de deplasare  **/
Coord dir[4];		// Directiile posibile de deplasare

vector < vector <Pixel> > image;	// Imaginea
vector <Coord> finish;				// Pixeli care termina un path
queue <Coord> Q;					// Coada pentru BFS

FILE *in, *out;						// Fisiere
Constants C;						// Constante

/**	Functie care citeste parametrii
 **/
void readParameters() {
	in = fopen("parametri.txt", "r");
	fscanf(in, "%d %d", &lambda, &prag);
	printf("Lambda: %d \t Prag: %d\n", lambda, prag);
	fclose(in);
}

/**	Citeste informatiile despre imagine
 **/
void readImage() {
	in = fopen("imagine.pgm", "r");

	Pixel X;
	char skip[100];
	int useless_max;

	fgets(skip, 100, in);
	fscanf(in, "%d %d", &cols, &rows);
	fscanf(in, "%d", &useless_max);

	for (int i=0; i<rows; i++) {
		vector <Pixel> img_row;
		for (int j=0; j<cols; j++) {
			fscanf(in, "%d", &X.value);
			img_row.push_back(X);
		}
		image.push_back(img_row);
	}

	printf("Cols: %d \t Rows: %d\n", cols, rows);
	printf("\n");
	fclose(in);
}

/**	Calculeaza constantele pe baza informatiilor din MASK
 **/
void readMask(char *mask_file, double &u, double &sigma) {
	vector<int> mask;
	int valuePixel;
	char skip[100];
	double sum = 0;

	in = fopen(mask_file, "r");

	/**	Skip first 3 lines
	 **/
	for (int i=0; i<3; i++)
		fgets(skip, 100, in);


	/**	Salveaza locatia pixelilor marcati din masca
	 **/
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++) {

			fscanf(in, "%d", &valuePixel);

			if (valuePixel) {
				sum += image[i][j].value;
				mask.push_back(image[i][j].value);
			}
		}
	}

	/**	Calculeaza MIU
	 **/
	u = (double)sum / mask.size();


	/**	Calculeaza SIGMA
	 **/
	sum = 0;
	for (unsigned int i=0; i<mask.size(); i++)
		sum += ((double)u - mask[i]) * (u - mask[i]);

	sigma = ((double)sqrt( sum / mask.size()));

	/**	Afiseaza MIU si SIGMA
	 **/
	printf("u: %f \t sigma: %f \n", u, sigma);
	fclose(in);
}

/**	Seteaza directiile posibile de deplasare
 ** Pentru a tranlata printr-un int valoarea pixelului
 **/
void setDirections() {
	dir[0].x = -1;			// SUS
	dir[0].y = 0;

	dir[1].x = 0;			// DREAPTA
	dir[1].y = 1;

	dir[2].x = 1;			// JOS
	dir[2].y = 0;

	dir[3].x = 0;			// STANGA
	dir[3].y = -1;
}

/**	Returneaza coordonatele noului pixel pe baza directiei de deplasare
 **/
Coord getCoordDir(Coord C, int direction) {
	Coord N;
	N.x = C.x + dir[direction].x;
	N.y = C.y + dir[direction].y;
	return N;
}

/**	Calculeaza intervalul de capacitati posibile pentru valorile 0 -> 255
 ** Calculez inainte toate valorile posibilie existente
 ** Astfel nu fac calculul pentru fiecare pixel in parte ci doar pentru 256 de
 ** valori posibile, si atribui fiecarui pixel, capacitatea de la indicele sau
 **/
void computeCapacity(double *v, double u, double sigma ) {
	double A, B;
	for (int i=0; i<256; i++) {
		A = (double)(i - u) / sigma;
		B = (double)2 * M_PI * sigma * sigma;

		v[i] = (double)0.5 * A * A + log(sqrt(B));
		v[i] = min((double)10, v[i]);
	}
}

/**	Construieste Graful adaugand vecinii unui pixel
 ** Vecinii reprezentati prin capacitate
 **/
void addNearPixel(Coord C, int direction) {
	IMG(C).capacity[direction] = 0;

	Coord NC = getCoordDir(C, direction);

	if (NC.x < 0 || NC.y < 0 || NC.x >= rows || NC.y >= cols) {
		IMG(C).capacity[direction] = -1;
		return;
	}

	if ( abs(IMG(C).value - IMG(NC).value) > prag)
		return;

	IMG(C).capacity[direction] = lambda;
}

/**	Functie care construieste graful
 **/
void createGraph() {
	setDirections();
	Coord point;

	for (unsigned int i=0; i<image.size(); i++) {
		for (unsigned int j=0; j<image[i].size(); j++) {

			point.x = i;
			point.y = j;
			for (int k=0; k<4; k++)
				addNearPixel(point, k);

			image[i][j].source = C.capF[image[i][j].value];
			image[i][j].dest = C.capB[image[i][j].value];

		}
	}
}

/** Debugging - print graf **/
void printGraph();

/** Expandeaza pixelul curent si adauga toti vecinii nevizitati ai pixelului
 ** curent ce au capacitatea pozitiva
 ** Utilizat pentru BFS
 **/
void newStep(Coord C, int direction) {

	if (IMG(C).capacity[direction] <= 0)
		return;

	Coord NC = getCoordDir(C, direction);

	direction = (direction + 2) % 4;

	if (NC.x < 0 || NC.x >=rows || NC.y < 0 || NC.y >= cols)
		return;
	if (IMG(NC).visited == 1)
		return;

	Q.push(NC);
	IMG(NC).visited = 1;
	IMG(NC).parent = direction;
}

/** BFS PENTRU FLUX
 **/
void bfs() {
	Coord C;
	finish.clear();

	/**	Adaug toti pixeli accesibili de la sursa (capacitate > 0)
	 ** Resetez totodata vizitat = 0 pe toti pixelii
	 **/

	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {

			/**	Reseteaza pixel nevizitat **/
			image[i][j].visited = 0;
			image[i][j].parent = -1;

			if (image[i][j].source <= 0)
				continue;

			image[i][j].parent = -1;
			image[i][j].visited = 1;

			C.x = i;
			C.y = j;
			Q.push(C);
		}
	}

	while(!Q.empty()) {
		C = Q.front();
		Q.pop();

		if (IMG(C).dest > 0)
			finish.push_back(C);

		for(int k=0; k<4; k++)
			newStep(C, k);
	}
}

/** Functia satureaza un PATH reconstruind drumul utilizand campul PARENT
 **/
void saturatePath(Coord C) {
	Coord Last = C;

	/** Calculzarea MAX FLOW ce poate fi pompat prin PATH **/
	double max_flow = IMG(C).dest;

	while(IMG(C).parent != -1) {
		int goBack = (IMG(C).parent + 2) % 4;
		C = getCoordDir(C, IMG(C).parent);

		max_flow = min(max_flow, IMG(C).capacity[goBack]);
	}


	max_flow = min(max_flow, IMG(C).source);

	/** Actualizarea capacitatilor reziduale ale legaturilor dintre pixeli **/

	C = Last;
	IMG(C).dest -= max_flow;
	while(IMG(C).parent != -1) {
		int goBack = (IMG(C).parent + 2) % 4;
		IMG(C).capacity[IMG(C).parent] += max_flow;

		C = getCoordDir(C, IMG(C).parent);
		IMG(C).capacity[goBack] -= max_flow;
	}
	IMG(C).source -= max_flow;

	ENERGY += max_flow;
}

int main()
{
	out = fopen("out.txt", "w");

	readParameters();
	readImage();
	readMask((char*)"mask_fg.pgm", C.uF, C.sigmaF);
	readMask((char*)"mask_bg.pgm", C.uB, C.sigmaB);
	computeCapacity(C.capF, C.uF, C.sigmaF);
	computeCapacity(C.capB, C.uB, C.sigmaB);

	createGraph();

	/**	FLUX MAXIM - algoritmul lui DINIC **/
	do {
		bfs();
		for (unsigned int i=0; i<finish.size(); i++)
			saturatePath(finish[i]);

	} while (finish.size());


	printf("\n");
	printf("Energie : %f\n", ENERGY);


	/** Afisarea imaginii segmentate **/
	FILE *result = fopen("segment.pgm","w");
	fprintf(result, "P2\n");
	fprintf(result, "%d %d\n", cols, rows);
	fprintf(result, "255\n");
	for (int i=0; i<rows; i++) {
		for (int j=0; j<cols; j++) {
			if(image[i][j].visited == 1)
				fprintf(result,"0\n");
			else
				fprintf(result,"255\n");
		}
	}

	fclose(result);
	fclose(out);
	return 0;
}

/** Functie de debugging => Printare Grafic **/
void printGraph() {
	int nod=0;
	fprintf(out, "\n");
	for (unsigned int i=0; i<image.size(); i++) {
		for (unsigned int j=0; j<image[i].size(); j++) {
			nod++;
			fprintf(out, "nod: %d %d\n", nod, image[i][j].value);
			fprintf(out, "s: %f\t\t", image[i][j].source);
			fprintf(out, "d: %f\n", image[i][j].dest);
			for (int k=0; k<4; k++)
				if (image[i][j].capacity[k] > 0)
					fprintf(out, "v: %f \tdir %d\n", image[i][j].capacity[k], k);

			fprintf(out, "p: %d\n", image[i][j].parent);
			fprintf(out, "\n");
		}
	}
}
