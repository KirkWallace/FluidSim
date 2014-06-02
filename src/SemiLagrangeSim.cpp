/*
 * SemiLagrangeSim.cpp
 *
 *  Created on: Apr 24, 2014
 *      Author: Wallace
 */

#include "..\inc\SemiLagrangeSim.h"
#include "..\inc\MACgrid.h"

SemiLagrangeSim::~SemiLagrangeSim() {}
SemiLagrangeSim::SemiLagrangeSim() {}


int main(){

	MACgrid* grid = new MACgrid();
	//grid->printDim();
	//cout<<"max is "<<grid->max(grid->temp)<<endl;
	grid->fill(11,3,5);
	cout<<"grid[2][2][2] = "<<grid->getValue(11,2,2,2)<<endl;
	cout<<"max = "<<grid->max(11)<<endl;
	int count0=0;
	int count1=0;
	for(int i=0; i<grid->nx; i++){
		for(int j=0;j<grid->ny;j++){
			for(int k=0; k<grid->nz;k++){
				cout<<"fillArray["<<i<<"]["<<j<<"]["<<k<<"] = "<<grid->getValue(11,i,j,k)<<endl;

			}
		}
	}
	cout<<"max = "<<grid->max(11)<<endl;
	cout<<"min = "<<grid->min(11)<<endl;
/*
	typedef int testp[10];


	int test[5][10];

	testp * testpp = test;
	for(int i=0;i<5;i++){
		for(int j=0;j<10;j++){
			(*testpp)[j] = j+i*10;
		}
		testpp++;
	}
	for(int i=0;i<5;i++){
		for(int j=0;j<10;j++){
			cout<<"test["<<i<<"]["<<j<<"] = "<<test[i][j]<<endl;
		}
	}
*/
/*
	typedef int test1[10];
	typedef test1 test2[5];


	int test[2][5][10];

	test2 * testppp = test;
	for(int k=0;k<2;k++){
		for(int i=0;i<5;i++){

			for(int j=0;j<10;j++){
				testppp[k][i][j] = j+i*10+k*50;
			}
			//testppp++;
		}
	}
	for(int k=0;k<2;k++){
		for(int i=0;i<5;i++){
			for(int j=0;j<10;j++){
				cout<<"test["<<k<<"]["<<i<<"]["<<j<<"] = "<<test[k][i][j]<<endl;
				cout<<"testppp["<<k<<"]["<<i<<"]["<<j<<"] = "<<testppp[k][i][j]<<endl;
			}
		}

	}

*/




}



