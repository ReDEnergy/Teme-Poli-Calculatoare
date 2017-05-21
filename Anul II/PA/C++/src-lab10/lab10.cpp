#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#define NONE -1

using namespace std;

vector <int> parents;
int flux [6][6];

vector<int> bfs ( int graf[][], int src, int dest){
	int i;
	queue <int> q;

	q.push(src);

	while ( parents[dest] == NONE && q.size() > 0) {
		int node = q.front();
		q.pop();

		for ( i = 0 ; i < graf.size(); i++ )
			if ( graf[node][i] > 0 && parents[i] == NONE ){
				parents[i] = node;
				q.push(i);
			}
	}

	if ( parents[dest] == NONE )
		return vector<int>();

	vector<int> rv;
	for ( i = dest; true ; i = parents[i] ){
		rv.push_back(i);
		if ( i == src )
			return reverse(rv.begin(),rv.end());
	}
}

int main(){
	int graf[][] = new int[6][6];
	int i;
	vector<int> rv;

	flux = new int[6][6];

	for ( i = 0 ; i < 6 ; i ++)
		parents[i] = NONE;

	graf[0][1] = 10;
	graf[1][2] = 5;
	graf[2][3] = 9;
	graf[0][5] = 6;
	graf[5][4] = 2;
	graf[4][3] = 4;


	while ( (rv = bfs(graf,0,3)) != vector<int>() ){
		int flux_min = 100;

		for ( int i = 0 ; i < 5; i ++)
			if ( flux[ rv[i] ][ rv[i + 1] ] < flux_min )
				flux_min = flux [ rv[i] ] [ rv[i + 1] ];

		for ( i = 0; i < 5 ; i ++)
			flux[ rv[i] ] [ rv[i + 1] ] -= flux_min;

	}

	for ( int i = 0 ; i < 5 ; i ++){
		for ( int j = 0 ; j < 5 ; j ++ )
			cout << flux[i][j] << " " ;
		cout << endl;
	}
	return 0;
}
