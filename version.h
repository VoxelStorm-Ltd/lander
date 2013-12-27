#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "27";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] =  "13.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 4;
	static const long MINOR  = 0;
	static const long BUILD  = 3379;
	static const long REVISION  = 18611;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 6167;
	#define RC_FILEVERSION 4,0,3379,18611
	#define RC_FILEVERSION_STRING "4, 0, 3379, 18611\0"
	static const char FULLVERSION_STRING [] = "4.0.3379.18611";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 79;
	

}
#endif //VERSION_H
