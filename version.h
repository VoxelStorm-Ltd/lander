#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "20";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] =  "13.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 3;
	static const long MINOR  = 3;
	static const long BUILD  = 2579;
	static const long REVISION  = 14180;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4604;
	#define RC_FILEVERSION 3,3,2579,14180
	#define RC_FILEVERSION_STRING "3, 3, 2579, 14180\0"
	static const char FULLVERSION_STRING [] = "3.3.2579.14180";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 79;
	

}
#endif //VERSION_H
