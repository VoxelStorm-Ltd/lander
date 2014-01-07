#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "07";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2014";
	static const char UBUNTU_VERSION_STYLE[] =  "14.01";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 4;
	static const long MINOR  = 3;
	static const long BUILD  = 3675;
	static const long REVISION  = 20311;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 6699;
	#define RC_FILEVERSION 4,3,3675,20311
	#define RC_FILEVERSION_STRING "4, 3, 3675, 20311\0"
	static const char FULLVERSION_STRING [] = "4.3.3675.20311";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 75;
	

}
#endif //VERSION_H
