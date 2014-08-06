


#include "MACgrid.h"

namespace MathLib
{

	
	//template argument deduction
	
	double dot_product(double &a, double &b, int size);
	double max_m(double &fillArray, int size);

	class Math_Vec{
	private:

	public:
		vector<double> data;
		Math_Vec(vector<double> math_init);
		Math_Vec operator=(const Math_Vec& other);
	
	}
	
	//int math_nx = 10;
	//int math_ny = 10;
	//int math_nz = 5;
	
	//	static double ml_add(double a, double b) { return a + b; }
		// Returns a + b
//	double minMax(double(&fillArray)[math_nx][math_ny][math_nz], bool isMax);
	//double minMax(double(&fillArray)[math_nx][math_ny][math_nz], bool isMax);

		// Returns a - b
		//static double max(double(&fillArray)[gridDef::nx][gridDef::ny][gridDef::nz]);
	//double max(double(&fillArray)[math_nx][math_ny][math_nz]);

		// Returns a * b
		//static double min(double(&fillArray)[gridDef::nx][gridDef::ny][gridDef::nz]);
	//double min(double(&fillArray)[math_nx][math_ny][math_nz]);
	/*
	class ml_vector {
	public: 
		ml_vector(int u, int v, int w);
		
		double u, v, w;

		double getMag();

	};
	class ml_array {
	public: 
		ml_array(int x, int y, int z);

		int x, y, z;
		ml_vector arr1;
	};
	*/
}
