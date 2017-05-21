#include <fstream>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <mpi.h>

#include "Complex.h"

using namespace std;


/*
 * Structura ce mentine informatiile despre multime
 * 	-	necesara pentru a comunica proceselor taskul corespunzator
 */
struct FileInfo {
	int type;
	int width;
	int height;
	int chunk;
	double x_min;
	double x_max;
	double y_min;
	double y_max;
	double rez;
	double steps;
	Complex C;
};


const int num_colors = 256;

unsigned char *img;

FileInfo F;


/*
 * Functia citeste informatiile din fisier si calculeaza inaltimea si latimea imaginii generate
 */
inline void readInput(char *file) {

	fstream fin;
	fin.open(file, fstream::in);

	F.C = Complex(0, 0);

	fin>>F.type;
	fin>>F.x_min>>F.x_max>>F.y_min>>F.y_max;
	fin>>F.rez>>F.steps;

	if (F.type == 1)
		fin>>F.C.x>>F.C.y;

	F.width = floor((F.x_max - F.x_min) / F.rez);
	F.height = floor((F.y_max - F.y_min) / F.rez);

	cout<<"\n";

	if (F.type == 1)
		cout<<"Computing Julia"<<endl;
	else
		cout<<"Computing Mandelbrot"<<endl;

}

/*
 * Debug - afisare info
 */
inline void printInput() {
	cout<<"Type: "<<F.type<<"\n";
	cout<<"X: "<<F.x_min<<" \t=> "<< F.x_max<<"\n";
	cout<<"Y: "<<F.y_min<<" \t=> "<< F.y_max<<"\n";
	cout<<"Rez: "<<F.rez<<"\n";
	cout<<"Steps: "<<F.steps<<"\n";
	cout<<"Complex: "; F.C.print(); cout<<"\n";
	cout<<"W: "<<F.width<<"  - H: "<<F.height<<"\n";
	cout<<"Chunk: "<<F.chunk<<endl;

}

/*
 * Scrierea imaginii in fisier
 */
inline void printImg(const char *file, unsigned char *img) {

	cout<<"Writing to file ... "<<endl;

	fstream fout;
	fout.open(file, fstream::out);

	fout<<"P2"<<"\n";
	fout<<F.width<<" "<<F.height<<"\n";
	fout<<"255"<<"\n";

	for (int i=F.height-1; i>=0; i--) {
		for (int j=0; j<F.width; j++)
			fout<<(int)img[i * F.width + j]<<" ";
		fout<<"\n";
	}

	fout.close();
}

/*
 * Functia necesara pentru a determina trebuie calculata de fiecare process
 */

inline void getProcessWork(int rank) {

	F.y_min += F.rez * rank * F.chunk;
	F.y_max = min(F.y_max, F.y_min + F.rez * F.chunk);

}


/*
 * Se uneste mesajul primit cu imaginea corespunzatoare rezultatului
 */
inline void addChuckToResult(unsigned char *temp, int rank, int size) {

	int k = 0;
	int start = rank * size;
	int end = start + size;

	if (end > F.height * F.width)
		end = F.height * F.width;

	for (int i=start; i<end; i++)
		img[i] = temp[k++];


}

/*
 * Functia de calcul al Multimii MandelBrot respectiv Julia
 */
inline void computeMJSerial(Complex C) {

	Complex Z;

	// Pozitia de input in vector
	int skip = 0;
	int step;

	// Pixeli corespondenti imaginii rezultante
	int pixel_x;
	int pixel_y = -1;

	img = new unsigned char[F.width * (F.height + 1)];

	for (double i = F.y_min; i < F.y_max; i = i + F.rez) {

		pixel_y++;
		pixel_x = -1;

		for (double j = F.x_min; j < F.x_max; j = j + F.rez) {

			step = 0;
			pixel_x++;

			if (F.type == 0) {
				C = Complex(j, i);
				Z = Complex(0, 0);
			}
			else {
				Z = Complex(j, i);
			}

			while (Z.square_module() < 4 && step < F.steps) {
				Z = Z * Z + C;
				step++;
			}

			img[skip + pixel_x] = step % num_colors;
		}

		skip += F.width;
	}
}


int main(int argc, char** argv) {

	int rank, nr_processes;
	int msg_size;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nr_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	// Procesul Master
	if (rank == 0) {

		// Citeste informatiile pentru generare
		readInput(argv[1]);

		F.chunk = ceil((float)F.height / nr_processes);

		// Se trimit informatiile catre toate procesele
		for (int i = 1; i<nr_processes; i++)
			MPI_Send(&F, sizeof(F), MPI_BYTE, i, 0, MPI_COMM_WORLD);

		// Procesul master determina ce parte din imagine calculeaza
		getProcessWork(rank);

		// Calculul Multimii
		computeMJSerial(F.C);

		// Dimensiunea maxima a imaginii calculate de fiecare proces
		msg_size = F.width * F.chunk;
		unsigned char *temp = new unsigned char[msg_size];

		// Se primesc bucatile de imagine calculate de celelalte procese
		for (int i = 1; i<nr_processes; i++) {

			MPI_Recv(temp, msg_size, MPI_CHAR, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);

			addChuckToResult(temp, status.MPI_SOURCE, msg_size);

		}

		// Se face output la imagine
		printImg(argv[2], img);

	}

	// Procesele aditionale
	else {

		// Se primesc informatii despre imagine
		MPI_Recv(&F, sizeof(F), MPI_BYTE, 0, 0, MPI_COMM_WORLD, &status);

		// Procesul determina ce parte din imagine calculeaza
		getProcessWork(rank);

		// Calculul Multimii
		computeMJSerial(F.C);

		// Dimensiune mesaj trimis
		msg_size = F.width * F.chunk;

		// Procesul trimite catre MASTER partea de imagine calculata
		MPI_Send(img, msg_size, MPI_CHAR, 0, 1, MPI_COMM_WORLD);

	}

	// Finalizare
	MPI_Finalize();
	return 0;
}
