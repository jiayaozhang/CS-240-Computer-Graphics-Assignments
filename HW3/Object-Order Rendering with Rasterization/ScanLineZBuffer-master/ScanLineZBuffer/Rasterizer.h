#ifndef __RASTERISATION_H
#define __RASTERISATION_H

#include "Model.h"

class Rasterizer
{
public:
	
	int width;
	int height;

	Rasterizer();
	//Rasterisation(Model& model);
	void setSize(int width, int height);//
	void getSize(int& width, int& height);

	void run(Model& model);//


#endif