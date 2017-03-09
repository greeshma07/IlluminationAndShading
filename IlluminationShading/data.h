
#ifndef IM_project_4_data_h
#define IM_project_4_data_h


#endif

#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

bool meshView=false;
string shadingType;

struct GLpoint {
	GLfloat x, y, z;
};


struct GLFace {
	unsigned short v1, v2, v3;
};



struct Color{
	GLfloat channel[3];
};


struct TriangleTest{
	int v1,v2,v3;
	Color c[3];
}triangle;

vector<TriangleTest> tg_list;

struct Vertex{
	float x;
	float y;
	float z;

	Color c;
	

	float normX; 
	float normY;
	float normZ;

	
	void normalize()
	{
		float len = x * x + y * y + z * z;
		normX = x / sqrt(len);
		normY = y / sqrt(len);
		normZ = z / sqrt(len);
	}

	const float operator * (const Vertex& right) const
	{
		float result;
		
		Vertex v4;
		result = (v4.x*right.x) + (v4.y*right.y) + (v4.z*right.z);

		return result;
	}


	
	const Vertex operator - (const Vertex& right) const
	{
		Vertex result;
		Vertex v4;
			result.x = v4.x - right.x;
			result.y = v4.y - right.y;
			result.z = v4.z - right.z;

		return result;
	}

	/*The cross-product */
	Vertex cross(const Vertex& right)
	{
		Vertex result;
		
		for (int i = 0; i < tg_list.size(); i += 3)
		{
			Vertex V1, V2;
			v1.x = tg_list[i].v1;
			v1.y = tg_list[i].v2;
			v1.z = tg_list[i].v3;

			v2.x = tg_list[i + 1].v1;
			v2.y = tg_list[i + 1].v2;
			v2.z = tg_list[i + 1].v3;

			v3.x = tg_list[i + 2].v1;
			v3.y = tg_list[i + 2].v2;
			v3.z = tg_list[i + 2].v3;

			V1 = (v3 - v1);
			V2 = (v2 - v1);
		}

		result.x = v1.y*v2.z - v1.z*v2.y;
		result.y = v1.z*v2.x - v1.x*v2.z;
		result.z = v1.x*v2.y - v1.y*v2.x;

		result.normalize();

		return result;
	}
}v1,v2,v3;

struct Vec{
	float elem[4];


};

struct Mat{
	float elem[4][4];
	void eye()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i == j)
					elem[i][j] = 1.0f;
				else
					elem[i][j] = 0.0f;
			}
		}
	}
	const Mat operator * (const Mat& right) const
	{
		Mat result;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				result.elem[i][j] = 0;
				for (int k = 0; k < 4; k++)
				{
					result.elem[i][j] += elem[i][k] * right.elem[k][j];
				}
			}
		}

		return result;
	}
	const Vec operator * (const Vec& vec) const
	{
		Vec result;

		for (int i = 0; i < 4; i++)
		{

			result.elem[i] = 0;
			for (int k = 0; k < 4; k++)
			{
				result.elem[i] += elem[i][k] * vec.elem[k];
			}

		}

		return result;
	}

	
};


#define MAX_SIZE_VERTICES 20002
#define MAX_SIZE_FACES 40000
GLpoint OBJ_VERTICES [MAX_SIZE_VERTICES] ;
GLFace OBJ_FACES[MAX_SIZE_FACES] ;

struct VertexTest{
	float x, y, z;
	float data[4];   
}vertex;


vector<VertexTest> vt_list; 

vector<Vertex> vertice_list;
vector<TriangleTest> triangles_list;
/*Define the light and colors*/
Vertex light;
Color ambient_color;
Color bunny_color;
Color phong_color;
Color specular_color;
Vertex eye_position;
int p = 0.5;

