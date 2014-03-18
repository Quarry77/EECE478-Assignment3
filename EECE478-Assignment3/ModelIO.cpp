// John McDonnell
// 23880099

#include "ModelIO.h"

// Default Constructor
ModelIO::ModelIO()
{
	validModel = false;
}

// Constructor
ModelIO::ModelIO(string file)
{
	ifstream modelFile(file);
	filePath = file;
	string line;
	validModel = false;
	defaultRadius = 1;
	name = "";
	int nameIndex[2];
	int texturesIndex[2];
	int verticesIndex[2];
	int normalsIndex[2];
	int trianglesIndex[2];

	// Read from model file
	cout << "Loading Model..." << endl;
	if(modelFile.is_open()) {
		int count = 0;
		while(getline(modelFile, line)) 
		{
			// Strip all empty and commented lines and count the rest
			line = RemoveLeadingWS(line);
			if(line.size() > 0 && line.at(0) != '#') {
				count++;
			}
		}

		modelFile.clear() ;
		modelFile.seekg(0, modelFile.beg);
		model.resize(count);

		count = 0;
		while(getline(modelFile, line)) 
		{
			line = RemoveLeadingWS(RemoveTrailingWS(line));
			if(line.size() > 0 && line.at(0) != '#') {
				if(line == "<name>")
					nameIndex[0] = count + 1;
				else if(line == "</name>")
					nameIndex[1] = count;
				else if(line == "<textures>")
					texturesIndex[0] = count + 1;
				else if(line == "</textures>")
					texturesIndex[1] = count;
				else if(line == "<vertices>")
					verticesIndex[0] = count + 1;
				else if(line == "</vertices>")
					verticesIndex[1] = count;
				else if(line == "<normals>")
					normalsIndex[0] = count + 1;
				else if(line == "</normals>")
					normalsIndex[1] = count;
				else if(line == "<triangles>")
					trianglesIndex[0] = count + 1;
				else if(line == "</triangles>")
					trianglesIndex[1] = count;

				model.at(count) = line;
				count++;
			}
		}

		modelFile.close();
		cout << "Model Loaded. Parsing..." << endl;
	} else {
		cout << "Unable to open file" << endl;
		return;
	}
	
	// Name
	for( unsigned int i = nameIndex[0]; i < nameIndex[1]; i++ ) {
		name += model.at(i) + " ";
	}
	name = RemoveTrailingWS(name);

	// Textures
	textures.resize(texturesIndex[1] - texturesIndex[0]);
	int k = 0;
	for( int i = texturesIndex[0]; i < texturesIndex[1]; i++ ) {
		textures.at(k) = model.at(i);
		k++;
	}

	// Vertices
	vertices.resize(verticesIndex[1] - verticesIndex[0]);
	k = 0;
	for( int i = verticesIndex[0]; i < verticesIndex[1]; i++ ) {
		vertex tmp;
		string thisLine = model.at(i);
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.x = StringToFloat(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.y = StringToFloat(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		tmp.z = StringToFloat(thisLine);
		vertices.at(k) = tmp;
		k++;

		// Set the default viewing distance based on vertex data
		float dist = sqrt(tmp.x * tmp.x + tmp.y * tmp.y + tmp.z * tmp.z);
		if(dist * 1.5 > defaultRadius) {defaultRadius = dist * 1.5;}
	}

	// Normals
	normals.resize(normalsIndex[1] - normalsIndex[0]);
	k = 0;
	for( int i = normalsIndex[0]; i < normalsIndex[1]; i++ ) {
		vertex tmp;
		string thisLine = model.at(i);
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.x = StringToFloat(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.y = StringToFloat(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		tmp.z = StringToFloat(thisLine);
		normals.at(k) = tmp;
		k++;
	}

	// Triangles
	triangles.resize(trianglesIndex[1] - trianglesIndex[0]);
	k = 0;
	for( int i = trianglesIndex[0]; i < trianglesIndex[1]; i++ ) {
		triangle tmp;
		string thisLine = model.at(i);
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.vertexIndex1 = StringToInt(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.vertexIndex2 = StringToInt(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.vertexIndex3 = StringToInt(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.normalIndex = StringToInt(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.textureIndex = StringToInt(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.textureCoord1 = StringToFloat(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.textureCoord2 = StringToFloat(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.textureCoord3 = StringToFloat(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.textureCoord4 = StringToFloat(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		for(unsigned int j = 0; j < thisLine.size(); j++) {
			if(isspace(thisLine.at(j))) {
				tmp.textureCoord5 = StringToFloat(thisLine.substr(0, j));
				thisLine = RemoveLeadingWS(thisLine.substr(j));
				break;
			}
		}
		tmp.textureCoord6 = StringToFloat(thisLine);
		triangles.at(k) = tmp;
		k++;
	}

	// Simple test to see if the model has any substance
	if(triangles.size() > 0 && vertices.size() > 0 && normals.size() > 0) {
		validModel = true;
	}
}

// Deconstructor
ModelIO::~ModelIO(void)
{
}

// Draws the triangles for the loaded model
void ModelIO::RenderModel(void)
{
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	glEnable(GL_TEXTURE_2D);
	for(unsigned int i = 0; i < triangles.size(); i++) {
		// Randomizes the colors in a predefined way to show contours without changing each frame
		//srand(i);
		//glColor3f((rand() % 105 + 5)/100.0, (rand() % 105 + 5)/100.0, (rand() % 105 + 5)/100.0);

		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
		glBindTexture(GL_TEXTURE_2D, textureIds.at(triangles.at(i).textureIndex));
		glBegin(GL_TRIANGLES);

		glNormal3f(normals.at(triangles.at(i).normalIndex).x,normals.at(triangles.at(i).normalIndex).y,normals.at(triangles.at(i).normalIndex).z);
		glTexCoord2f(triangles.at(i).textureCoord1, triangles.at(i).textureCoord2);
		glVertex3f(vertices.at(triangles.at(i).vertexIndex1).x, vertices.at(triangles.at(i).vertexIndex1).y, vertices.at(triangles.at(i).vertexIndex1).z);
		glTexCoord2f(triangles.at(i).textureCoord3, triangles.at(i).textureCoord4);
		glVertex3f(vertices.at(triangles.at(i).vertexIndex2).x, vertices.at(triangles.at(i).vertexIndex2).y, vertices.at(triangles.at(i).vertexIndex2).z);
		glTexCoord2f(triangles.at(i).textureCoord5, triangles.at(i).textureCoord6);
		glVertex3f(vertices.at(triangles.at(i).vertexIndex3).x, vertices.at(triangles.at(i).vertexIndex3).y, vertices.at(triangles.at(i).vertexIndex3).z);
		
		glEnd();
	}

}

// Removes white space at the beginning of the string
string ModelIO::RemoveLeadingWS(string str)
{
	for(unsigned int i = 0; i < str.size(); i++) {
		if(!isspace(str.at(i))) {
			return str.substr(i);
		}
	}
	return str;
}

// Removes white space at the end of the string
string ModelIO::RemoveTrailingWS(string str)
{
	for(int i = str.size() - 1; i >= 0; i--) {
		if(!isspace(str.at(i))) {
			return str.substr(0,i + 1);
		}
	}
	return str;
}

// Convert from a string to a float
float ModelIO::StringToFloat(string str)
{
	return atof(str.c_str());
}

// Convert from a string to an int
int ModelIO::StringToInt(string str)
{
	return atof(str.c_str());
}

void ModelIO::LoadTextures()
{
	string directory = "";
	for(int i = filePath.size() - 1; i >= 0; i--){
		if(filePath.at(i) == '/'){
			directory = filePath.substr(0, i + 1);
			break;
		}
	}

	int width;
	int height;
	textureIds.resize(textures.size());

	for(int i = 0; i < textures.size(); i++){
		unsigned char* image = ReadPPM(directory + textures.at(i), width, height);

		glGenTextures(1, &textureIds.at(i));
		glBindTexture(GL_TEXTURE_2D, textureIds.at(i));

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
		
		delete[] image;
	}

}

// Adapted from ppm.cpp in Assignment 2 Useful Links
unsigned char* ModelIO::ReadPPM(string &filename, int &width, int &height)
{
	FILE *ifp;
	char buffer[80];
	int i;
	int header[3];
	int tmp;

	ifp = fopen(filename.c_str(), "rb");
	if( !ifp ) {
		throw std::string("Error opening file \"") + filename + std::string("\"");
	}
	  
	i = 0;
	fgets(buffer, 80, ifp);
	if( strncmp(buffer, "P6", 2) ) {
		fclose(ifp);
		throw std::string("File is not in valid PPM format");
	}

	while( (i < 3) ) {
		if( (tmp=fgetc(ifp)) == '#' ) {
		fgets(buffer, 80, ifp);
		continue;
		} else {
		ungetc(tmp, ifp);
		fscanf(ifp, "%d", &header[i++]);
		}
	}
	fgets(buffer, 80, ifp);

	// Renew image
	width = header[0];
	height = header[1];
	unsigned char *img = new unsigned char[width*height*3];

	fread(img, 3, width * height, ifp);
	img;
	  
	fclose(ifp);
	return img;
}