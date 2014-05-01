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

	double time; //time kept in sec from instantiation
	double tframe; //24 frames a second

	//void initGrid();

public:
	//must define simulation size prior to compilation
	static const int nx = 10;
	static const int ny = 15;
	static const int nz = 20;

	double pressure[nx][ny][nz], temp[nx][ny][nz], quantity[nx][ny][nz],
	u[nx+1][ny][nz],v[nx][ny+1][nz],w[nx][ny][nz+1];

	MACgrid();
	virtual ~MACgrid();

	//grid access methods
	void printDim(){cout<<"Grid Dimensions"<<endl<<"x: "<<nx<<" y: "<<ny<<" z: "<<nz<<endl;}
	void fill(double val, char var); //fills every entry in the grid with val
	void fill(double val, char var, int i, int j, int k); //puts val at the index (i,j,k)

	//Time methods
	double getTime() {return time;}
	void incrementTime(){incrementTime(tframe);}
	void incrementTime(double sec) {time+=sec;}

	double getGridValue(int x, int y, int z){return pressure[x][y][z];}

	//getters
	int getDim(char a); //returns the size of the x(u), y(v), z(w) dimensions of the grid

};



#endif /* MACGRID_H_ */
