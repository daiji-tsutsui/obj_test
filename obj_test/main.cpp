//
//  main.cpp
//  obj_test
//
//  Created by 筒井 大二 on 2018/10/24.
//  Copyright © 2018年 筒井 大二. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include "OBJ_Loader.h"
using namespace std;

void idle(void);
void setup(void);
void resize(int width, int height);
void timer(int value);
void mouse(int button, int state, int cx, int cy);
void motion(int cx, int cy);
void display(void);

int btnFlg = 0;
double px, py;
double t = 0.0; double t0 = 0.0;
double s = 0.0; double s0 = 0.0;
double r = 2.5;
string username = "tsutsui";
string filename = "Untitled";
ostringstream s_input;
ostringstream s_output;
double pos[200][3][3] = {0};
int meshNum = 200;

int main(int argc, char* argv[]) {
	objl::Loader Loader;
	s_input << "/Users/" << username << "/Documents/C/obj_test/obj_test/" << filename << ".obj";
	bool loadout = Loader.LoadFile(s_input.str());
	
	if (loadout) {
		// Create/Open e1Out.txt
		s_output << "/Users/" << username << "/Documents/C/obj_test/obj_test/e1Out.txt";
		ofstream file(s_output.str());
		
		// Go through each loaded mesh and out its contents
		/* */meshNum = Loader.LoadedMeshes.size(); if(meshNum >= 200){ meshNum = 200; }
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++) {
			// Copy one of the loaded meshes to be our current mesh
			objl::Mesh curMesh = Loader.LoadedMeshes[i];
			file << "Mesh " << i << ": " << curMesh.MeshName << "\n";
			file << "Vertices:\n";
			
			// Go through each vertex and print its number,
			//  position, normal, and texture coordinate
			for (int j = 0; j < curMesh.Vertices.size(); j++) {
				file << "V" << j << ": " <<
				"P(" << curMesh.Vertices[j].Position.X << ", " << curMesh.Vertices[j].Position.Y << ", " << curMesh.Vertices[j].Position.Z << ") " <<
				"N(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << ") " <<
				"TC(" << curMesh.Vertices[j].TextureCoordinate.X << ", " << curMesh.Vertices[j].TextureCoordinate.Y << ")\n";
				/* */if(i < 200){
					pos[i][j][0] = curMesh.Vertices[j].Position.X / 10.0;
					pos[i][j][1] = curMesh.Vertices[j].Position.Y / 10.0 - 1.0;
					pos[i][j][2] = curMesh.Vertices[j].Position.Z / 10.0;
				}
			}
			
			// Print Indices
			file << "Indices:\n";
			
			// Go through every 3rd index and print the
			//	triangle that these indices represent
			for (int j = 0; j < curMesh.Indices.size(); j += 3) {
				file << "T" << j / 3 << ": " << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << "\n";
			}
			
			// Print Material
			file << "Material: " << curMesh.MeshMaterial.name << "\n";
			file << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
			file << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
			file << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
			file << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
			file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
			file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
			file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
			file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
			file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
			file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
			file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
			file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";
			file << "\n";
		}
		file.close();
	}
	else {
		s_output << "/Users/" << username << "/Documents/C/obj_test/obj_test/e1Out.txt";
		ofstream file(s_output.str());
		file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
		file.close();
	}
	
	/*--Main loop-------*/
	glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("3D model test");
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
//	glutIdleFunc(idle);
//	glutTimerFunc(100 , timer , 0);
	setup();
	glutMainLoop();
	
	return 0;
}

/*--For OpenGL-------------------------------------------------------------------------*/
void idle(void){
	glutPostRedisplay();
}
void setup(void) {
	glClearColor(1.0, 0.99, 0.91, 1.0);       //White
}
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,
				   (double)width/height,
				   1.0,
				   100.0);
	gluLookAt(0.0, 0.0, 2.5,       //Position of Camera
			  0.0, 0.0, 0.0,        //Position of Object
			  0.0, 1.0, 0.0);       //Upward direction of Camera
	glMatrixMode(GL_MODELVIEW);
}
void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(30 , timer , 0);
}
void mouse(int button, int state, int cx, int cy) {
	if(button == GLUT_LEFT_BUTTON){
		if(state == GLUT_DOWN){
			btnFlg = 1;
			px = cx; py = cy;
		}else if(state == GLUT_UP){
			btnFlg = 0;
			t0 = t;
			s0 = s;
		}
	}
}
void motion(int cx, int cy){
	if(btnFlg){
		t = t0 - 0.005 * (cx - px);
		s = max(min(s0 + 0.005 * (cy - py), M_PI/2), -M_PI/2);
		glutPostRedisplay();
	}
}

/*--Display func-------------------------------------------------------------------------*/
void display(void){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	gluLookAt(r*cos(s)*sin(t), r*sin(s), r*cos(s)*cos(t),
			  0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0);
	
	//x-axis
	glColor3d(1.0, 0.0, 0.0);	//Red
	glBegin(GL_LINES);
	glVertex3d(-2.0, 0.0, 0.0);
	glVertex3d(2.0, 0.0, 0.0);
	glEnd();
	//y-axis
	glColor3d(0.0, 1.0, 0.0);	//Green
	glBegin(GL_LINES);
	glVertex3d(0.0, -2.0, 0.0);
	glVertex3d(0.0, 2.0, 0.0);
	glEnd();
	//z-axis
	glColor3d(0.0, 0.0, 1.0);	//Blue
	glBegin(GL_LINES);
	glVertex3d(0.0, 0.0, -2.0);
	glVertex3d(0.0, 0.0, 2.0);
	glEnd();
	
	//3d model
	glPointSize(3.0);
	glColor3d(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for(int i = 0; i < meshNum; i++){
		for(int j = 0; j < 3; j++){
			glVertex3d(pos[i][j][0],pos[i][j][1],pos[i][j][2]);
		}
	}
	glEnd();
	
	
	glutSwapBuffers();
}
