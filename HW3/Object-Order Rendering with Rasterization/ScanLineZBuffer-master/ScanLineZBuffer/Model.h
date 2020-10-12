#ifndef __MODEL_H
#define __MODEL_H

#include <iostream>
#include <vector>
#include <string>
#include "Vec.h"

class Vertex
{
public:
	Point3f point;
	Color3f color;
	Vec3f normal;// vector normal
};

class Face
{
public:
	std::vector<int> vertexIdx;
	std::vector<int> normalIdx;
	// obj file would provide the index of surface, 
	//normal vector (vertex normal vector)
	Vec3f normal;// surface normal vector determined by 3 points
	Color3f color;// the average color of each vertex on the bin
};

// model for loading obj file
class Model
{
public:
	std::vector<Vertex> vertexes;
	std::vector<Face> faces;
	std::vector<Vec3f> normals;// vertex normal vector
	Point3f center_point;
	
	bool load(const std::string& path);
	Model(){};
	Model(const std::string& path);
	void rotate(float** RotateMat, std::vector<Vertex>& vertexes);

	//----------modify---------
	void rescale(double scale);
	//------------------------
	
	//void transform(float theta);
};

#endif