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

#include "MACgrid.h"



using namespace gridDef;

MACgrid::MACgrid() :gridTime(0), tframe(1.0 / 24.0)
{
	defineFluid(HALF);
	fill(TEMP, 1.0);
	fill(PRES, 0.0);
	fill(QUAN, 0.0);
	dt = tframe;

	cout << "cell fill TEMP: " << cellFillGood(TEMP) << endl;
	cout << "cell fill PRES: " << cellFillGood(PRES) << endl;
	cout << "cell fill QUAN: " << cellFillGood(QUAN) << endl;
	//cout << "MathLib max = " << MathLib::max(&(pressure)) << endl;
}

MACgrid::~MACgrid(){ 
	if (pressure||temp||quantity||u||v||w)
		delete pressure,temp,quantity,u,v,w;
}

//setup
void MACgrid::defineFluid(int fillType){
	double errCt = 0;
	for (int i = 0; i<nx; i++){
		for (int j = 0; j<ny; j++){
			for (int k = 0; k<nz; k++){
				if (i == 0 || i == nx - 1 || j == 0 || j == ny - 1 || k == 0 || k == nz - 1) cell[i][j][k] = SOLID; //all boundaries are solid for now
				else if (fillType == NONE)  cell[i][j][k] = AIR;
				else if (fillType == FULL)  cell[i][j][k] = FLUID;
				else if (fillType == HALF && k < (double)nz / 2)
				{
					if (j == 5) cell[i][j][k] = -1;
					else cell[i][j][k] = FLUID;
				}
				else if (fillType == HALF && k >= (double)nz / 2) cell[i][j][k] = AIR;
				else {
					cell[i][j][k] = -1;
				}
				if (cell[i][j][k] == -1) errCt++;
				//cout<<"cell["<<i<<"]["<<j<<"]["<<k<<"]= "<<cell[i][j][k]<<endl;
			}
		}
	}
	cout << "# of -1's = " << errCt << endl;

}
void MACgrid::initVelocityField(int fieldType, double val){
	srand(time(NULL));
	int xiter = nx + 1;
	int yiter = ny + 1;
	int ziter = nz + 1;

	if (fieldType == EMPT || fieldType == FLAT || fieldType == RAND) {
		for (int i = 0; i<xiter; i++){
			for (int j = 0; j<yiter; j++){
				for (int k = 0; k<ziter; k++){
					if (!(i == xiter - 1 && (j == yiter - 1 || k == ziter - 1))) u[i][j][k] = fillCell(i, j, k, fieldType, val);
					if (!(j == yiter - 1 && (i == xiter - 1 || k == ziter - 1))) v[i][j][k] = fillCell(i, j, k, fieldType, val);
					if (!(k == ziter - 1 && (j == yiter - 1 || i == xiter - 1))) w[i][j][k] = fillCell(i, j, k, fieldType, val);
				}
			}
		}
	}

	else if (fieldType == RAND20) {
		//20% variation around a value is 10% in either direction
		double rp = 10; //rp stands for random percentage variable
		//generates a random value that is no more than 10% up or down from the values around it
		double averageU = 0; //used to average comp1, comp2 and comp3
		double divByU = 3.0; //used to divide the the average, determines how many comp's are being averaged

		double averageV = 0; //used to average comp1, comp2 and comp3
		double divByV = 3.0; //used to divide the the average, determines how many comp's are being averaged

		double averageW = 0; //used to average comp1, comp2 and comp3
		double divByW = 3.0; //used to divide the the average, determines how many comp's are being averaged


		for (int i = 0; i<xiter; i++){
			for (int j = 0; j<yiter; j++){
				for (int k = 0; k<ziter; k++){

					averageU = 0;
					divByU = 3.0;
					averageV = 0;
					divByV = 3.0;
					averageW = 0;
					divByW = 3.0;


					if (i>0){
						if (!(i == xiter - 1 && (j == yiter - 1 || k == ziter - 1)))
							averageU += u[i - 1][j][k] + randSP(rp)*u[i - 1][j][k];
						else divByU--;

						if (!(j == yiter - 1 && (i == xiter - 1 || k == ziter - 1)))
							averageV += v[i - 1][j][k] + randSP(rp)*v[i - 1][j][k];
						else divByV--;

						if (!(k == ziter - 1 && (j == yiter - 1 || i == xiter - 1)))
							averageW += w[i - 1][j][k] + randSP(rp)*w[i - 1][j][k];
						else divByW--;
					}

					if (j>0){
						if (!(i == xiter - 1 && (j == yiter - 1 || k == ziter - 1)))
							averageU += u[i - 1][j][k] + randSP(rp)*u[i - 1][j][k];
						else divByU--;

						if (!(j == yiter - 1 && (i == xiter - 1 || k == ziter - 1)))
							averageV += v[i - 1][j][k] + randSP(rp)*v[i - 1][j][k];
						else divByV--;

						if (!(k == ziter - 1 && (j == yiter - 1 || i == xiter - 1)))
							averageW += w[i - 1][j][k] + randSP(rp)*w[i - 1][j][k];
						else divByW--;
					}

					if (k>0){
						if (!(i == xiter - 1 && (j == yiter - 1 || k == ziter - 1)))
							averageU += u[i - 1][j][k] + randSP(rp)*u[i - 1][j][k];
						else divByU--;

						if (!(j == yiter - 1 && (i == xiter - 1 || k == ziter - 1)))
							averageV += v[i - 1][j][k] + randSP(rp)*v[i - 1][j][k];
						else divByV--;

						if (!(k == ziter - 1 && (j == yiter - 1 || i == xiter - 1)))
							averageW += w[i - 1][j][k] + randSP(rp)*w[i - 1][j][k];
						else divByW--;
					}

					//if(divBy==0) average = val;
					//else average/=divBy;

					//arrayToFill[i][j][k]= average;

				}
			}
		}
	}
	else
	{
		cout << "Please use a character for the dimension call to fill(double,int)" <<
			" -ie: fill(1.0,10) to put 1 in the pressure field" << endl;
	}


}

void MACgrid::initialize()
{
	for (int i = 0; i < nx; i++)
	{	for (int j = 0; j < ny; j++)
		{   for (int k = 0; k < nz; k++)
			{
				Aplusi[i][j][k] = 0;
				Aplusj[i][j][k] = 0;
				Aplusk[i][j][k] = 0;
			}
		}
	};
	//iterate through nx+1, ny+1, nz+1 arrays
	for (int i = 0; i < nx+1; i++)
	{	for (int j = 0; j < ny+1; j++)
		{	for (int k = 0; k < nz+1; k++)
			{
			Adiag[i][j][k] = 0;
		    }
		}
	}
}

//////Access controls for Grid Values//////////
bool MACgrid::cellFillGood(int fillArray){

	gridpt2D * arrayToCheck;  //now arrayToFill is a pointer to an array[*][ny][nz]

	if (fillArray == PRES) arrayToCheck = pressure;
	else if (fillArray == TEMP) arrayToCheck = temp;
	else if (fillArray == QUAN) arrayToCheck = quantity;
	else arrayToCheck = pressure;

	bool err = false;
	int a = 0;
	int b = 0;
	int c = 0;
	for (int i = 0; i<nx; i++){
		for (int j = 0; j<ny; j++){
			for (int k = 0; k<nz; k++){
				a = i;
				b = j;
				c = k;
				if (arrayToCheck[i][j][k] == -1)
				{
					err = true;
					break;
				}
				 
			}
		}
	}
	cout << "last cell checked : [" << a << "][" << b << "][" << c << "]" << endl;

	return !err;
	
}
void MACgrid::fill(int fillArray, int fillType){
	cout << "fill location: " << fillArray << "    fill Type: " << fillType << endl;
	if(fillType == EMPT) fill(fillArray,FLAT,0.0); //fill every entry with 0
	else if(fillType == FLAT) fill(fillArray,FLAT,1.0); //default fills to 1.0 unless called with a value
	else if(fillType == RAND) fill(fillArray,RAND,1.0); //The limit for the random value is passed through the val variable
	else if(fillType == RAND20) fill(fillArray,RAND20,1.0); //The limit of the random fluctuation is passed through the val variable
	else {
		cout<<"ERROR in fillType: Please enter a 0-3 for the fillType. 0-empty, 1-flat, 2-rand, 3-rand20"<<endl
			<<"call to fill(int,int) made with fillType = "<<fillType<<endl;
	}
}

void MACgrid::fill(int fillArray, int fillType, double val){

	gridpt2D * arrayToFill;  //now arrayToFill is a pointer to an array[*][ny][nz]

	if (fillArray == PRES) arrayToFill = pressure;
	else if (fillArray == TEMP) arrayToFill = temp;
	else if (fillArray == QUAN) arrayToFill = quantity;
	else arrayToFill = pressure;


	if(fillType == EMPT || fillType == FLAT || fillType == RAND) {
		for(int i=0; i<nx; i++){
			for(int j=0;j<ny;j++){
				for(int k=0; k<nz;k++){
					arrayToFill[i][j][k] = fillCell(i,j,k,fillType,val);
				}
			}
		}
	}
	else if(fillType == RAND20) {
		//20% variation around a value is 10% in either direction
		double rp = 10; //10%; rp stands for random percentage variable

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
							comp1 = arrayToFill[i-1][j][k] + randSP(rp)*arrayToFill[i-1][j][k];
							average+=comp1;
						} else divBy--;

						if(j>0){
							comp2 = arrayToFill[i][j-1][k] + randSP(rp)*arrayToFill[i][j-1][k];
							average+=comp2;
						} else divBy--;

						if(k>0){
							comp3 = arrayToFill[i][j][k-1] + randSP(rp)*arrayToFill[i][j][k-1];
							average+=comp3;
						} else divBy--;

						if(divBy==0) average = val;
						else average/=divBy;

						arrayToFill[i][j][k]= average;

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

double MACgrid::fillCell(int i, int j, int k, int fillType, double val){
	srand(time(NULL));
	double fillreturn = 0.0;
	double solidfill = -2;
	double airfill = 0.0; //also empty fill
	double errorfill = -1;
	//Check the cellType before filling with a velocity
	//For now:
	//AIR and EMPTY cells receive 0
	//FLUID cells receive a velocity/fill based on the fillType
	//SOLID cells receive a -2
	//DEFAULT:
	//The outer surface of the grid is filled with solid cellTypes
	if (cell[i][j][k] == SOLID)
	{
		fillreturn = solidfill;
	}
	else if (cell[i][j][k] == FLUID)
	{
		if (fillType == EMPT) 		fillreturn = 0.0;
		else if (fillType == FLAT) 	fillreturn = val;
		else if (fillType == RAND)   fillreturn = val*.1*(double)(rand() % 10);
		else if (fillType == RAND20) fillreturn = 0.0;
		else {
			fillreturn = errorfill;
			cout << "ERR fluid" << endl;
			//cout << "ERROR in fillType: Please enter a 0-3 for the fillType. 0-empty, 1-flat, 2-rand, 3-rand20" << endl
			//	<< "call to fill(int,int) made with fillType = " << fillType << endl;
		}
	}
	else if (cell[i][j][k] == AIR)
	{
		fillreturn = airfill;
	}
	else //something other than a solid fluid or air
	{
		fillreturn = errorfill;
		cout << "missed cell: ["<<i<<"]["<<j<<"]["<<k<<"] filled with"<< cell[i][j][k] << endl;
		//cout << "ERROR: in define geometry. Expecting a cellType of 0-2. CellType received was " <<
		//	cell[i][j][k] << endl;
	}
	return fillreturn;
}


void MACgrid::interpVelocityField(){
	//Using Linear Interpolation and the nearest points on the grid faces.
	//I use three different arrays to preserve direction.

	for(int i=0; i<nx; i++){
		for(int j=0;j<ny;j++){
			for(int k=0; k<nz;k++){
				velu[i][j][k]= (u[i][j][k]+u[i+1][j][k])/2.0;
				velv[i][j][k]= (v[i][j][k]+v[i][j+1][k])/2.0;
				velw[i][j][k]= (w[i][j][k]+w[i][j][k+1])/2.0;
				vel[i][j][k]= sqrt(pow(velu[i][j][k],2)+pow(velv[i][j][k],2)+pow(velw[i][j][k],2));
			}
		}
	}
}

double 	MACgrid::getValue(int fillArray, int i, int j, int k){
	if(fillArray == PRES) 	  return pressure[i][j][k];
	else if(fillArray == TEMP) return temp[i][j][k];
	else if(fillArray == QUAN) return quantity[i][j][k];
	else if(fillArray == VELU) return velu[i][j][k];
	else if(fillArray == VELV) return velv[i][j][k];
	else if(fillArray == VELW) return velw[i][j][k];
	else return -1.0;

}

//////Time Step Calculations//////////

double MACgrid::calcTimeLimit(){
	dt = 1.0;//max(&interpVelocity);
	dt = max(INTV);
	return dt;
}

///////Math methods///////////////
double MACgrid::minMax(int fillArray, bool isMax){

	gridpt2D * arrayToFill;  //now arrayToFill is a pointer to an array[*][ny][nz]

	if (fillArray == PRES) arrayToFill = pressure;
	else if (fillArray == TEMP) arrayToFill = temp;
	else if (fillArray == QUAN) arrayToFill = quantity;
	else if (fillArray == INTV) arrayToFill = vel;
	else arrayToFill = pressure;

	double max = arrayToFill[0][0][0];
	double min = max;

	for (int i = 0; i<nx; i++){
		for (int j = 0; j<ny; j++){
			for (int k = 0; k<nz; k++){
				if (arrayToFill[i][j][k] >= max) max = arrayToFill[i][j][k];
				if (arrayToFill[i][j][k] <= min) min = arrayToFill[i][j][k];
			}
		}
	}

	if (isMax) return max;
	else return min;
}
double MACgrid::max(int fillArray){
	return minMax(fillArray, true);
}
double MACgrid::min(int fillArray){
	return minMax(fillArray, false);
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

	//boundary condition methods
double MACgrid::usolid(int i, int j, int k)
{
	double us = 0; //temporary variable to hold the value of usolid
	/*	if (cell[i + 1][j][k] == FLUID) us = - u[i + 1][j][k];
	else if (cell[i-1][j][k]==FLUID) us = -u[i][j][k];
	else if (cell[i][j][k] == FLUID && cell[i + 1][j][k] == AIR) us = 0; */
	return us;
}
double MACgrid::vsolid(int i, int j, int k)
{
	//if (cell[i][j][k])
	return 0.0;
}
double MACgrid::wsolid(int i, int j, int k)
{
	//if (cell[i][j][k])
	return 0.0;
} 
	
void MACgrid::project(double delT, int velocityField){
	initialize(); //sets all the arrays used for math to zero

	//Calculate the negative divergence b (the right hand side) with modifications at solid wall boundaries
	double scale = 1 / dx; //scale used to calculate the RHS of the linear system for pressure update
	for (int i = 0; i < nx; i++){
		for (int j = 0; j < ny; j++){
			for (int k = 0; k < nz; k++){
				if (cell[i][j][k] == FLUID)
				{	//set the rhs for fluid surrounded by fluid  p45r1
					rhs[i][j][k] = -scale * (u[i + 1][j][k] - u[i][j][k] + v[i][j + 1][k] - v[i][j][k] + w[i][j][k + 1] - w[i][j][k]);
					//now modify the rhs for solid wall boundaries  p49r1
					if (cell[i - 1][j][k]==SOLID) rhs[i][j][k] -= scale * (u[i][j][k] - usolid(i, j, k));
					if (cell[i + 1][j][k]==SOLID) rhs[i][j][k] += scale * (u[i + 1][j][k] - usolid(i + 1, j, k));

					if (cell[i][j - 1][k] == SOLID) rhs[i][j][k] -= scale * (v[i][j][k] - vsolid(i, j, k));
					if (cell[i][j + 1][k] == SOLID) rhs[i][j][k] += scale * (v[i][j + 1][k] - vsolid(i, j + 1, k));

					if (cell[i][j][k - 1] == SOLID) rhs[i][j][k] -= scale * (w[i][j][k] - wsolid(i, j, k));
					if (cell[i][j][k + 1] == SOLID) rhs[i][j][k] += scale * (w[i][j][k + 1] - wsolid(i, j, k + 1));
				}
			}
		}
	}

	//Set the entries of A (stored in Adiag, etc) p51r1.
	scale = dt / (rho*dx*dx); //scale used to set up the A pressure matrix for pressure update
	for (int i = 0; i < nx; i++){
		for (int j = 0; j < ny; j++){
			for (int k = 0; k < nz; k++){

				if (cell[i][j][k] == FLUID && cell[i+1][j][k] == FLUID){
					Adiag[i][j][k] += scale;
					Adiag[i + 1][j][k] += scale;
					Aplusi[i][j][k] -= scale;
				} 
				else if (cell[i][j][k] == FLUID && cell[i + 1][j][k] == EMPT) 
					Adiag[i][j][k] += scale;
				
				if (cell[i][j][k] == FLUID && cell[i][j + 1][k] == FLUID){
					Adiag[i][j][k] += scale;
					Adiag[i][j + 1][k] += scale;
					Aplusj[i][j][k] -= scale;
				}
				else if (cell[i][j][k] == FLUID && cell[i][j + 1][k] == EMPT) 
					Adiag[i][j][k] += scale;
				
				if (cell[i][j][k] == FLUID && cell[i][j][k + 1] == FLUID){
					Adiag[i][j][k] += scale;
					Adiag[i][j][k + 1] += scale;
					Aplusk[i][j][k] -= scale;
				}
				else if (cell[i][j][k] == FLUID && cell[i][j][k + 1] == EMPT) 
					Adiag[i][j][k] += scale;
			}
		}
	}
	//Construct the MIC(0) preconditioner.
	
	//Solve Ap=b with MICCG(0), ie, the PCG algorithm with MIC(0) as the preconditioner. 

	//Compute the new velicities U^(n+1) according to the pressure-gradient update to u
	scale = dt / (rho*dx);
	for (int i = 0; i<nx; i++){
		for (int j = 0; j<ny; j++){
			for (int k = 0; k<nz; k++){
				if (cell[i][j][k] == FLUID){
					u[i][j][k] -= scale * pressure[i][j][k];
					u[i+1][j][k] += scale * pressure[i][j][k];
					v[i][j][k] -= scale * pressure[i][j][k];
					v[i][j+1][k] += scale * pressure[i][j][k];
					w[i][j][k] -= scale * pressure[i][j][k];
					w[i][j][k+1] += scale * pressure[i][j][k];

				}
				else if (cell[i][j][k] == SOLID){
					u[i][j][k] -= scale * usolid(i,j,k);
					u[i + 1][j][k] += scale * usolid(i+1,j,k);
					v[i][j][k] -= scale * vsolid(i,j,k);
					v[i][j + 1][k] += scale * vsolid(i,j+1,k);
					w[i][j][k] -= scale * wsolid(i,j,k);
					w[i][j][k + 1] += scale * wsolid(i,j,k+1);
				}
				else if (cell[i][j][k] == AIR){

				}
				else {
				}
				//cout<<"cell["<<i<<"]["<<j<<"]["<<k<<"]= "<<cell[i][j][k]<<endl;
			}
		}
	}

}
/*
double* MACgrid::PCG(){
    int iterlimit = 100;
	double tol = .0000001; // units [s^-1]
	int count = 0;
		for (int i = 0; i < 6; i++){
		p[i] = 0;
		b[i] = 0;
		r[i] = 0;
		s[i] = 0;
		z[i] = 0;
	}
	std::vector<double>::iterator it;
	p.assign(b.begin(),b.end());

	return p;
}
double* MACgrid::applyA(double* s){
return s;
}

vector<double> MACgrid::applyPreconditioner(vector<double> r){
return r;
}
*/
vector<double> MACgrid::PCG(){
	int iterlimit = 100;
	double tol = .0000001; // units [s^-1]
	int count = 0;
	double beta = 0; //sigma ratio
	p.assign(6, 0); //set up the initial guess p=0;
	b.assign(6, 0);
	r.assign(6, 0);
	s.assign(6, 0);
	z.assign(6, 0);

	r = b; //set residual equal to b
	z = applyPreconditioner(r); //set auxillary vector
	s = z; //set the search vector equal to initial guess

	sigma = dot_product(z.data(), r.data(),z.size());

	vector<double> a = { 1.2, 2.3 };

	double* sPtr = s.data();
	double* zPtr = z.data();
	int tbeforeMeth = 0;
	int tafterMeth = 0;
	int tbefore = 0;
	int tafter = 0;

	while (count<iterlimit && max_m(r.data(), r.size()) >= tol){
		z = applyA(s); //setting the auxillary vector
		alpha = sigma / dot_product(z.data(), s.data(),z.size());//alpha = sig/(z.s)

		//update p = p+alpha*s and r = r-alpha*z
		update_pres_res(p.data(), s.data(), alpha, p.size());
		update_pres_res(r.data(), z.data(), -alpha, r.size());

		if (max_m(r.data(), r.size()) <= tol) return p;
		z = applyPreconditioner(r);
		sigmaNew = dot_product(z.data(),r.data(),z.size());
		beta = sigmaNew / sigma;
		tbeforeMeth = time(NULL);
		update_search(s.data(), z.data(), beta, s.size());//update search vector s=z+s*beta
		tafterMeth = time(NULL);
		tbefore = time(NULL);
		for(int i = 0; i<s.size();i++) *sPtr++ = *zPtr + beta*(*sPtr);
		tafter = time(NULL);
		cout << "method took " << tafterMeth - tbeforeMeth << "  while direct took " << tafter - tbefore << endl;
		sigma = sigmaNew; //update sigma
	}
	
	return a;
}
vector<double> MACgrid::applyA(vector<double> s){

	return p;
}

vector<double> MACgrid::applyPreconditioner(vector<double> r){
	return p;
}


//using Semi-Lagrangian method for advection. Meaning we use a lagrangian technique for 
//following a "particle" to feed the Eulerian volumetric method with a value
void MACgrid::advect(int fieldProperty){



}


//interpolate returns the next time step n+1 of the quantity q^n; This is where adaptability goes
double MACgrid::interpolate(double quantn, vector<double> xp){

	return 1.0; //supposed to return the next time step of the quantity quantN
}




