/*
 * MACgrid.cpp
 *
 *  Created on: Apr 25, 2014
 *      Author: Wallace
 *
 *      Marker-And-Cell  (MAC) grid. This file is used to store simulation variables in an organized manner.
 *      Properties of interest are stored in the center of the grid cell, while the components of the velocity
 *      field are stored on the half indices.
 *
 *      ...    |                |    |   ...
 *      ...____|__v(i,j+1/2,k)__|____|___...
 *      ...    |      	        |    |   ...
 *      ...	   |    P(i,j,k)    |    |   ...
 *      ...    |      	        |    |   ...
 * 		...____|__v(i,j-1/2,k)__|____|___...
 * 		...    |                |    |   ...
 *
 * 		So the MAC grid theoretically is numbered using half indices (as shown above for one of the three dimensions), however
 * 		for ease of code naming we will always round up. So the first element of the v array would be on the bottom edge of our
 * 		MAC grid.
 *
 *		...    |                |    |   ...
 *      ...____|__v(i,j+1,k)____|____|___...
 *      ...    |      	        |    |   ...
 *      ...	   |    P(i,j,k)    |    |   ...
 *      ...    |      	        |    |   ...
 * 		...____|__v(i,j,k)______|____|___...
 *      ...    |                |    |   ...
 *
 * 		In other words accessing the pressure and all of the velocity components around it would look something like this:
 *
 * 					v(i,j+1,k)
 * 		u(i,j,k)	p(i,j,k)		u(i+1,j,k)
 * 					v(i,j,k)
 *
 * 		Where the w component would be into the screen. ie- w(1,1,0) would be closest to your face, and w(1,1,1) would be
 * 		closer to the screen.
 */

#include "..\inc\MACgrid.h"

MACgrid::MACgrid(){
	MACgrid(1,1,1);
}

MACgrid::MACgrid(int nx, int ny, int nz){
	setDim(nx, ny, nz);
}
MACgrid::~MACgrid(){}

void MACgrid::setDim(int nx, int ny, int nz){
	this->nx = nx;
	this->ny=ny;
	this->nz=nz;
	const int x =nx;
	const int y =ny;
	const int z =nz;
	pressure = new double[x][y][z];
}

void MACgrid::printDim(){
	cout<<"grid dimensions"<<endl;
	cout<<"x: "<<nx<<" y: "<<ny<<" z: "<<nz<<endl;
}

int MACgrid::getDim(char a){
	if(a =='x') return nx;
	else if(a =='y') return ny;
	else if(a =='z') return nz;
	else{
		cout<<"please use an x,y,z with getDim to refer to the dimension you want. "<<
			endl<<"Input recieved was "<<a<<endl;
		return 0;
	}
}
void MACgrid::initGrid(double &du, double &dv, double &dw){
	int j = 0;
	int k = 0;
	for(int i=0;i<(grid->getDim('x'));i++){
		j=0;
		k=0;
		pressure[i][j][k] = du[i];
		for(int j=0;j<grid->getDim('y');j++){
			k=0;
			pressure[i][j][k] = dv[j];
			for(int k=0;k<grid->getDim('z');k++){
				pressure[i][j][k] = dw[k];
			}
		}
	}


}
