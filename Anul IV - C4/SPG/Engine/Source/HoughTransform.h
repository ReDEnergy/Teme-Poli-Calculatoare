#pragma once
#include "Globals.h"
#include "Texture.h"
#include <queue>

using namespace std;

namespace HoughTransform {

	unsigned char* out_image;
	float *lSIN, *lCOS;
	float *cSIN, *cCOS;
	int **line_acum;
	int **circle_acum;
	int ntheta = 180;
	int ThA_line = 350;
	int N, M;
	Texture hough_transform;
	FILE *Fdebug;

	void plotLine(unsigned char *image, int theta, int ro) {
		int y = 0, lasty, minY, maxY;
		for (int x = 0; x < M; x++) {
			lasty = y;
			y = (int)((ro - x * lCOS[theta]) / lSIN[theta]);
			minY = min(y, lasty);
			maxY = max(y, lasty);
			if (maxY < 0 || minY > N)
				continue;
			if (maxY > N) maxY = N-1;
			if (minY < 0) minY = 0;

			if (image[3 * minY * M + 3 * x] == 0 || image[3 * maxY * M + 3 * x] == 0)
				continue;

			if (maxY - minY < 10) {
				for (int l = minY; l <= maxY; l++) {
					if (image[3 * l * M + 3 * x] == 0)
						return;
				}
			}

			for (int l = minY; l <= maxY; l++) {
				out_image[3 * l * M + 3 * x] = 255;
				if (l > 2)			out_image[3 * (l - 3) * M + 3 * x] = 255;
				if (l > 1)			out_image[3 * (l - 2) * M + 3 * x] = 255;
				if (l > 0)			out_image[3 * (l - 1) * M + 3 * x] = 255;
				if (l < (N - 1))	out_image[3 * (l + 1) * M + 3 * x] = 255;
				if (l < (N - 2))	out_image[3 * (l + 2) * M + 3 * x] = 255;
				if (l < (N - 3))	out_image[3 * (l + 3) * M + 3 * x] = 255;

				if (x > 1)			out_image[3 * l * M + 3 * (x - 2)] = 255;
				if (x > 0)			out_image[3 * l * M + 3 * (x - 1)] = 255;
				if (x < (M - 1))	out_image[3 * l * M + 3 * (x + 1)] = 255;
				if (x < (M - 2))	out_image[3 * l * M + 3 * (x + 2)] = 255;
			}
		}
	}

	void plotCircle(int a, int b, int radius) {
		int x, y;
		cout << "\tCircle: (" << a << ", " << b << ")" << endl;

		int steps = int(2 * M_PI * radius);
		float degree = float(3600.0 / steps);
		for (int step = 0; step < steps; step++) {
			x = int(a + radius * cCOS[int(step * degree)]);
			y = int(b + radius * cSIN[int(step * degree)]);
			if (x >= 0 && x < M && y >= 0 && y < N) {
				out_image[3 * y * M + 3 * x + 2] = 255;
			}
		}
	}


	void fillShape(unsigned char* image, int x, int y) {
		queue<pair <int, int> > pixels;

		const int dx[] = { 0, 0, 1, -1};
		const int dy[] = { -1, 1, 0, 0};

		bool *used = new bool[3 * M * N];
		memset(used, 0, 3 * M * N);

		pixels.push(make_pair(x, y));
		used[y * M + x] = true;

		while (!pixels.empty()) {
			pair <int, int> p = pixels.front();
			pixels.pop();
			for (int i = 0; i < 4; ++i) {
				int xx = p.first + dx[i];
				int yy = p.second + dy[i];
				if (xx >= 0 && xx < M && yy >=0 && yy < N && used[yy * M + xx] == false) 
				{
					if (out_image[3 * yy * M + 3 * xx] || out_image[3 * yy * M + 3 * xx + 2]) {
						pixels.push(make_pair(xx, yy));
						used[yy * M + xx]  = true;
					}
				}
			}
			out_image[3 * p.second * M + 3 * p.first] = 0;
			out_image[3 * p.second * M + 3 * p.first + 1] = 255;
			out_image[3 * p.second * M + 3 * p.first + 2] = 0;
		}
	}

	void solveTheMostStupidHomeworkEver(unsigned char *image) { // I mean it
		for (int y = 0; y < N; y++) {
			for (int x= 0; x < M; x++) {
				if (image[3 * y * M + 3 * x] &&
					out_image[3 * y * M + 3 * x] && 
					out_image[3 * y * M + 3 * x + 2] && 
					out_image[3 * y * M + 3 * x + 1] == 0)
				{
					fillShape(image, x, y);
				}
			}
		}
	}

	void resetLineWorkingMemory() {
		memset(out_image, 0, 3 * N * M);
		for (int i = 0; i < ntheta; i++)
			memset(line_acum[i], 0, M * sizeof(int));
	}

	void resetCircleWorkingMemory() {
		for (int i = 0; i < N; i++)
			memset(circle_acum[i], 0, M * sizeof(int));
	}

	void detectLines(unsigned char *image, int min_angle, int max_angle) {

		float D = (float)sqrt(N * N + M * M);

		resetLineWorkingMemory();

		cout << "Hough Transform: Line Acumulator" << endl;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (image[3 * i * M + 3 * j] == 255) {
					for (int theta = min_angle; theta < max_angle; theta++) {
						float ro = abs(i * lSIN[theta] + j * lCOS[theta]);
						int rop = (int)((M - 1) * (ro + D) / (2 * D));
						line_acum[theta][rop]++;
					}
				}
			}
		}

		cout << "Hough Transform: Extract Lines" << endl;
		float ro;
		for (int theta = 0 ; theta < ntheta -1 ; theta++) {
			for (int rop = 0 ; rop < M ; rop++) {
				if (line_acum[theta][rop] > ThA_line) {
					ro = ((rop * 2 * D) / (M - 1)) - D;
					plotLine(image, theta, (int)ro);
				}
			}
		}
	}

	void detectCircles(unsigned char *image, int min_radius, int max_radius) {

		int x, y;
		cout << "Hough Transform: Circle" << endl;
		int maxA = 0;
		for (int radius = min_radius; radius < max_radius; radius++) {
			maxA = 0;
			resetCircleWorkingMemory();
			cout << "Circle Detection - radius: " << radius << endl;
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < M; j++) {
					if (image[3 * i * M + 3 * j] == 255) {
						int steps = int(2 * M_PI * radius);
						float degree = float(3600.0 / steps);
						for (int step = 0; step < steps; step++) {
							y = int(i + radius * cSIN[int(step * degree)]);
							x = int(j + radius * cCOS[int(step * degree)]);
							if (x >= 0 && x < M && y >= 0 && y < N) {
								circle_acum[y][x]++;
								maxA = max(circle_acum[y][x], maxA);
							}
						}
					}
				}
			}

			cout << "\tMax Thresholding: " << maxA << endl;

			for (int y = 0; y < N; y++) {
				for (int x = 0; x < M; x++) {
					if (circle_acum[y][x] > 2.5 * radius) {
						plotCircle(x, y, radius);
					}
				}
			}
			cout << endl;
		}
	}

	void generateTexture() {
		cout << "Hough Line Transform Create Texture" << endl;
		hough_transform.create2DTexture(out_image, M, N, 3);
	}

	void bindTexture(GLenum TextureUnit) {
		hough_transform.bind(TextureUnit);
	}

	void init(int height, int width) {
		N = height;
		M = width;

		hough_transform.load2D("Textures\\tema4.png");

		// Debug Files
		//Fdebug = fopen("out.txt", "w");

		out_image = new GLubyte[3 * N * M];

		lCOS = new float[ntheta];
		lSIN = new float[ntheta];

		for (int i=0; i<ntheta; i++)
		{
			float t = (float)( i * M_PI / (ntheta - 1) - M_PI_2 );
			lCOS[i] = (float)cos(t);
			lSIN[i] = (float)sin(t);
		}

		cCOS = new float[3600];
		cSIN = new float[3600];

		for (int i=0; i<3600; i++)
		{
			float t = (float)(i * 2 * M_PI / 3600);
			cCOS[i] = (float)cos(t);
			cSIN[i] = (float)sin(t);
		}


		line_acum = new int*[ntheta];
		for (int i = 0; i < ntheta; i++)
			line_acum[i] = new int[M];

		circle_acum = new int*[N];
		for (int i=0; i<N; i++)
			circle_acum[i] = new int[M];
		
	}
}