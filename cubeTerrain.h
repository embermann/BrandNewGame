#pragma once
#include "pandaFramework.h"
#include "pandaSystem.h"

#include "geomVertexFormat.h"
#include "geomTriangles.h"
#include "geomNode.h"

#include <iostream>

using namespace std;

const int chunksize = 16;
const int height = 100;// max chunk height



class cubeTerrain {
public:

	cubeTerrain() {
		
		terrainNode = NodePath("terrain_node");
		terrainNode.set_pos(0, 0, 0);

		for (int x = 0; x < chunksize; x++) {
			for (int y = 0; y < chunksize; y++) {
				for (int z = 0; z < height; z++) {
					vertexMap[x][y][z] = 0;
				}
			}
		}

	}

	//Set noise map
	void set_noise_map(int noiseM[chunksize][chunksize]) {

		for (int x = 0; x < chunksize; x++) {
			for (int y = 0; y < chunksize; y++) {
				noiseMap[x][y] = noiseM[x][y];
			}
		}

	}

	//Get terrainNode
	NodePath get_node() {

		return terrainNode;

	}


	void generate() {

		GeomVertexWriter vertex(vData, "vertex");
		GeomVertexWriter normal(vData, "normal");

		for (int x = 0; x < chunksize - 1; x++) { //Generating surface vertices
			for (int y = 0; y < chunksize - 1; y++) {
				

				vertex.add_data3(x, y, noiseMap[x][y]); //palce a vertex in a lower left corner
				normal.add_data3(0, 0, 1);

				vertexMap[x][y][noiseMap[x][y]] = vertexID; //remember the ID of it
				vertexID++; 

				if (noiseMap[x][y + 1] != noiseMap[x][y]) {
					vertex.add_data3(x, y + 1, noiseMap[x][y]); //palce a vertex in a upper left corner
					normal.add_data3(0, 0, 1);

					vertexMap[x][y + 1][noiseMap[x][y]] = vertexID; //remember the ID of it
					vertexID++;
				}

				if (noiseMap[x + 1][y + 1] != noiseMap[x][y]) {
					vertex.add_data3(x + 1, y + 1, noiseMap[x][y]); //palce a vertex in a upper right corner
					normal.add_data3(0, 0, 1);

					vertexMap[x + 1][y + 1][noiseMap[x][y]] = vertexID; //remember the ID of it
					vertexID++;
				}

				if (noiseMap[x + 1][y] != noiseMap[x][y]) {
					vertex.add_data3(x + 1, y, noiseMap[x][y]); //palce a vertex in a lower right corner
					normal.add_data3(0, 0, 1);

					vertexMap[x + 1][y][noiseMap[x][y]] = vertexID; //remember the ID of it
					vertexID++;
				}
			}
		}
		
		for (int x = 0; x < chunksize - 1; x++) { //connect vertices into triangles
			for (int y = 0; y < chunksize - 1; y++) {
				//first triangle in a square
				squarePrim->add_vertex(vertexMap[x][y][noiseMap[x][y]]);
				squarePrim->add_vertex(vertexMap[x + 1][y][noiseMap[x][y]]);
				squarePrim->add_vertex(vertexMap[x][y + 1][noiseMap[x][y]]);

				//second triangle in a square
				squarePrim->add_vertex(vertexMap[x + 1][y + 1][noiseMap[x][y]]);
				squarePrim->add_vertex(vertexMap[x][y + 1][noiseMap[x][y]]);
				squarePrim->add_vertex(vertexMap[x + 1][y][noiseMap[x][y]]);
			}
		}


		


		//Create GeomNode
		PT(Geom) square = new Geom(vData);
		square->add_primitive(squarePrim);

		PT(GeomNode) squareNode = new GeomNode("squareNode");
		squareNode->add_geom(square);

		//Attach it to the terrainNode;
		terrainNode.attach_new_node(squareNode);



		cout << "done!"; 
	}

private:

	int noiseMap[chunksize][chunksize];
	int vertexMap[chunksize][chunksize][height];
	int vertexID = 0;
	NodePath terrainNode;

	PT(GeomVertexData) vData = new GeomVertexData("square", GeomVertexFormat::get_v3n3(), Geom::UH_static);
	PT(GeomTriangles) squarePrim = new GeomTriangles(Geom::UH_static);

};