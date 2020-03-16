#pragma once
#include "pandaFramework.h"
#include "pandaSystem.h"

#include "geomVertexFormat.h"
#include "geomTriangles.h"
#include "geomNode.h"

#include <iostream>

using namespace std;

const int chunksize = 16;
const int height = 100; // max chunk height  должен делиться на 2

class cube {
public:

	bool top = false; //tells if corresponding side of the cube exists
	bool up = false;
	bool left = false;
	bool right = false;
	bool down = false;


};

class cubeTerrain {
public:

	cubeTerrain() {

		terrainNode = NodePath("terrain_node");
		terrainNode.set_pos(0, 0, 0);

	}

	//Set noise map
	void set_noise_map(int noiseM[chunksize + 2][chunksize + 2]) {

		for (int x = 0; x < chunksize + 2; x++) {
			for (int y = 0; y < chunksize + 2; y++) {
				noiseMap[x][y] = noiseM[x][y];
				if (noiseMap[x][y] < min_height) {
					min_height = noiseMap[x][y];
				}
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
		cube easyMap[chunksize][chunksize][height];
		int vertexID = 0;

		for (int x = 0; x < chunksize; x++) { //easyMap generating
			for (int y = 0; y < chunksize; y++) {
				easyMap[x][y][noiseMap[x][y]].top = true;

				for (int z = noiseMap[x][y]; z >=min_height; z--) {
					if (y != chunksize - 1) {
						easyMap[x][y][z].up = true; //side up
					}
					else
					{
						if (noiseMap[x][chunksize] < z) {
							easyMap[x][y][z].up = true;
						}
					}

					if (x != chunksize - 1) {
						easyMap[x][y][z].right = true; //side right
					}
					else
					{
						if (noiseMap[chunksize][y] < z) {
							easyMap[x][y][z].right = true;
						}
					}

					if (y != 0) { //side down
						if (easyMap[x][y - 1][z].up == true) { 
							easyMap[x][y - 1][z].up = false;
							//cout << "easyMap[x][y - 1][z].up == true" << endl;
						}
						else
						{
							easyMap[x][y][z].down = true;
						}
					} 
					else
					{
						if (noiseMap[x][chunksize + 1] < z) {
							easyMap[x][y][z].down = true;
						}
					}

					if (x != 0) { //side left
						if (easyMap[x - 1][y][z].right == true) { 
							easyMap[x - 1][y][z].right = false;
						}
						else
						{
							easyMap[x][y][z].left = true;
						}
					}
					else
					{
						if (noiseMap[chunksize + 1][y] < z) {
							easyMap[x][y][z].left = true;
						}
					}
				}
			}
		}

		for (int x = 0; x < chunksize; x++) { //generating and connecting vertices
			for (int y = 0; y < chunksize; y++) {

				vertex.add_data3(x, y, noiseMap[x][y] + 1); //palce a vertex in a lower left corner
				normal.add_data3(0, 0, 1);

				vertex.add_data3(x, y + 1, noiseMap[x][y] + 1); //palce a vertex in an upper left corner
				normal.add_data3(0, 0, 1);

				vertex.add_data3(x + 1, y + 1, noiseMap[x][y] + 1); //palce a vertex in an upper right corner
				normal.add_data3(0, 0, 1);

				vertex.add_data3(x + 1, y, noiseMap[x][y] + 1); //palce a vertex in a lower right corner
				normal.add_data3(0, 0, 1);

				//first triangle in a square  ----- connecting top vertices
				squarePrim->add_vertex(vertexID);
				squarePrim->add_vertex(vertexID + 3);
				squarePrim->add_vertex(vertexID + 1);

				//second triangle in a square
				squarePrim->add_vertex(vertexID + 2);
				squarePrim->add_vertex(vertexID + 1);
				squarePrim->add_vertex(vertexID + 3);

				vertexID = vertexID + 4;

				for (int z = noiseMap[x][y]; z >= 0; z--) {
					if (easyMap[x][y][z].up == true) { //up vertices

						vertex.add_data3(x, y + 1, z); //palce a vertex in a lower left corner (looking from down)
						normal.add_data3(0, 1, 0);

						vertex.add_data3(x + 1, y + 1, z); //palce a vertex in a lower right corner (looking from down)
						normal.add_data3(0, 1, 0);

						vertex.add_data3(x, y + 1, z + 1); //palce a vertex in an upper left corner (looking from down)
						normal.add_data3(0, 1, 0);

						vertex.add_data3(x + 1, y + 1, z + 1); //palce a vertex in an upper right corner (looking from down)
						normal.add_data3(0, 1, 0);

						//first triangle in a square  ----- connecting up vertices
						squarePrim->add_vertex(vertexID);
						squarePrim->add_vertex(vertexID + 2);
						squarePrim->add_vertex(vertexID + 1);

						//second triangle in a square
						squarePrim->add_vertex(vertexID + 3);
						squarePrim->add_vertex(vertexID + 1);
						squarePrim->add_vertex(vertexID + 2);

						vertexID = vertexID + 4;
					}

					if (easyMap[x][y][z].down == true) { //down vertices

						vertex.add_data3(x, y, z); //palce a vertex in a lower left corner (looking from down)
						normal.add_data3(0, -1, 0);

						vertex.add_data3(x + 1, y, z); //palce a vertex in a lower right corner (looking from down)
						normal.add_data3(0, -1, 0);

						vertex.add_data3(x, y, z + 1); //place a vertex in an upper left corner (looking from down)
						normal.add_data3(0, -1, 0);

						vertex.add_data3(x + 1, y, z + 1); //place a vertex in an upper right corner (looking from down)
						normal.add_data3(0, -1, 0);

						//first triangle in a square  ----- connecting down vertices
						squarePrim->add_vertex(vertexID);
						squarePrim->add_vertex(vertexID + 1);
						squarePrim->add_vertex(vertexID + 2);

						//second triangle in a square
						squarePrim->add_vertex(vertexID + 3);
						squarePrim->add_vertex(vertexID + 2);
						squarePrim->add_vertex(vertexID + 1);

						vertexID = vertexID + 4;
					}

					if (easyMap[x][y][z].right == true) { //right vertices

						vertex.add_data3(x + 1, y + 1, z); //palce a vertex in a lower left corner (looking from left)
						normal.add_data3(1, 0, 0);

						vertex.add_data3(x + 1, y, z); //palce a vertex in a lower right corner (looking from left)
						normal.add_data3(1, 0, 0);

						vertex.add_data3(x + 1, y + 1, z + 1); //place a vertex in an upper left corner (looking from left)
						normal.add_data3(1, 0, 0);

						vertex.add_data3(x + 1, y, z + 1); //place a vertex in an upper right corner (looking from left)
						normal.add_data3(1, 0, 0);

						//first triangle in a square  ----- connecting right vertices
						squarePrim->add_vertex(vertexID);
						squarePrim->add_vertex(vertexID + 2);
						squarePrim->add_vertex(vertexID + 1);

						//second triangle in a square
						squarePrim->add_vertex(vertexID + 3);
						squarePrim->add_vertex(vertexID + 1);
						squarePrim->add_vertex(vertexID + 2);

						vertexID = vertexID + 4;
					}

					if (easyMap[x][y][z].left == true) { //left vertices

						vertex.add_data3(x, y + 1, z); //palce a vertex in a lower left corner (looking from left)
						normal.add_data3(-1, 0, 0);

						vertex.add_data3(x, y, z); //palce a vertex in a lower right corner (looking from left)
						normal.add_data3(-1, 0, 0);

						vertex.add_data3(x, y + 1, z + 1); //palce a vertex in an upper left corner (looking from left)
						normal.add_data3(-1, 0, 0);

						vertex.add_data3(x, y, z + 1); //palce a vertex in an upper right corner (looking from left)
						normal.add_data3(-1, 0, 0);

						//first triangle in a square  ----- connecting left vertices
						squarePrim->add_vertex(vertexID);
						squarePrim->add_vertex(vertexID + 1);
						squarePrim->add_vertex(vertexID + 2);

						//second triangle in a square
						squarePrim->add_vertex(vertexID + 3);
						squarePrim->add_vertex(vertexID + 2);
						squarePrim->add_vertex(vertexID + 1);

						vertexID = vertexID + 4;
					}
				}
			}
		}


		//Create GeomNode
		PT(Geom) square = new Geom(vData);
		square->add_primitive(squarePrim);

		PT(GeomNode) squareNode = new GeomNode("squareNode");
		squareNode->add_geom(square);

		//Attach it to the terrainNode;
		terrainNode.attach_new_node(squareNode);

		//cout << "done!"; 
	}

private:

	int noiseMap[chunksize + 2][chunksize + 2];
	PT(GeomVertexData) vData = new GeomVertexData("square", GeomVertexFormat::get_v3n3(), Geom::UH_static);
	PT(GeomTriangles) squarePrim = new GeomTriangles(Geom::UH_static);
	int min_height = height;
	NodePath terrainNode;

	

	/*
	ID vertex normal
	0  0 0 0  0 0 1
	1
	2


	*/

};