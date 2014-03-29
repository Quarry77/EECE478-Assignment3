#ifndef _PPM_H_
#define _PPM_H_

#include <string>

namespace PPM {

	// read() creates memory dynamically - remember to delete[] it when you're finished using the image!
	// (You can delete the memory after creating the texture - the texture copies the data to graphics
	// memory.)
	
	unsigned char *read(const std::string &filename, int &width, int &height);

	void write(const std::string &filename, const unsigned char *img, const int width, const int height);

} // namespace PPM

#endif

