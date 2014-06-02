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
 *		  _____^______
 *		 |"Grid Face"|  The grid "face" refers to the walls of the square, while the "center" is a value at the middle (in this case
 *		 P is in the grid "center", while the v's are stored on the faces)
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
#include <ctime>
#include <math.h>
#include <algorithm>

MACgrid::MACgrid():gridTime(0),tframe(1.0/24.0)
{
	fill(TEMP,1.0);
	fill(PRES,0.0);
	fill(QUAN,0.0);

}

MACgrid::~MACgrid(){}

//////Access controls for Grid Values//////////
void MACgrid::fill(int fillLocation, int fillType){
	if(fillType == EMPT) fill(fillLocation,FLAT,0.0); //fill every entry with 0
	else if(fillType == FLAT) fill(fillLocation,FLAT,1.0); //default fills to 1.0 unless called with a value
	else if(fillType == RAND) fill(fillLocation,RAND,1.0); //The limit for the random value is passed through the val variable
	else if(fillType == RAND20) fill(fillLocation,RAND20,1.0); //The limit of the random fluctuation is passed through the val variable
	else {
		cout<<"ERROR in fillType: Please enter a 0-3 for the fillType. 0-empty, 1-flat, 2-rand, 3-rand20"<<endl
			<<"call to fill(int,int) made with fillType = "<<fillType<<endl;
	}
}
void MACgrid::fill(int fillLocation, int fillType, double val){
	srand(time(NULL));
	//build a 3D pointer so we don't have to do insane logic flow and or pass an array around
		//for points in the centers of the MAC grid
		typedef double gridpt1D[nz];
		typedef gridpt1D gridpt2D[ny];
		gridpt2D * fillArray;  //now fillArray is a pointer to an array[*][ny][nz]
		//for points stored on the y dimension "walls" of the MAC grid
		typedef gridpt1D gridpt2DlongY[ny+1];
		//for points stored on the z dimension "walls" of the MAC grid
		typedef double gridpt1DlongZ[nz+1];
		typedef gridpt1DlongZ gridpt2DlongZ[ny];


	if (fillLocation == PRES) fillArray = pressure;
	else if (fillLocation == TEMP) fillArray = temp;
	else if (fillLocation == QUAN) fillArray = quantity;
/*
	else if (fillLocation == VELU) gridpt2D * fillArray = u;
	else if (fillLocation == VELV) gridpt2DlongY * fillArray = v;
	else if (fillLocation == VELW) gridpt2DlongZ * fillArray = w;
*/

	if(fillType == EMPT) {
		for(int i=0; i<nx; i++){
			for(int j=0;j<ny;j++){
				for(int k=0; k<nz;k++){
					fillArray[i][j][k]= 0.0;
				}
			}
		}
	}
	else if(fillType == FLAT) {
		for(int i=0; i<nx; i++){
			for(int j=0;j<ny;j++) {
				for(int k=0; k<nz;k++){
					fillArray[i][j][k] = val;
				}
			}
		}
	}
	else if(fillType == RAND) {
		for(int i=0; i<nx; i++){
			for(int j=0;j<ny;j++){
				for(int k=0; k<nz;k++){
					fillArray[i][j][k] = val*.1*(double)(rand() % 10) ;
				}
			}
		}
	}
	else if(fillType == RAND20) {
		//generates a random value that is no more than 10% up or down from the values around it
		double comp1 = 0; // "component 1" holds the last value placed in the first level (x dim)
		double comp2 = 0; // "component 2" holds the last value placed in the second level (y dim)
		double comp3 = 0; // "component 3" holds the last value placed in the third level (z dim)
		double average = 0; //used to average comp1, comp2 and comp3
		double divBy = 3.0; //used to divide the the average, determines how many comp's are being averaged
			for(int i=0;i<nx; i++){
				for(int j=0;j<ny;j++){
					for(int k=0;k<nz;k++){

						average = 0;
						divBy = 3.0;

						if(i>0){
							comp1 = fillArray[i-1][j][k] + pow(-1,rand()%2)*.01*(rand()%10)*fillArray[i-1][j][k];
							average+=comp1;
						} else divBy--;

						if(j>0){
							comp2 = fillArray[i][j-1][k] + pow(-1,rand()%2)*.01*(rand()%10)*fillArray[i][j-1][k];
							average+=comp2;
						} else divBy--;

						if(k>0){
							comp3 = fillArray[i][j][k-1] + pow(-1,rand()%2)*.01*(rand()%10)*fillArray[i][j][k-1];
							average+=comp3;
						} else divBy--;

						if(divBy==0) average = val;
						else average/=divBy;

						fillArray[i][j][k]= average;

					}
				}
			}
	}
	else
	{
		cout<<"Please use a character for the dimension call to fill(double,int)"<<
				" -ie: fill(1.0,10) to put 1 in the pressure field"<<endl;
	}
}

void MACgrid::fill(int fillLocation, double val, int i, int j, int k){
	if(fillLocation == PRES) pressure[i][j][k] = val;
	else if(fillLocation == TEMP) temp[i][j][k] = val;
	else if(fillLocation == QUAN) quantity[i][j][k] = val;
	else if(fillLocation == VELU) interpVelocityU[i][j][k] = val;
	else if(fillLocation == VELV) interpVelocityV[i][j][k] = val;
	else if(fillLocation == VELW) interpVelocityW[i][j][k] = val;

	else
	{
		cout<<"Please use a character for the dimension call to fill(double,char)"<<
				" -ie: fill(1.0,'x') to put 1 in the x dimension"<<endl;
	}
}

void MACgrid::interpVelocityField(){
	//Using Linear Interpolation and the nearest points on the grid faces.
	//I use three different arrays to preserve direction.

	for(int i=0; i<nx; i++){
		for(int j=0;j<ny;j++){
			for(int k=0; k<nz;k++){
				interpVelocityU[i][j][k]= (u[i][j][k]+u[i+1][j][k])/2.0;
				interpVelocityV[i][j][k]= (v[i][j][k]+v[i][j+1][k])/2.0;
				interpVelocityW[i][j][k]= (w[i][j][k]+w[i][j][k+1])/2.0;
				interpVelocity[i][j][k]= sqrt(pow(interpVelocityU[i][j][k],2)+pow(interpVelocityV[i][j][k],2)+pow(interpVelocityW[i][j][k],2));
			}
		}
	}
}
void MACgrid::initVelocityField(int fieldType, double val){
	srand(time(NULL));
	int xiter = nx+1;
	int yiter = ny+1;
	int ziter = nz+1;

	if(fieldType == EMPT || fieldType == FLAT) {
		double tfill = 0;
		if(fieldType==FLAT) tfill=val;
		for(int i=0; i<xiter; i++){
			for(int j=0;j<yiter;j++){
				for(int k=0; k<ziter;k++){
					if(!(i==xiter-1&&(j==yiter-1||k==ziter-1))) u[i][j][k]= tfill;
					if(!(j==yiter-1&&(i==xiter-1||k==ziter-1))) v[i][j][k]= tfill;
					if(!(k==ziter-1&&(j==yiter-1||i==xiter-1))) w[i][j][k]= tfill;
				}
			}
		}
	}
	else if(fieldType == RAND) {
		for(int i=0; i<xiter; i++){
			for(int j=0;j<yiter;j++){
				for(int k=0; k<ziter;k++){
					if(!(i==xiter-1&&(j==yiter-1||k==ziter-1))) u[i][j][k]= val*.1*(double)(rand() % 10);
					if(!(j==yiter-1&&(i==xiter-1||k==ziter-1))) v[i][j][k]= val*.1*(double)(rand() % 10);
					if(!(k==ziter-1&&(j==yiter-1||i==xiter-1))) w[i][j][k]= val*.1*(double)(rand() % 10);
				}
			}
		}
	}
	else if(fieldType == RAND20) {
		//generates a random value that is no more than 10% up or down from the values around it
		double averageU = 0; //used to average comp1, comp2 and comp3
		double divByU = 3.0; //used to divide the the average, determines how many comp's are being averaged

		double averageV = 0; //used to average comp1, comp2 and comp3
		double divByV = 3.0; //used to divide the the average, determines how many comp's are being averaged

		double averageW = 0; //used to average comp1, comp2 and comp3
		double divByW = 3.0; //used to divide the the average, determines how many comp's are being averaged


		for(int i=0; i<xiter; i++){
			for(int j=0;j<yiter;j++){
				for(int k=0; k<ziter;k++){

						averageU = 0;
						divByU = 3.0;
						averageV = 0;
						divByV = 3.0;
						averageW = 0;
						divByW = 3.0;


						if(i>0){
							if(!(i==xiter-1&&(j==yiter-1||k==ziter-1)))
								averageU+= u[i-1][j][k] + pow(-1,rand()%2)*.01*(rand()%10)*u[i-1][j][k];
							else divByU--;

							if(!(j==yiter-1&&(i==xiter-1||k==ziter-1)))
								averageV+= v[i-1][j][k] + pow(-1,rand()%2)*.01*(rand()%10)*v[i-1][j][k];
							else divByV--;

							if(!(k==ziter-1&&(j==yiter-1||i==xiter-1)))
								averageW+= w[i-1][j][k] + pow(-1,rand()%2)*.01*(rand()%10)*w[i-1][j][k];
							else divByW--;
						}

						if(j>0){
							if(!(i==xiter-1&&(j==yiter-1||k==ziter-1)))
								averageU+= u[i-1][j][k] + pow(-1,rand()%2)*.01*(rand()%10)*u[i-1][j][k];
							else divByU--;

							if(!(j==yiter-1&&(i==xiter-1||k==ziter-1)))
								averageV+= v[i-1][j][k] + pow(-1,rand()%2)*.01*(rand()%10)*v[i-1][j][k];
							else divByV--;

							if(!(k==ziter-1&&(j==yiter-1||i==xiter-1)))
								averageW+= w[i-1][j][k] + pow(-1,rand()%2)*.01*(rand()%10)*w[i-1][j][k];
							else divByW--;
						}

						if(k>0){
							if(!(i==xiter-1&&(j==yiter-1||k==ziter-1)))
								averageU+= u[i-1][j][k] + pow(-1,rand()%2)*.01*(rand()%10)*u[i-1][j][k];
							else divByU--;

							if(!(j==yiter-1&&(i==xiter-1||k==ziter-1)))
								averageV+= v[i-1][j][k] + pow(-1,rand()%2)*.01*(rand()%10)*v[i-1][j][k];
							else divByV--;

							if(!(k==ziter-1&&(j==yiter-1||i==xiter-1)))
								averageW+= w[i-1][j][k] + pow(-1,rand()%2)*.01*(rand()%10)*w[i-1][j][k];
							else divByW--;
						}

						//if(divBy==0) average = val;
						//else average/=divBy;

						//fillArray[i][j][k]= average;

					}
				}
			}
	}
	else
	{
		cout<<"Please use a character for the dimension call to fill(double,int)"<<
				" -ie: fill(1.0,10) to put 1 in the pressure field"<<endl;
	}


}
double 	MACgrid::getValue(int fillLocation, int i, int j, int k){
	if(fillLocation == PRES) 	  return pressure[i][j][k];
	else if(fillLocation == TEMP) return temp[i][j][k];
	else if(fillLocation == QUAN) return quantity[i][j][k];
	else if(fillLocation == VELU) return interpVelocityU[i][j][k];
	else if(fillLocation == VELV) return interpVelocityV[i][j][k];
	else if(fillLocation == VELW) return interpVelocityW[i][j][k];
	else return -1.0;

}

//////Time Step Calculations//////////

double MACgrid::calcTimeLimit(){
	timeStep = 1.0;//max(&interpVelocity);
	timeStep = max(INTV);
	return timeStep;
}

///////Math methods///////////////
double MACgrid::minMax(int fillLocation, bool isMax){

	typedef double gridpt1D[nz];
	typedef gridpt1D gridpt2D[ny];
	gridpt2D * fillArray;  //now fillArray is a pointer to an array[*][ny][nz]

	if (fillLocation == PRES) fillArray = pressure;
	else if (fillLocation == TEMP) fillArray = temp;
	else if (fillLocation == QUAN) fillArray = quantity;
	else if (fillLocation == INTV) fillArray = interpVelocity;

	double max = fillArray[0][0][0];
	double min = max;

	for(int i=0; i<nx; i++){
		for(int j=0;j<ny;j++){
			for(int k=0; k<nz;k++){
				if(fillArray[i][j][k]>= max) max = fillArray[i][j][k];
				if(fillArray[i][j][k]<= min) min = fillArray[i][j][k];
			}
		}
	}

	if(isMax) return max;
	else return min;
}
double MACgrid::max(int fillLocation){
	return minMax(fillLocation,true);
	}
double MACgrid::min(int fillLocation){
	return minMax(fillLocation,false);
	}

//getters
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


//grid modifiers
void MACgrid::advect(int fieldProperty){



}
