#include "Utils.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <array>
#include "Eigen/Eigen"

using namespace std;

namespace PolygonalLibrary 
{
bool ImportMesh(PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;

}
//******************************
bool ImportCell0Ds(PolygonalMesh& mesh)
{	
	ifstream fstr("./Cell0Ds.csv");
	if (fstr.fail()) {
		return false;
	}
	
	string temp;
	getline(fstr, temp);
	
	list<string> listLines;

    string line;
    while (getline(fstr, line))
        listLines.push_back(line);
	fstr.close();
	
	mesh.NumCell0Ds = listLines.size();
	if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }
	
	mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);
	
	 for (string& line : listLines)
    {
		std::replace(line.begin(), line.end(), ';', ' ');
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        converter >>  id >> marker >> mesh.Cell0DsCoordinates(0, id) >> mesh.Cell0DsCoordinates(1, id);

        mesh.Cell0DsId.push_back(id);
		if (marker!=0)
	    {
		auto it = mesh.MarkerCell0Ds.find(marker);
		if(it != mesh.MarkerCell0Ds.end())
		{
			(*it).second.push_back(id);
		}
		else 
		{
			mesh.MarkerCell0Ds.insert({marker, {id}});
		}
		}
	}
		return true;
	}


bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream fstr("./Cell1Ds.csv");
	if(fstr.fail())
        return false;
	
	string temp;
	getline(fstr, temp);

    list<string> listLines;
    string line;
    while (getline(fstr, line))
        listLines.push_back(line);

    fstr.close();
    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrems = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (string& line : listLines)
    {
		std::replace(line.begin(), line.end(), ';', ' ');
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >>  id >> marker >>  mesh.Cell1DsExtrems(0, id) >>  mesh.Cell1DsExtrems(1, id);
        mesh.Cell1DsId.push_back(id);
		if (marker!=0)
	    {
		auto it = mesh.MarkerCell1Ds.find(marker);
		if(it != mesh.MarkerCell1Ds.end())
		{
			(*it).second.push_back(id);
		}
		else 
		{
			mesh.MarkerCell1Ds.insert({marker, {id}});
		}
		}
	}
		return true;
}

bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream fstr("./Cell2Ds.csv");
	if(fstr.fail())
        return false;
	
	string temp;
	getline(fstr, temp);

    list<string> listLines;
    string line;
    while (getline(fstr, line))
        listLines.push_back(line);

    fstr.close();
    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);
	
	for (string& line : listLines)
    {
		std::replace(line.begin(), line.end(), ';', ' ');
        istringstream converter(line);
		unsigned int id;
		unsigned int marker;
		unsigned int nVert;
		unsigned int vertice;
        vector<unsigned int> vertices;
		unsigned int nEdg;
		unsigned int lato;
        vector<unsigned int> edges;
		
		converter >> id;
		converter >> marker;
		if (marker!=0)
	    {
		auto it = mesh.MarkerCell2Ds.find(marker);
		if(it != mesh.MarkerCell2Ds.end())
		{
			(*it).second.push_back(id);
		}
		else 
		{
			mesh.MarkerCell2Ds.insert({marker, {id}});
		}
		}
		converter >> nVert;
		for (unsigned int i = 0; i < nVert; i++){
			converter >> vertice;
            vertices.push_back(vertice);
		}
		converter >> nEdg;
		for (unsigned int i = 0; i < nEdg; i++){
			converter >> lato;
            edges.push_back(lato);
		}
		
		mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);
	}	
	return true;
}
}
