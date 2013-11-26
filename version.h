#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "26";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] =  "13.11";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 5;
	static const long BUILD  = 549;
	static const long REVISION  = 3127;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 860;
	#define RC_FILEVERSION 1,5,549,3127
	#define RC_FILEVERSION_STRING "1, 5, 549, 3127\0"
	static const char FULLVERSION_STRING [] = "1.5.549.3127";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 49;
	

}
#endif //VERSION_H
