/*
 * SemiLagrangeSim.cpp
 *
 *  Created on: Apr 24, 2014
 *      Author: Wallace
 */

#include "..\inc\SemiLagrangeSim.h"
#include "..\inc\MACgrid.h"

SemiLagrangeSim::SemiLagrangeSim() {
	initField(1,2);
	width=1;
	height =2;

}
SemiLagrangeSim::~SemiLagrangeSim() {}
SemiLagrangeSim::SemiLagrangeSim(int a, int b) {
	initField(a,b);
	width=a;
	height =b;
}

void SemiLagrangeSim::initField(int w, int h)
{
	width=w;
	height =h;

}
void SemiLagrangeSim::printArea()
{
	cout<<"the area is "<<area()<<endl;

}
int main(){
/*
	SemiLagrangeSim* a = new SemiLagrangeSim(2,4);
	a->initField(3,4);
	a->printArea();
	delete a;
*/
	MACgrid* grid = new MACgrid(10,10,10);
	grid->printDim();
	double data[10][10][10];
	//double * data[grid->getDim('x')][grid->getDim('y')][grid->getDim('z')];
	for(int i=0;i<(grid->getDim('x'));i++){
		for(int j=0;j<grid->getDim('y');j++){
			for(int k=0;k<grid->getDim('z');k++){
				data[i][j][k] = 1.0;
			}
		}
	}

	grid->initGrid(data);

}



