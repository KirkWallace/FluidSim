/*
 * SemiLagrangeSim.cpp
 *
 *  Created on: Apr 24, 2014
 *      Author: Wallace
 */

#include "..\inc\SemiLagrangeSim.h"
#include "..\inc\MACgrid.h"

SemiLagrangeSim::~SemiLagrangeSim() {}
SemiLagrangeSim::SemiLagrangeSim() {}


int main(){

	MACgrid* grid = new MACgrid();
	grid->printDim();

}



