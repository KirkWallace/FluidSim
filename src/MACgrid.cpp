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
#include <cstdio>

MACgrid::MACgrid():time(0),tframe(1.0/24.0)
{
	fill(0.0,'t');
	fill(0.0,'p');
	fill(0.0,'q');
}

MACgrid::~MACgrid(){}


void MACgrid::fill(double val, char var){
	if(var=='p') {
		for(int i=0; i<nx; i++){
			for(int j=0;j<ny;j++){
				for(int k=0; k<nz;k++){
					pressure[i][j][k] = val;
				}
			}
		}
	}
	else if(var=='t') {
		for(int i=0; i<nx; i++){
			for(int j=0;j<ny;j++){
				for(int k=0; k<nz;k++){
					temp[i][j][k] = val;
				}
			}
		}
	}
	else if(var=='q') {
			for(int i=0; i<nx; i++){
				for(int j=0;j<ny;j++){
					for(int k=0; k<nz;k++){
						quantity[i][j][k] = val;
					}
				}
			}
	}
	else
	{
		cout<<"Please use a character for the dimension call to fill(double,char)"<<
				" -ie: fill(1.0,'x') to put 1 in the x dimension"<<endl;
	}
}

void MACgrid::fill(double val, char var, int i, int j, int k){
	if(var=='p') {
					pressure[i][j][k] = val;
	}
	else if(var=='t') {
					temp[i][j][k] = val;
	}
	else if(var=='q') {
					quantity[i][j][k] = val;
	}
	else
	{
		cout<<"Please use a character for the dimension call to fill(double,char)"<<
				" -ie: fill(1.0,'x') to put 1 in the x dimension"<<endl;
	}
}


int MACgrid::getDim(char a){
	if(a =='x'||a =='u') return nx;
	else if(a =='y'||a =='v') return ny;
	else if(a =='z'||a =='w') return nz;
	else{
		cout<<"please use an x,y,z with getDim to refer to the dimension you want. "<<
			endl<<"Input recieved was "<<a<<endl;
		return 0;
	}
}



