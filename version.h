#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "08";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] =  "13.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 2;
	static const long MINOR  = 10;
	static const long BUILD  = 2124;
	static const long REVISION  = 11627;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3727;
	#define RC_FILEVERSION 2,10,2124,11627
	#define RC_FILEVERSION_STRING "2, 10, 2124, 11627\0"
	static const char FULLVERSION_STRING [] = "2.10.2124.11627";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 24;
	

}
#endif //VERSION_H
