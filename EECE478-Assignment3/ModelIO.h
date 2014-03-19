// John McDonnell
// 23880099

#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <sstream>
#include <GL/glut.h>

using namespace std;

class ModelIO
{
public:
	bool validModel;
	float defaultRadius;
	vector<string> model;
	string name;
	string filePath;
	vector<string> textures;
	vector<GLuint> textureIds;
	struct vertex {
		float x;
		float y;
		float z;
	};
	vector<vertex> vertices;
	vector<vertex> normals;
	struct triangle{
		int vertexIndex1;
		int vertexIndex2;
		int vertexIndex3;
		int normalIndex;
		int textureIndex;
		float textureCoord1;
		float textureCoord2;
		float textureCoord3;
		float textureCoord4;
		float textureCoord5;
		float textureCoord6;
	};
	vector<triangle> triangles;

	ModelIO(string file);
	ModelIO(void);
	~ModelIO(void);

	void RenderModel(void);
	static string RemoveLeadingWS(string str);
	static string RemoveTrailingWS(string str);
	static float StringToFloat(string str);
	static int StringToInt(string str);
	void LoadTextures(void);
	unsigned char *ReadPPM(string &filename, int &width, int &height);
};

