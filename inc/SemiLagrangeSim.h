/*
 * SemiLagrangeSim.h
 *
 *  Created on: Apr 24, 2014
 *      Author: Wallace
 */

#ifndef SEMILAGRANGESIM_H_
#define SEMILAGRANGESIM_H_

#include <iostream>
using namespace std;



class SemiLagrangeSim{
public:
	int width;
	int height;
	SemiLagrangeSim();
	virtual ~SemiLagrangeSim();
	SemiLagrangeSim(int a, int b);

	void initField(int width, int height);
	int area() {return width*height;}
	void printArea();
};

#endif /* SEMILAGRANGESIM_H_ */
