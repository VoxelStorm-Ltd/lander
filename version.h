#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "27";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] =  "13.11";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 6;
	static const long BUILD  = 684;
	static const long REVISION  = 3871;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1110;
	#define RC_FILEVERSION 1,6,684,3871
	#define RC_FILEVERSION_STRING "1, 6, 684, 3871\0"
	static const char FULLVERSION_STRING [] = "1.6.684.3871";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 84;
	

}
#endif //VERSION_H
