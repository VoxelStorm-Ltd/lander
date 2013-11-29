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
	static const long MINOR  = 7;
	static const long BUILD  = 708;
	static const long REVISION  = 4002;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1162;
	#define RC_FILEVERSION 1,7,708,4002
	#define RC_FILEVERSION_STRING "1, 7, 708, 4002\0"
	static const char FULLVERSION_STRING [] = "1.7.708.4002";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 8;
	

}
#endif //VERSION_H
