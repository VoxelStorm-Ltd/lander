#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "04";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] =  "13.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 2;
	static const long MINOR  = 0;
	static const long BUILD  = 1158;
	static const long REVISION  = 6328;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1960;
	#define RC_FILEVERSION 2,0,1158,6328
	#define RC_FILEVERSION_STRING "2, 0, 1158, 6328\0"
	static const char FULLVERSION_STRING [] = "2.0.1158.6328";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 58;
	

}
#endif //VERSION_H
