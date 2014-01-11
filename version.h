#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "11";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2014";
	static const char UBUNTU_VERSION_STYLE[] =  "14.01";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 4;
	static const long MINOR  = 4;
	static const long BUILD  = 3710;
	static const long REVISION  = 20515;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 6754;
	#define RC_FILEVERSION 4,4,3710,20515
	#define RC_FILEVERSION_STRING "4, 4, 3710, 20515\0"
	static const char FULLVERSION_STRING [] = "4.4.3710.20515";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 10;
	

}
#endif //VERSION_H
