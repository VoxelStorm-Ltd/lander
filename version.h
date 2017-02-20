#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "20";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 4;
	static const long MINOR  = 4;
	static const long BUILD  = 3729;
	static const long REVISION  = 20602;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 6775;
	#define RC_FILEVERSION 4,4,3729,20602
	#define RC_FILEVERSION_STRING "4, 4, 3729, 20602\0"
	static const char FULLVERSION_STRING [] = "4.4.3729.20602";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 29;
	

}
#endif //VERSION_H
