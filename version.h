#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "09";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] =  "13.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 3;
	static const long MINOR  = 0;
	static const long BUILD  = 2216;
	static const long REVISION  = 12073;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3910;
	#define RC_FILEVERSION 3,0,2216,12073
	#define RC_FILEVERSION_STRING "3, 0, 2216, 12073\0"
	static const char FULLVERSION_STRING [] = "3.0.2216.12073";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 16;
	

}
#endif //VERSION_H
