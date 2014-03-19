// John McDonnell
// 23880099

#include "ModelIO.h"
#include <sstream>

using namespace std;

class CityIO
{
public:
	struct building{
		ModelIO model;
		float sx;
		float sy;
		float sz;
		float rx;
		float ry;
		float rz;
		float tx;
		float ty;
		float tz;
	};

	vector<building> buildings;
	string name;

	CityIO(string file);
	CityIO(void);
	~CityIO(void);
};

