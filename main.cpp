#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <iomanip>

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;
int main()
{
	PolygonalMesh mesh;

    if(!ImportMesh(mesh))
    {
        cerr << "file not found" << endl;
        return 1;
    }
	
	//TEST PER I MARKER 
	cout<< "Test Marker"<< endl;
	cout<< endl;
	for (const auto& chiave : mesh.MarkerCell0Ds) {
		cout<<"Punti con marker "<< chiave.first << ":";
		for (const auto& punto : chiave.second)
			cout << punto << " ";
		cout<<endl;
	}
	cout<<endl;
	for (const auto& chiave : mesh.MarkerCell1Ds) {
		cout<<"Lati con marker "<< chiave.first << ":";
		for (const auto& lato : chiave.second)
			cout << lato << " ";
		cout<<endl;
	}
	cout<<endl;
	for (const auto& chiave : mesh.MarkerCell2Ds) {
		cout<<"Poligoni con marker "<< chiave.first << ":";
		for (const auto& poligono : chiave.second)
			cout << poligono << " ";
		cout<<endl;
	}
		
	// TEST PER LA LUNGHEZZA DEI LATI
	vector<int> lati_nulli;
	for (int edge: mesh.Cell1DsId){
		double a =mesh.Cell0DsCoordinates(0,mesh.Cell1DsExtrems(0,edge))- mesh.Cell0DsCoordinates(0,mesh.Cell1DsExtrems(1,edge));
		double b =mesh.Cell0DsCoordinates(1,mesh.Cell1DsExtrems(0,edge))- mesh.Cell0DsCoordinates(1,mesh.Cell1DsExtrems(1,edge));
		double len = sqrt(a*a + b*b);
		if( len < std::numeric_limits<double>::epsilon() ){
			lati_nulli.push_back(edge);
			cout << "Il lato "<< edge << " ha lunghezza nulla" << endl;
		}
	}
	int diml = lati_nulli.size();
	if (diml == 0) {
		cout << "Tutti i lati sono corretti" << endl; }
	
	// TEST PER L'AREA DEI POLIGONI
	vector<int> poligoni_nulli;
	for (int polyg: mesh.Cell2DsId){
		vector<unsigned int> vertices = mesh.Cell2DsVertices[polyg];
		double area = 0;
		int numVert =  vertices.size();

		// trovo baricentro
		double x_bar = 0;
		double y_bar = 0;
		for (unsigned int vert : vertices){
			x_bar += mesh.Cell0DsCoordinates( 0, vert);
			y_bar += mesh.Cell0DsCoordinates( 1, vert);
		}
		x_bar = x_bar/double(numVert);
		y_bar = y_bar/double(numVert);

		//traslo tutti i vertici in modo da avere baricentro nell'origine
		map<double, unsigned int> angoli;
		for (int i = 0; i< numVert; i++){
			double x_trasl = mesh.Cell0DsCoordinates( 0, vertices[i]) - x_bar;
			double y_trasl = mesh.Cell0DsCoordinates( 1, vertices[i]) - y_bar;
			double ang = atan2(y_trasl, x_trasl);
			if (ang<0)
			ang+= 2*M_PI;
			angoli.insert({ang,{vertices[i]}});
		}
		int vert_orario[numVert];
		int i = 0;
		for (auto [angolo, vertice] : angoli){
			vert_orario[i] = vertice;
			i++;
		}

		for (int i = 0; i<numVert-1;i++)
		{
			unsigned int vert1 = vert_orario[i];
			unsigned int vert2 = vert_orario[i+1];
			area =area + mesh.Cell0DsCoordinates( 0, vert1) * mesh.Cell0DsCoordinates( 1, vert2);
			area =area - mesh.Cell0DsCoordinates( 1, vert1) * mesh.Cell0DsCoordinates( 0, vert2);
		}
		area = area + mesh.Cell0DsCoordinates( 0, vert_orario[numVert-1]) * mesh.Cell0DsCoordinates( 1, vert_orario[0]);
		area = area - mesh.Cell0DsCoordinates( 1, vert_orario[numVert-1]) * mesh.Cell0DsCoordinates(0, vert_orario[0]);

		area = fabs(area);
		area = area/2.0;

		double eps = std::numeric_limits<double>::epsilon();
		double tol2d = sqrt(3.0)*(eps*eps)/4.0;

		if( area < tol2d){
			cout << "Il poligono" << polyg << "ha area nulla";
			poligoni_nulli.push_back(polyg);
		}
	}
	int dimp = poligoni_nulli.size();
	if (dimp == 0){
		cout << "Tutti i poligoni sono corretti" << endl; }

	Gedim::UCDUtilities utilities;
    {
        vector<Gedim::UCDProperty<double>> cell0Ds_properties(1);

        cell0Ds_properties[0].Label = "Marker";
        cell0Ds_properties[0].UnitLabel = "-";
        cell0Ds_properties[0].NumComponents = 1;

        vector<double> cell0Ds_marker(mesh.NumCell0Ds, 0.0);
        for(const auto &m : mesh.MarkerCell0Ds)
            for(const unsigned int id: m.second)
                cell0Ds_marker.at(id) = m.first;

        cell0Ds_properties[0].Data = cell0Ds_marker.data();

        utilities.ExportPoints("./Cell0Ds.inp",
                               mesh.Cell0DsCoordinates,
                               cell0Ds_properties);
    }
	
	{

        vector<Gedim::UCDProperty<double>> cell1Ds_properties(1);

        cell1Ds_properties[0].Label = "Marker";
        cell1Ds_properties[0].UnitLabel = "-";
        cell1Ds_properties[0].NumComponents = 1;

        vector<double> cell1Ds_marker(mesh.NumCell1Ds, 0.0);
        for(const auto &m : mesh.MarkerCell1Ds)
            for(const unsigned int id: m.second)
                cell1Ds_marker.at(id) = m.first;

        cell1Ds_properties[0].Data = cell1Ds_marker.data();

        utilities.ExportSegments("./Cell1Ds.inp",
                                 mesh.Cell0DsCoordinates,
                                 mesh.Cell1DsExtrems,
                                 {},
                                 cell1Ds_properties);
    }
						   
	
    return 0;
}
