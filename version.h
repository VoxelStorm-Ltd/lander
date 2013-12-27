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
	static const long MAJOR  = 3;
	static const long MINOR  = 10;
	static const long BUILD  = 3253;
	static const long REVISION  = 17882;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 5940;
	#define RC_FILEVERSION 3,10,3253,17882
	#define RC_FILEVERSION_STRING "3, 10, 3253, 17882\0"
	static const char FULLVERSION_STRING [] = "3.10.3253.17882";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 53;
	

}
#endif //VERSION_H
