

#ifndef IM_project_4_functions_h
#define IM_project_4_functions_h


#endif

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#define sqr(a) ((a) * (a))
using namespace std;

void VectorOffset (GLpoint *pIn, GLpoint *pOffset, GLpoint *pOut)
{
	pOut->x = pIn->x - pOffset->x;
	pOut->y = pIn->y - pOffset->y;
	pOut->z = pIn->z - pOffset->z;
}


// Compute the cross product a X b into pOut
void VectorGetNormal (GLpoint *a, GLpoint *b, GLpoint *pOut)
{
	pOut->x = a->y * b->z - a->z * b->y;
	pOut->y = a->z * b->x - a->x * b->z;
	pOut->z = a->x * b->y - a->y * b->x;
}


// Normalize pIn vector into pOut
bool VectorNormalize (GLpoint *pIn, GLpoint *pOut)
{
	GLfloat len = (GLfloat)(sqrt(sqr(pIn->x) + sqr(pIn->y) + sqr(pIn->z)));
	if (len)
	{
		pOut->x = pIn->x / len;
		pOut->y = pIn->y / len;
		pOut->z = pIn->z / len;
		return true;
	}
	return false;
}


// Compute p1,p2,p3 face normal into pOut
bool ComputeFaceNormal (GLpoint *p1, GLpoint *p2, GLpoint *p3, GLpoint *pOut)
{
	// Uses p2 as a new origin for p1,p3
	GLpoint a;
	VectorOffset(p3, p2, &a);
	GLpoint b;
	VectorOffset(p1, p2, &b);
	// Compute the cross product a X b to get the face normal
	GLpoint pn;
	VectorGetNormal(&a, &b, &pn);
	// Return a normalized vector
	return VectorNormalize(&pn, pOut);
}



void populateVertexMatrices()
{
	
	int sizeVert = vt_list.size();
	int sizeFaces = tg_list.size();
	for (int i=0; i < sizeVert; i++)
	{
		OBJ_VERTICES[i].x = (float)vt_list[i].x;
		OBJ_VERTICES[i].y = (float)vt_list[i].y;
		OBJ_VERTICES[i].z = (float)vt_list[i].z;
	}

	

	for (int i=0; i < sizeFaces; i++)
	{
		OBJ_FACES[i].v1 = (unsigned short)tg_list[i].v1;
		OBJ_FACES[i].v2 = (unsigned short)tg_list[i].v2;
		OBJ_FACES[i].v3 = (unsigned short)tg_list[i].v3;
	}


}

void initialize()
{
	light.x = 0;	//light position
	light.y = 2;
	light.z = 5;

	light.c.channel[0] = 0.8;
	light.c.channel[1] = 0.8;
	light.c.channel[2] = 0.8;

	light.normalize();

	light.c.channel[0] = 0.9;	//light color
	light.c.channel[1] = 0.9;
	light.c.channel[2] = 0.9;

	ambient_color.channel[0] = 0.2;	//ambient color
	ambient_color.channel[1] = 0.2;
	ambient_color.channel[2] = 0.2;

	bunny_color.channel[0] = 0.5;	//surface color
	bunny_color.channel[1] = 0.5;
	bunny_color.channel[2] = 0.5;

	phong_color.channel[0] = 0.8;
	phong_color.channel[1] = 0.8;
	phong_color.channel[2] = 0.8;

	specular_color.channel[0] = 1.0;
	specular_color.channel[1] = 1.0;
	specular_color.channel[2] = 1.0;

	eye_position.x = 3;
	eye_position.y = 2;
	eye_position.z = 0;

}
