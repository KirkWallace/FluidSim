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
#include <cstdio>
#include <ctime>
#include "MathLib.h"

namespace gridDef {

	static const int nx = 10;
	static const int ny = 10;
	static const int nz = 5; //static
}

using namespace std;
using namespace MathLib;
using namespace gridDef;

	class MACgrid{


	private:

		double gridTime; //time kept in seconds from instantiation
		double tframe; //24 frames a second
		double dt = tframe; //stores the upper limit of time we should iterate the simulation with
		const double cellWidth = 1.0;  //width of one grid cell in mm
		double dx = cellWidth / 20.0; //stores the distance traveled in each step of the discretization
		double rho = 1.0; //g/cm^3 ;density - defaults to the density of water
	public:

		double p[6]; ///I have no idea how big these need to be, but I need to move on.
		double b[6];
		double r[6];
		double s[6];
		double z[6];
		double sigma, sigmaNew; //used in the Pressure Update

	//	vector<double> p; //all of the pressure unknowns for the pressure solve Ap=d
	//	vector<double> b; //all of the pressure unknowns for the pressure solve Ap=d

		//build a 3D pointer so we don't have to do insane logic flow and or pass an array around
		//for points in the centers of the MAC grid
		typedef double gridpt1D[nz];
		typedef gridpt1D gridpt2D[ny];
		//for points stored on the y dimension "walls" of the MAC grid
		typedef gridpt1D gridpt2DlongY[ny + 1];
		//for points stored on the z dimension "walls" of the MAC grid
		typedef double gridpt1DlongZ[nz + 1];
		typedef gridpt1DlongZ gridpt2DlongZ[ny];


		//must define simulation size prior to compilation
		typedef int cellType; // CONST used to control the type of cell (ie-FLUID,SOLID,AIR)
		typedef int fieldType; //CONST controls the quick fill of a specific field value (ie-empty, flat, random, etc)
		typedef int fluidFill; //CONST controls which cells are initially fluid (ie-none, full, or half)



		//constants used for logic control within this program

		fieldType EMPT = 0; //empty field (ie filled with 0)
		fieldType FLAT = 1; //flat field (ie same positive non-zero value)
		fieldType RAND = 2; //random field
		fieldType RAND20 = 3; //random field where the random value only varies by 20% of the nearest values

		static const int PRES = 10; //pressure
		static const int TEMP = 11; //temperature
		static const int QUAN = 12; //quantity
		static const int VELU = 13; //velocity in the u dim (ie-  u)
		static const int VELV = 14; //velocity in the v dim (ie-  v)
		static const int VELW = 15; //velocity in the w dim (ie-  w)
		static const int INTV = 16; //interpVelocity

		cellType EMPTY = 0; //grid cell is empty (ie- it is a void and pressure = 0)
		cellType AIR = 1; //grid cell is full of air (ie-pressure = 0 for now)
		cellType FLUID = 2; //grid cell is full of some fluid (ie-water,plasma,etc.)
		cellType SOLID = 3; //grid cell is full of a solid (ie-rock,concrete,etc.)
		cellType FSHALF = 4; //grid cell is partially fluid and solid
		cellType FAHALF = 5; //grid cell is partially fluid and air
		cellType SAHALF = 6; //grid cell is partially solid and air

		fluidFill NONE = 0; //no grid cells are fluid cells
		fluidFill FULL = 1; //all grid cells are fluid cells
		fluidFill HALF = 2; //half of the grid cells are fluid cells, the other half is air (for now)


		double pressure[nx][ny][nz];
		double temp[nx][ny][nz];
		double quantity[nx][ny][nz];
		double velu[nx][ny][nz]; //u component of the interpolated velocity field at the grid POINTS
		double velv[nx][ny][nz]; //v component of the interpolated velocity field at the grid POINTS
		double velw[nx][ny][nz]; //w component of the interpolated velocity field at the grid POINTS
		double vel[nx][ny][nz]; //magnitude of the interpolated velocity field at the grid POINTS
		double u[nx + 1][ny][nz]; //u component of the velocity field at the grid FACES
		double v[nx][ny + 1][nz]; //v component of the velocity field at the grid FACES
		double w[nx][ny][nz + 1]; //w component of the velocity field at the grid FACES
		double rhs[nx][ny][nz];   //stores the right hand side (rhs) of the linear system for the pressure update
		double Adiag[nx + 1][ny + 1][nz + 1]; //diagonal entries of the A pressure matrix
		double Aplusi[nx][ny][nz]; //storing of the positive neighbor entries of the A pressure matrix, negative values obtained from symmetry
		double Aplusj[nx][ny][nz]; //storing of the positive neighbor entries of the A pressure matrix, negative values obtained from symmetry
		double Aplusk[nx][ny][nz]; //storing of the positive neighbor entries of the A pressure matrix, negative values obtained from symmetry

		int cell[nx][ny][nz]; //to be filled with cellTypes

		MACgrid();
		virtual ~MACgrid();

		//setup 
		void initVelocityField(int fieldType, double val); //initializes the values of the fluid velocity
		void defineFluid(int fillType); //labels each grid cell with a fluid, solid or air integer value.
		void initialize(); //initializes array values to zero before they are used

		//error testing methods
		bool cellFillGood(int fillArray); //makes sure the defineFluid() method properly filled all of the properties arrays

		//grid access methods
		void printDim(){ cout << "Grid Dimensions" << endl << "x: " << nx << " y: " << ny << " z: " << nz << endl; }
		void fill(int fillArray, int fillType); //quick fill method call
		void fill(int fillArray, int fillType, double val); //fills every entry in the grid with val
		void fillVelocity();
		double fillCell(int i, int j, int k, int fillType, double val);
		void interpVelocityField();
		double getValue(int fillArray, int i, int j, int k);

		//Time methods
		double getTime() { return gridTime; }
		void incrementTime(){ incrementTime(tframe); }
		void incrementTime(double sec) { gridTime += sec; }
		double calcTimeLimit(); //returns the time step that limits fluid travel to 5 grid cell widths
		double calcTimeLimitFast(); //returns the time step that limits fluid travel to 1 grid cell width.

		//Math methods
		double minMax(int fillArray, bool isMax);
		double max(int fillArray);
		double min(int fillArray);
		double getGridValue(int x, int y, int z){ return pressure[x][y][z]; }
		double randSP(double percent){ srand(time(NULL)); return pow(-1, rand() % 2)*.001*percent*(rand() % 10); }
		//randSP randomly signed percentage up to "percent"

		//getters
		int getDim(char a); //returns the size of the x(u), y(v), z(w) dimensions of the grid

		//grid modifiers
		void project(double delT, int velocityField);
		double usolid(int i, int j, int k);
		double vsolid(int i, int j, int k);
		double wsolid(int i, int j, int k);
		//vector<double> PCG();
		//vector<double> applyA(vector<double> s);
		//vector<double> applyPreconditioner(vector<double> r);

		double* PCG();
		double* applyA(double* s);
		double* applyPreconditioner(double* r);

		void advect(int fieldProperty);


	};









#endif /* MACGRID_H_ */
