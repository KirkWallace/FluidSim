#include "main.h"
#include "MACgrid.h"
#include "rendering.h"


//using namespace gridDef;
//using namespace MathLib;
using namespace rendering;
using namespace std;

int main(int argc, char *argv[]) {

	MACgrid* grid = new MACgrid();
	//grid->printDim();
	//cout<<"max is "<<grid->max(grid->temp)<<endl;
	grid->fill(11, 1, 5);
	int count0 = 0;
	int count1 = 0;
	double h[] = { 1, 2, 3, 4 ,5};
	double j[] = { 1, 2, 30, 4 ,5};

	std::cout << "the dot product is " << dot_product(*h, *j , sizeof(h)/sizeof(h[0])) << std::endl;
	std::cout << "the max is " << max_m(*j, sizeof(j) / sizeof(j[0])) << endl;
	/*
	for (int i = 0; i<grid->nx; i++){
		for (int j = 0; j<grid->ny; j++){
			for (int k = 0; k<grid->nz; k++){
				//cout << "fillArray[" << i << "][" << j << "][" << k << "] = " << grid->getValue(11, i, j, k) << endl;

			}
		}
	}*/
//	cout << "max = " << max(&(grid->pressure)) << endl;
	cout << "min = " << grid->min(11) << endl;
	cout << "please hit enter to continue" << endl;
	string a;
	getline(cin, a);


	grid->PCG(); 
	
	vector<double> pressureStore = grid->p;
	int i = 0;
	for (auto it = pressureStore.begin(); it < pressureStore.end(); ++it)
		cout << "p[" << i++ << "] = " << *it << endl;
		*/
	/* Glut setup function calls */
	drawSnowMenTut(argc, argv);

	
	return 1;
}









