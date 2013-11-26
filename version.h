#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "26";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] =  "13.11";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 4;
	static const long BUILD  = 459;
	static const long REVISION  = 2605;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 683;
	#define RC_FILEVERSION 1,4,459,2605
	#define RC_FILEVERSION_STRING "1, 4, 459, 2605\0"
	static const char FULLVERSION_STRING [] = "1.4.459.2605";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 59;
	

}
#endif //VERSION_H
