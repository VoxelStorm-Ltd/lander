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
	static const long MINOR  = 8;
	static const long BUILD  = 1902;
	static const long REVISION  = 10483;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3282;
	#define RC_FILEVERSION 2,8,1902,10483
	#define RC_FILEVERSION_STRING "2, 8, 1902, 10483\0"
	static const char FULLVERSION_STRING [] = "2.8.1902.10483";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 2;
	

}
#endif //VERSION_H
