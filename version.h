#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "29";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] =  "13.11";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 8;
	static const long BUILD  = 849;
	static const long REVISION  = 4716;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1481;
	#define RC_FILEVERSION 1,8,849,4716
	#define RC_FILEVERSION_STRING "1, 8, 849, 4716\0"
	static const char FULLVERSION_STRING [] = "1.8.849.4716";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 49;
	

}
#endif //VERSION_H
