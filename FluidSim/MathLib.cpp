#include "MathLib.h"

namespace MathLib 
{

	double dot_product(double &a, double &b, int size){
		double dotted = 0;
			for (int i = 0; i < size; i++)
					dotted += (&a)[i] * (&b)[i];
		return dotted;
	}

	double MathLib::max_m(double &fillArray, int size){
		double maxV = 0; //max Value
		if (size>0) maxV = (&fillArray)[0];
		else std::cout << "ERROR: please make sure max called on an array of length greater than 0" <<std::endl;

		for (int i = 0; i < size; i++){
			if ((&fillArray)[i] >= maxV) maxV = (&fillArray)[i];
		}
		
		return maxV;
	}


	class Math_Vec{
			
		Math_Vec(vector<double> math_init){
				for (std::vector<int>::iterator it = math_init.begin(); it != math_init.end(); ++it)
					data.push_back(it);
			}
		Math_Vec Math_Vec::operator=(const Math_Vec& other){
		
		};

	};
		///////Math methods///////////////

	//int math_nx = 10;
	//int math_ny = 10;
	//int math_nz = 5;

	/*
	double MathLib::minMax(double(&fillArray)[math_nx][math_ny][math_nz], bool isMax){


			double max = fillArray[0][0][0];
			double min = max;
			
			for (int i = 0; i < math_nx; i++){
				for (int j = 0; j < math_ny; j++){
					for (int k = 0; k < math_nz; k++){
						if (fillArray[i][j][k] >= max) max = fillArray[i][j][k];
						if (fillArray[i][j][k] <= min) min = fillArray[i][j][k];
					}
				}
			}
			
			if (isMax) return max;
			else return min;
		}
	
	double MathLib::max(double(&fillArray)[math_nx][math_ny][math_nz]){
			return minMax(fillArray, true);
		}
	double MathLib::min(double(&fillArray)[math_nx][math_ny][math_nz]){
			return minMax(fillArray, false);
		}
		*/
}