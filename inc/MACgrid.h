/*
 * MACgrid.h
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
 * 		  _____^______
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

#ifndef MACGRID_H_
#define MACGRID_H_

#include <vector>
#include <iostream>




using namespace std;

class MACgrid{

private:

	double gridTime; //time kept in seconds from instantiation
	double tframe; //24 frames a second
	double timeStep = tframe; //stores the upper limit of time we should iterate the simulation with
	static const double cellWidth = 1.0;  //width of one grid cell in mm

public:
	//must define simulation size prior to compilation
	static const int nx = 10;
	static const int ny = 15;
	static const int nz = 20;

	//constants used for logic control within this program
	static const int EMPT = 0; //empty field (ie filled with 0)
	static const int FLAT = 1; //flat field (ie same positive non-zero value)
	static const int RAND = 2; //random field
	static const int RAND20 = 3; //random field where the random value only varies by 20% of the nearest values
	static const int PRES = 10; //pressure
	static const int TEMP = 11; //temperature
	static const int QUAN = 12; //quantity
	static const int VELU = 13; //velocity in the u dim (ie-  u)
	static const int VELV = 14; //velocity in the v dim (ie-  v)
	static const int VELW = 15; //velocity in the w dim (ie-  w)
	static const int INTV = 16; //interpVelocity

	double pressure[nx][ny][nz], temp[nx][ny][nz], quantity[nx][ny][nz],
	interpVelocityU[nx][ny][nz],interpVelocityV[nx][ny][nz],interpVelocityW[nx][ny][nz],interpVelocity[nx][ny][nz],
	u[nx+1][ny][nz],v[nx][ny+1][nz],w[nx][ny][nz+1];

	MACgrid();
	virtual ~MACgrid();

	//grid access methods
	void printDim(){cout<<"Grid Dimensions"<<endl<<"x: "<<nx<<" y: "<<ny<<" z: "<<nz<<endl;}
	void fill(int fillLocation, int fillType); //quick fill method call
	void fill(int fillLocation, int fillType, double val); //fills every entry in the grid with val
	void fill(int fillLocation, double val, int i, int j, int k); //puts val at the index (i,j,k)
	void fillVelocity();
	void interpVelocityField();
	void initVelocityField(int fieldType,double val);
	double getValue(int fillLocation, int i, int j, int k);

	//Time methods
	double getTime() {return gridTime;}
	void incrementTime(){incrementTime(tframe);}
	void incrementTime(double sec) {gridTime+=sec;}
	double calcTimeLimit(); //returns the time step that limits fluid travel to 5 grid cell widths
	double calcTimeLimitFast(); //returns the time step that limits fluid travel to 1 grid cell width.

	//Math methods
	double minMax(int fillLocation, bool isMax);
	double max(int fillLocation);
	double min(int fillLocation);
	double getGridValue(int x, int y, int z){return pressure[x][y][z];}

	//getters
	int getDim(char a); //returns the size of the x(u), y(v), z(w) dimensions of the grid

	//grid modifiers
	void advect(int fieldProperty);
};







#endif /* MACGRID_H_ */
