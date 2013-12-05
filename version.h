#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "05";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] =  "13.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 2;
	static const long MINOR  = 3;
	static const long BUILD  = 1474;
	static const long REVISION  = 8088;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 2559;
	#define RC_FILEVERSION 2,3,1474,8088
	#define RC_FILEVERSION_STRING "2, 3, 1474, 8088\0"
	static const char FULLVERSION_STRING [] = "2.3.1474.8088";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 74;
	

}
#endif //VERSION_H
