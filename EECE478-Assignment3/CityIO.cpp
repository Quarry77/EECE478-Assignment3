// John McDonnell
// 23880099

#include "CityIO.h"

// Default Constructor
CityIO::CityIO()
{
}

// Constructor
CityIO::CityIO(string file)
{
	string line;
	vector<string> citySpecs;
	ifstream cityFile(file);
	if(cityFile.is_open()) {
		while(getline(cityFile, line)) {
			line = ModelIO::RemoveLeadingWS(line);
			if(line.length() > 0 && line.at(0) != '#') {
				citySpecs.push_back(line);
			}
		}

	}
	cityFile.close();

	name = citySpecs[0];
	for( int i = 2; i < 2 + ModelIO::StringToInt(citySpecs[1]); i++ ) {
		building tmp;
		string fileLoc;
		stringstream ss(citySpecs[i]);
		ss >> fileLoc;
		tmp.model = ModelIO(fileLoc);
		ss >> tmp.sx;
		ss >> tmp.sy;
		ss >> tmp.sz;
		ss >> tmp.rx;
		ss >> tmp.ry;
		ss >> tmp.rz;
		ss >> tmp.tx;
		ss >> tmp.ty;
		ss >> tmp.tz;
		buildings.push_back(tmp);
	}
}

// Deconstructor
CityIO::~CityIO(void)
{
}

void CityIO::LoadSkyBoxTextures(vector<GLuint> &texIds ) {
	string skyboxLoc[] = {
	"./JohnSkybox/concrete512.ppm",
	"./JohnSkybox/TropicalSunnyDayBack2048.ppm",
	"./JohnSkybox/TropicalSunnyDayRight2048.ppm",
	"./JohnSkybox/TropicalSunnyDayFront2048.ppm",
	"./JohnSkybox/TropicalSunnyDayLeft2048.ppm",
	"./JohnSkybox/TropicalSunnyDayUp2048.ppm"
	};
	int width;
	int height;
	texIds.clear();

	for(int i = 0; i < 6; i++) {
		unsigned char* image = PPM::read(skyboxLoc[i], width, height);

		GLuint id;
		glGenTextures(1, &id);
		texIds.push_back(id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);
		
		delete[] image;
	}
}