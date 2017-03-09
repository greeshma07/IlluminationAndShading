#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

using namespace std;

#include "data.h"
#include "functions.h"
#include "callbackFunctions.h"

int	m_nbFaces;
int	m_nbVertices;
GLpoint	*m_pFaceNormals=NULL;
GLpoint	*m_pVertNormals=NULL;
int	*m_pStorage=NULL;

void loadObjFile(char *fileName);
void ComputeVerticeNormal (int ixVertice)
{
	if (!m_pStorage)
	{
		m_pStorage = new int[m_nbFaces];
		if (!m_pStorage)
			return;
	}
	int nbAdjFaces = 0;
	GLFace * pFace = (GLFace *)&OBJ_FACES;
	for (int ix = 0; ix < m_nbFaces; ix++, pFace++)
		if (pFace->v1 == ixVertice)
			m_pStorage[nbAdjFaces++] = ix;
		else
			if (pFace->v2 == ixVertice)
				m_pStorage[nbAdjFaces++] = ix;
			else
				if (pFace->v3 == ixVertice)
					m_pStorage[nbAdjFaces++] = ix;
	GLpoint pn;
	pn.x = pn.y = pn.z = 0;
	for (int jx = 0; jx < nbAdjFaces; jx++)
	{
		int ixFace = m_pStorage[jx];
		pn.x += m_pFaceNormals[ixFace].x;
		pn.y += m_pFaceNormals[ixFace].y;
		pn.z += m_pFaceNormals[ixFace].z;
	}
	pn.x /= nbAdjFaces;
	pn.y /= nbAdjFaces;
	pn.z /= nbAdjFaces;

	// Normalize the vertex normal
	VectorNormalize(&pn, &m_pVertNormals[ixVertice]);
}


void startDraw(){
	if ((m_pFaceNormals == NULL))
	{
		m_pFaceNormals = new GLpoint[m_nbFaces];
		if (m_pFaceNormals)
		{
			GLFace * pFace = (GLFace *)&OBJ_FACES;
			for (int ix = 0; ix < m_nbFaces; ix++, pFace++)
				ComputeFaceNormal(&OBJ_VERTICES[pFace->v1], &OBJ_VERTICES[pFace->v2], &OBJ_VERTICES[pFace->v3], &m_pFaceNormals[ix]);
		}
	}

	// Compute and store vertex normals if necessary
	if (m_pFaceNormals && (m_pVertNormals == NULL))
	{
		m_pVertNormals = new GLpoint[m_nbFaces * 3];
		if (m_pVertNormals)
			for (int ix = 0; ix < m_nbVertices; ix++)
				ComputeVerticeNormal(ix);
	}
}
void renderAllTriangles(){	
	if(shadingType=="none"){
		for (int i = 0; i < tg_list.size(); i++) //triangle index
		{
			for (int j = 0; j < 3; j++) //vertex index
			{
				for (int k = 0; k < 3; k++) //color channel index
				{
					tg_list[i].c[j].channel[k] = 0.5;

				}
			}
		}
		for(int i = 0; i < tg_list.size(); i++){
			if(meshView)
				glBegin(GL_LINE_LOOP);
			else
				glBegin(GL_TRIANGLES);
			int pt0 = tg_list[i].v1;
			int pt1 = tg_list[i].v2;
			int pt2 = tg_list[i].v3;
			glVertex3f(vt_list[pt0].x, vt_list[pt0].y, vt_list[pt0].z);
			glVertex3f(vt_list[pt1].x, vt_list[pt1].y, vt_list[pt1].z);
			glVertex3f(vt_list[pt2].x, vt_list[pt2].y, vt_list[pt2].z);
			glEnd();
		}
	}

	else if(shadingType == "flat"){
		Vertex result;
		for (int i = 0; i < triangles_list.size(); i++) //triangle index
		{
			for (int j = 0; j < 3; j++) //vertex index
			{
				for (int k = 0; k < 3; k++) //color channel index
				{
					triangles_list[i].c[j].channel[k] = bunny_color.channel[0] * (ambient_color.channel[0] + light.c.channel[0] * max(0, int(light.x * light.cross(result).x))) +
														bunny_color.channel[1] * (ambient_color.channel[1] + light.c.channel[1] * max(0, int(light.y * light.cross(result).y))) +
														bunny_color.channel[2] * (ambient_color.channel[2] + light.c.channel[2] * max(0, int(light.z * light.cross(result).z)));
				}
			}
		}
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		float ld[4] = { 1, 1, 1, 0 };
		glLightfv(GL_LIGHT0, GL_AMBIENT, ld);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, ld);
		glLightfv(GL_LIGHT0, GL_SPECULAR, ld);	

		// Set a default material
		GLfloat a[4] = { 0, 0, 0, 1 };
		int ambient=0.2;
		glMaterialfv(GL_FRONT, ambient, a);
		GLfloat d[4] = { 1, 0, 0, 1 };
		glMaterialfv(GL_FRONT, ambient, d);
		int specular=1.0;
		glMaterialfv(GL_FRONT, specular, d);
		glMaterialf(GL_FRONT, GL_SHININESS, 128);
		if(meshView)
				glBegin(GL_LINE_LOOP);
		else
			glBegin(GL_TRIANGLES);
		GLFace * pFace = (GLFace *)&OBJ_FACES;
		for (int ix = 0; ix < m_nbFaces; ix++, pFace++)
		{		
			if (m_pFaceNormals)
			{
				// Flat shading
				glNormal3fv((float *)&m_pFaceNormals[ix]);
			}
			glVertex3fv((float *)&OBJ_VERTICES[pFace->v1]);
			glVertex3fv((float *)&OBJ_VERTICES[pFace->v2]);
			glVertex3fv((float *)&OBJ_VERTICES[pFace->v3]);
		
		}
		glEnd();	
	}
	else if(shadingType=="flatPhong"){
		eye_position.normalize();
		for (int i = 0; i < triangles_list.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int k = 0; k < 3; k++)
				{
					Vertex Cr,result;	
					Cr.x = 2 * (light.cross(result).x * (light.x * (float)(light.cross(result).x - (float)(light.x))));
					Cr.y = 2 * (light.cross(result).y * (light.y * (float)(light.cross(result).y - (float)(light.y))));
					Cr.z = 2 * (light.cross(result).z* (light.z * (float)(light.cross(result).z - (float)(light.z))));
			
					triangles_list[i].c[j].channel[k] = (bunny_color.channel[0] * (ambient_color.channel[0] + light.c.channel[0] * 
														max(0, int(light.x * light.cross(result).x)))+
														light.c.channel[0] * specular_color.channel[0] * max(0, (int)pow((eye_position.x * Cr.x), p))) +

														(bunny_color.channel[1] * (ambient_color.channel[1] + light.c.channel[1] *
														max(0, int(light.y * light.cross(result).y))) +
														light.c.channel[1] * specular_color.channel[1] * max(0, (int)pow((eye_position.y * Cr.y), p))) +

														(bunny_color.channel[2] * (ambient_color.channel[2] + light.c.channel[2] * 
														max(0, int(light.z * light.cross(result).z)))+
														light.c.channel[1] * specular_color.channel[1] * max(0, (int)pow((eye_position.z * Cr.z), p)));
				}
			}
		}
	}
}
void onDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0); //RGB
	/* Render all the triangles */
	renderAllTriangles();
	glFlush(); //clear the memory
}

int main(int argc, char * argv[]) {

	/*Initialize glut stuff*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Bunny");
	shadingType="none";
	loadObjFile("bunny_high.obj");
	populateVertexMatrices();

	m_nbFaces = sizeof(OBJ_FACES) / sizeof(OBJ_FACES[0]);
	m_nbVertices = sizeof(OBJ_VERTICES) / sizeof(OBJ_VERTICES[0]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glShadeModel(GL_FLAT);
	startDraw();
	initialize();
	/*Initialize gl stuff*/
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.4, 0.4, -0.4 * .48 / .64, 0.4 * .48 / .64, 2, 10); //parallel projection
	gluLookAt(0, 0, 5, 0, 0.2, 0, 0, 1, 0);


	/*Register GL stuff with the GLUT*/
	glutDisplayFunc(onDisplay);
	glutKeyboardFunc(onKeyboard);

	/*Initialize the loop*/
	glutMainLoop();
	

	delete(m_pFaceNormals);
	delete(m_pVertNormals);
	delete(m_pStorage);

	return 0;
}

void loadObjFile(char *fileName){
	ifstream file;
    file.open(fileName);
	if(file.fail()){
		cout<<"Failed to load object file "<<fileName<<endl;
		exit(1);
	}	
	string line,v,f;
	float x,y,z;
	int v1,v2,v3;
    while(!file.eof()){
        if(getline(file,line)){ 
			istringstream iss(line);
			if(line[0] == 'v'){
				iss >> v >> x >> y >> z;
				vertex.x=x;
				vertex.y=y;
				vertex.z=z;
				vt_list.push_back(vertex);
			}
			if(line[0] == 'f'){
				iss >> f >> v1 >> v2 >> v3;
				triangle.v1=v1-1;
				triangle.v2=v2-1;
				triangle.v3=v3-1;
				tg_list.push_back(triangle);
			}
		}
	}
}