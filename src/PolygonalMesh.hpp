#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace PolygonalLibrary {

struct PolygonalMesh
{
	unsigned int NumCell0Ds;
	unsigned int NumCell1Ds;
	unsigned int NumCell2Ds;
	
	
	vector<unsigned int> Cell0DsId;
	vector<unsigned int> Cell1DsId;
	vector<unsigned int> Cell2DsId;
	
	Eigen::MatrixXd Cell0DsCoordinates;
	Eigen::MatrixXi Cell1DsExtrems;
	
	map<unsigned int, list<unsigned int>> MarkerCell0Ds;
	map<unsigned int, list<unsigned int>> MarkerCell1Ds;
	map<unsigned int, list<unsigned int>> MarkerCell2Ds;
	vector<vector<unsigned int>> Cell2DsVertices;
    vector<vector<unsigned int>> Cell2DsEdges;
};

}
