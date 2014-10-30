#include "main.h"
#include "MACgrid.h"
#include "rendering.h"
#include <ctime>
#include <time.h>


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
	/*
	double h[] = { 1, 2, 3, 4 ,5};
	double j[] = { 1, 2, 30, 4 ,5};

	std::cout << "the dot product is " << dot_product(*h, *j , sizeof(h)/sizeof(h[0])) << std::endl;
	std::cout << "the max is " << max_m(*j, sizeof(j) / sizeof(j[0])) << endl;
	*/

	grid->PCG();
	vector<double> h(10000000, 4);
	vector<double> j(10000000, 2);
	double harr[10000];
	double jarr[10000];
	double beta = 100.34;
	double* sPtr = h.data();
	double* zPtr = j.data();
	clock_t t1;
	clock_t t2;
	t1 = clock();
	update_search(h.data(), j.data(), beta, h.size());//update search vector s=z+s*beta
	t1 = clock() - t1;
	printf("Method took %d clicks (%f seconds).\n", t1, ((float)t1) / CLOCKS_PER_SEC);
	t2 = clock();
	for (int i = 0; i<h.size(); i++) *sPtr++ = *zPtr + beta*(*sPtr);
	t2 = clock() - t2;
	printf("Direct took %d clicks (%f seconds).\n", t1, ((float)t1) / CLOCKS_PER_SEC);
	
	t1 = clock();
	for (int i = 0; i<h.size(); i++) h.at(i) = j.at(i) + beta * h.at(i);
	t1 = clock() - t1;
	printf("Direct access took %d clicks (%f seconds).\n", t1, ((float)t1) / CLOCKS_PER_SEC);
	
	t1 = clock();
	for (int i = 0; i < 10000; i++){
		harr[i] = 4;
		jarr[i] = 2;
	}
	t1 = clock() - t1;
	printf("Setting the c arrays took %d clicks (%f seconds).\n", t1, ((float)t1) / CLOCKS_PER_SEC);

	t1 = clock();
	for (int i = 0; i<10000; i++) harr[i] = jarr[i] + beta * harr[i];
	t1 = clock() - t1;
	printf("C array update took %d clicks (%f seconds).\n", t1, ((float)t1) / CLOCKS_PER_SEC);
	

	double a = 2;
	std::cout << "the dot product is " << dot_product(h.data(), j.data(), h.size()) << std::endl;
	std::cout << "the max is " << max_m(j.data(), j.size()) << endl;
	update_pres_res(h.data(), j.data(), 2, h.size());
	std::cout << "update_pres_res(h,j,2,2) = " << h[0] <<","<<h[1]<< endl;
	cout << "h was " << h.at(0) << endl;
	//cout << "the time is" << time(NULL) << endl;
	h = j;
	cout << "h is now " << h.at(0) << endl;
	vector<double> b[3][2];
	b[0][0].assign(5, 2);
	b[1][0].assign(4, 100);
	b[2][0].assign(3, 5);
	double mydubs[] = { 2, 3, 4, 6, 56, 2, 8, 34, 23, 7, 43, 45, 6, 732, 86, 12, 23 };
	for (int ct = 0; ct < 5; ct++){
		b[0][1].push_back(mydubs[ct]); cout << "filling iter " << ct << " with " << mydubs[ct] << endl;
	}
	for (int ct = 5; ct < 10; ct++) b[1][1].push_back(mydubs[ct]);
	b[2][1].assign(7, 0);
	double* b21ptr = b[2][1].data();
	for (int ct = 10; ct < 17; ct++) *b21ptr++=mydubs[ct];
	for (int kirk = 0; kirk < sizeof(mydubs) / sizeof(mydubs[0]); kirk++)cout << "mydubs[" << kirk << "] = " << mydubs[kirk] << endl;

	int dim1 = 2;
	int dim2 = 1;
	double* ptr = b[dim1][dim2].data();
	for (int i = 0; i < b[dim1][dim2].size(); i++){
		cout << "b[" << dim1 << "]["<<dim2<<"] iter "<<i<<" = " << *ptr++ << endl;
	}
	/*
	for (int i = 0; i<grid->nx; i++){
		for (int j = 0; j<grid->ny; j++){
			for (int k = 0; k<grid->nz; k++){
				//cout << "fillArray[" << i << "][" << j << "][" << k << "] = " << grid->getValue(11, i, j, k) << endl;

			}
		}
	}*/
//	cout << "max = " << max(&(grid->pressure)) << endl;
	/*
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









