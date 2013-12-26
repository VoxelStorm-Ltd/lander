#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "26";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] =  "13.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 3;
	static const long MINOR  = 9;
	static const long BUILD  = 3179;
	static const long REVISION  = 17461;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5767;
	#define RC_FILEVERSION 3,9,3179,17461
	#define RC_FILEVERSION_STRING "3, 9, 3179, 17461\0"
	static const char FULLVERSION_STRING [] = "3.9.3179.17461";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 79;
	

}
#endif //VERSION_H
