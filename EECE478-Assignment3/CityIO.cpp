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
